#include <stdio.h>
#include <stdlib.h>

#include "chessSystem.h"
#include "map.h"
#include "mapUtil.h"
#include "tournament.h"
#include "game.h"
#include "player.h"
#include "playerInTournament.h"

struct chess_system_t {
    Map tournaments;
    Map players;
};


//================== INTERNAL FUNCTIONS START ==================//

// Checks if a player plays in a given tournament
// static bool isPlayerPlayingInTournament(ChessSystem chess, int player_id, int tournament_id)
// {
//     if (chess == NULL || player_id <= 0 || tournament_id <= 0)
//     {
//         return false;
//     }

//     Player wanted_player = mapGet(chess->players, &player_id);
//     if (wanted_player == NULL)
//     {
//         return false;
//     }

//     return playerIsPlayingInTournament(wanted_player, tournament_id);
// }


// Checks if 2 players play against eachother in a given tournament
static bool isGameBetweenPlayersExists(ChessSystem chess, int tournament_id,
                                       int first_player_id, int second_player_id)
{
    if (chess == NULL)
    {
        return false;
    }
    
    Player first_player = mapGet(chess->players, &first_player_id);
    Tournament tournament = mapGet(chess->tournaments, &tournament_id);
    
    if (first_player == NULL || tournament == NULL)
    {
        return false;
    }
    
    int *game_ids = playerGetGameIdsInTournament(first_player, tournament_id);
    if (game_ids == NULL)
    {
        return false;
    }

    int max_games_per_player = tournamentGetMaxGamesPerPlayer(tournament);
    for (int i = 0 ; i < max_games_per_player ; i++)
    {
        if (game_ids[i] == INVALID_GAME_ID)
        {
            return false;
        }
        
        Game current_game = tournamentGetGame(tournament, game_ids[i]);
        if (gameisPlayerInGame(current_game, second_player_id))
        {
            return true;
        }
    }
    return false;
}


//
static bool isPlayerInTournamentExceededGames(ChessSystem chess, int player_id,
                                              int tournament_id)
{
    Player player = mapGet(chess->players, &player_id);
    return playerCanPlayMoreGamesInTournament(player, tournament_id);
}



// Verify the input is valid according to demands
static ChessResult chessAddGameVerifyInput(ChessSystem chess, int tournament_id, int first_player,
                                int second_player, int play_time)
{
    if (chess == NULL)
    {
        return CHESS_NULL_ARGUMENT;
    }

    if (tournament_id <= 0 || first_player <= 0 || second_player <= 0  || first_player == second_player)
    {
        return CHESS_INVALID_ID;
    }

    if (mapContains(chess->tournaments , &tournament_id) == false)
    {
        return CHESS_TOURNAMENT_NOT_EXIST;
    }
     
    Tournament tournament = mapGet(chess->tournaments, &tournament_id);
    if (tournamentGetWinner(tournament) <= 0)
    {
        return CHESS_TOURNAMENT_ENDED;
    }

    if (isGameBetweenPlayersExists(chess, tournament_id, first_player, second_player) == true)
    {
        return CHESS_GAME_ALREADY_EXISTS;
    }

    if (play_time < 0)
    {
        return CHESS_INVALID_PLAY_TIME;
    }

    if (isPlayerInTournamentExceededGames(chess, first_player, tournament_id) ||
        isPlayerInTournamentExceededGames(chess, second_player, tournament_id))
    {
        return CHESS_EXCEEDED_GAMES;
    }

    return CHESS_SUCCESS;
}


// Creates players if needed, updates the amount_of_new_players int for each
// new player created. Returns ChessResult according to the function's outcome
static ChessResult chessAddGameCreatePlayersIfNeeded(ChessSystem chess, /*int *amount_of_new_player,*/
                                        int first_player, int second_player)
{
    bool first_player_created = false; 

    if (mapContains(chess->players, &first_player) == false)
    {
        Player first_player_struct = playerCreate(first_player);
        if (first_player_struct == NULL)
        {
            return CHESS_OUT_OF_MEMORY;
        }

        MapResult put_result = mapPut(chess->players, &first_player, first_player_struct);
        playerDestroy(first_player_struct);

        if (put_result != MAP_SUCCESS)
        {
            return CHESS_OUT_OF_MEMORY;
        }
        first_player_created = true;
        //*amount_of_new_player += 1;
    }

    if (mapContains(chess->players, &second_player) == false)
    {
        Player second_player_struct = playerCreate(second_player);
        if (second_player_struct == NULL)
        {
            return CHESS_OUT_OF_MEMORY;
        }

        MapResult put_result = mapPut(chess->players, &second_player, second_player_struct);
        playerDestroy(second_player_struct);

        if (put_result != MAP_SUCCESS)
        {
            if (first_player_created)
            {
                mapRemove(chess->players, &first_player);
            }
            return CHESS_OUT_OF_MEMORY;
        }

        //*amount_of_new_player += 1;
    }

    return CHESS_SUCCESS;
}

//================== INTERNAL FUNCTIONS END ==================//


ChessSystem chessCreate()
{
    ChessSystem chess_system = malloc(sizeof(*chess_system));
    
    if (chess_system == NULL)
    {
        return NULL;
    }

    Map players     = createPlayersMap();
    if (players == NULL)
    {
        free(chess_system);
        return NULL;
    }
    Map tournaments = createTournamentsMap();

    if (tournaments == NULL)
    {
        free(players);
        free(chess_system);
        return NULL;
    }

    chess_system->players = players;
    chess_system->tournaments = tournaments;

    return chess_system;
}

void chessDestroy(ChessSystem chess)
{
    if (chess == NULL)
    {
        return;
    }

    mapDestroy(chess->players);
    mapDestroy(chess->tournaments);
    free(chess);
}


ChessResult chessAddTournament (ChessSystem chess, int tournament_id,
                                int max_games_per_player, const char* tournament_location)
{
    if (chess == NULL)
    {
        return CHESS_NULL_ARGUMENT;
    }

    if (tournament_id <= 0)
    {
        return CHESS_INVALID_ID;
    }

    if (mapContains(chess->tournaments, &tournament_id))
    {
        return CHESS_TOURNAMENT_ALREADY_EXISTS;
    }

    if (!tournamentValidateLocation(tournament_location))
    {
        return CHESS_INVALID_LOCATION;
    }

    if (max_games_per_player <= 0)
    {
        return CHESS_INVALID_MAX_GAMES;
    }

    Tournament new_tournament = tournamentCreate(tournament_id, max_games_per_player, tournament_location);
    if (new_tournament == NULL)
    {
        return CHESS_OUT_OF_MEMORY;
    }
    MapResult tournament_put_result = mapPut(chess->tournaments, &tournament_id, new_tournament);
    tournamentDestroy(new_tournament); // A copy was sent to the map, no need for the object at this point

    if (tournament_put_result == MAP_OUT_OF_MEMORY)
    {
        return CHESS_OUT_OF_MEMORY;
    }

    return CHESS_SUCCESS;
}


ChessResult chessAddGame(ChessSystem chess, int tournament_id, int first_player,
                                int second_player, Winner winner, int play_time)
{

    ChessResult verify_input = chessAddGameVerifyInput(chess, tournament_id,
                                    first_player, second_player, play_time);
    
    if (verify_input != CHESS_SUCCESS)
    {
        return verify_input;
    }
    
    
    
    // Checks if players already exist in the system
    //int amount_of_new_players = 0;
    ChessResult player_create_result = chessAddGameCreatePlayersIfNeeded(
                                        chess, first_player, second_player);
    if (player_create_result != CHESS_SUCCESS)
    {
        return player_create_result;
    }



    return CHESS_SUCCESS;
}

//     // CHECK IF GAME ALREADY EXISTS - STATIC HELPER FUNCTION
//     // CHECK AMOUNT OF NEW PLAYERS WITH isPlayerPlayingInTournament
 


//ChessResult chessRemoveTournament (ChessSystem chess, int tournament_id)


//ChessResult chessRemovePlayer(ChessSystem chess, int player_id)


//ChessResult chessEndTournament (ChessSystem chess, int tournament_id)
// calculate the winner

//double chessCalculateAveragePlayTime (ChessSystem chess, int player_id, ChessResult* chess_result)


//ChessResult chessSavePlayersLevels (ChessSystem chess, FILE* file)


//ChessResult chessSaveTournamentStatistics (ChessSystem chess, char* path_file)


//static int ChessTournamentCalculateWinner (Tournament tournament)
//{
    //TOURNAMENT_WIN_WEIGHT
//}
