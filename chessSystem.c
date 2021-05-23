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
static bool isPlayerPlayingInTournament(ChessSystem chess, int player_id, int tournament_id)
{
    if (chess == NULL || player_id <= 0 || tournament_id <= 0)
    {
        return false;
    }

    Player wanted_player = mapGet(chess->players, &player_id);
    if (wanted_player == NULL)
    {
        return false;
    }

    return playerIsPlayingInTournament(wanted_player, tournament_id);
}


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
    bool return_value = playerCanPlayMoreGamesInTournament(player, tournament_id);
    return !(return_value);
}



// Verify the input is valid for the chessAddGame function
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
    if (tournamentGetWinner(tournament) > 0)
    {
        return CHESS_TOURNAMENT_ENDED;
    }

    if (isGameBetweenPlayersExists(chess, tournament_id, first_player, second_player) == true)
    {
        return CHESS_GAME_ALREADY_EXISTS;
    }

    return CHESS_SUCCESS;
}


// Creates players if needed. Returns ChessResult according to the function's outcome
static ChessResult chessAddGameCreatePlayersIfNeeded(ChessSystem chess,
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

    }

    return CHESS_SUCCESS;
}


// Creates the game, adds it to the players' & tournament records
static ChessResult chessAddGameTournamentAndPlayer(Tournament tournament, Player first_player_struct,
                            Player second_player_struct, Winner winner, int play_time,
                            int amount_of_new_players, int max_games_per_player)
{
    ChessResult new_game_result =  tournamentAddGame(tournament, playerGetID(first_player_struct),
                        playerGetID(second_player_struct), winner, play_time, amount_of_new_players);
    
    if (new_game_result != CHESS_SUCCESS)
    {
        return new_game_result;
    }
    int new_game_id = tournamentGetSizeGames(tournament) - 1;
    Game new_game = tournamentGetGame(tournament, new_game_id);

    PlayerResult add_game_result = playerAddGame(first_player_struct, new_game, max_games_per_player);
    if (add_game_result != PLAYER_SUCCESS)
    {
        return CHESS_OUT_OF_MEMORY;
    }
    add_game_result = playerAddGame(second_player_struct, new_game, max_games_per_player);
    if (add_game_result != PLAYER_SUCCESS)
    {
        return CHESS_OUT_OF_MEMORY;
    }

    return CHESS_SUCCESS;
}


// Creates PlayerInTournaments for the players if needed,
// updating the amount_of_new_players pointer in the process
static ChessResult chessAddGameCreatePlayerInTournamentsIfNeeded(int tournament_id,
                  Player first_player_struct, Player second_player_struct,
                  int max_games_per_player, int *amount_of_new_players)
{
    // player's first game in tournament
    if (playerGetGameIdsInTournament(first_player_struct, tournament_id) == NULL)
    {
        *amount_of_new_players += 1;
        if (playerAddTournament(first_player_struct, tournament_id,
                            max_games_per_player) == PLAYER_OUT_OF_MEMORY)
        {
            return CHESS_OUT_OF_MEMORY;
        }
    }

    if (playerGetGameIdsInTournament(second_player_struct, tournament_id) == NULL)
    {
        *amount_of_new_players += 1;
        if (playerAddTournament(second_player_struct, tournament_id,
                            max_games_per_player) == PLAYER_OUT_OF_MEMORY)
        {
            return CHESS_OUT_OF_MEMORY;
        }
    }

    return CHESS_SUCCESS;
}


// Updates the result of a game after a player was removed
static void chessRemovePlayerUpdateGameResult(ChessSystem chess, int tournament_id,
                                              int game_id, int player_id)
{
    // Get game, get opponent, update outcome
    Tournament tournament = mapGet(chess->tournaments, &tournament_id);
    Game current_game = tournamentGetGame(tournament, game_id);
    int opponent_id   = gameGetPlayersOpponent(current_game, player_id);
    Player opponent   = mapGet(chess->players, &opponent_id);
    int game_winner   = gameGetIdOfWinner(current_game);
    if (game_winner  == opponent_id)
    {
        return;
    }
    if (game_winner == INVALID_PLAYER)
    {
        playerUpdateResultsAfterOpponentDeletion(opponent,tournament_id , DRAW_TO_WIN);
        return;
    }

    playerUpdateResultsAfterOpponentDeletion(opponent, tournament_id, LOSE_TO_WIN);
}


// Verifies input for the chessRemovePlayer function
static ChessResult chessRemovePlayerVerifyInput(ChessSystem chess, int player_id)
{
    if (chess == NULL)
    {
        return CHESS_NULL_ARGUMENT;
    }

    if (player_id <= 0)
    {
        return CHESS_PLAYER_NOT_EXIST;
    }

    if (!mapContains(chess->players, &player_id))
    {
        return CHESS_PLAYER_NOT_EXIST;
    }

    return CHESS_SUCCESS;
}


// Calculates the score of a given player in a tournament
static int chessCalculatePlayerScore(ChessSystem chess, int player_id, int tournament_id)
{
    if (chess == NULL)
    {
        return -1;
    }
    
    Player player = mapGet(chess->players, &player_id);
    if (player == NULL)
    {
        return -1;
    }

    int *game_ids = playerGetGameIdsInTournament(player, tournament_id);
    if (game_ids == NULL)
    {
        return -1;
    }

    Tournament tournament = mapGet(chess->tournaments, &tournament_id);
    if (tournament == NULL)
    {
        return -1;
    }
    
    int max_games_per_player = tournamentGetMaxGamesPerPlayer(tournament);
    int score = 0;
    for (int i = 0 ; i < max_games_per_player ; i++)
    {
        Game game = tournamentGetGame(tournament, game_ids[i]);
        int game_winner = gameGetIdOfWinner(game);
        if (game_winner == player_id)
        {
            score += TOURNAMENT_WIN_WEIGHT;
        }

        if (game_winner == INVALID_PLAYER)
        {
            score += TOURNAMENT_DRAW_WEIGHT;
        }
    }

    return score;
}


// Returns the ID of the player that should be the winner, determined by score, wins, id...
static int ChessTournamentComparePlayerWithWinner (ChessSystem chess, int tournament_id,
                    int winner_id, int winner_score, int player_id, int *player_score)
{
    if (chess == NULL || player_score == NULL)
    {
        return INVALID_PLAYER;
    }

    *player_score = chessCalculatePlayerScore(chess, player_id, tournament_id);

    if (*player_score < winner_score)
    {
        return winner_id;
    }

    if (*player_score > winner_score)
    {
        return player_id;
    }

    // Both player share the same score
    Player winner = mapGet(chess->players, &winner_id);
    Player player = mapGet(chess->players, &player_id);
    int loss_compare = playerGetLossesInTournament(winner, tournament_id) -
                        playerGetLossesInTournament(player, tournament_id);

    if (loss_compare < 0)
    {
        return winner_id;
    }

    if (loss_compare > 0)
    {
        return player_id;
    }

    int win_compare = playerGetWinsInTournament(winner, tournament_id) -
                        playerGetWinsInTournament(player,tournament_id);

    if (win_compare > 0)
    {
        return winner_id;
    }

    if (win_compare < 0)
    {
        return player_id;
    }
    
    return winner_id > player_id ? winner_id : player_id;
    
}


// Calculates the winner in a given tournament and returns their ID
static int ChessTournamentCalculateWinner (ChessSystem chess, int tournament_id)
{
    if (chess == NULL)
    {
        return INVALID_PLAYER;
    }

    Tournament tournament = mapGet(chess->tournaments, &tournament_id);
    if (tournament == NULL)
    {
        return INVALID_PLAYER;
    }
    int amount_of_games = tournamentGetSizeGames(tournament);
    int *player_iterator = mapGetFirst(chess->players);
    if (player_iterator == NULL || amount_of_games == 0)
    {
        return INVALID_PLAYER;
    }

    int winner_id = *player_iterator;
    int winner_score     = chessCalculatePlayerScore(chess, winner_id, tournament_id);
    //free free freefreefreefreefree
    while (player_iterator)
    {
        bool is_playing = isPlayerPlayingInTournament(chess, *player_iterator, tournament_id);
        if (!is_playing)
        {
            player_iterator = mapGetNext(chess->players);
            continue;
        }

        int player_score = 0;
        int new_winner = ChessTournamentComparePlayerWithWinner(chess, tournament_id, winner_id,
                                            winner_score, *player_iterator, &player_score);
        
        if (new_winner == *player_iterator)
        {
            winner_id = new_winner;
            winner_score = player_score;
        }
        player_iterator = mapGetNext(chess->players);
    }

    return winner_id;
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
    // Verifying basic input
    ChessResult verify_input = chessAddGameVerifyInput(chess, tournament_id,
                                    first_player, second_player, play_time);
    
    if (verify_input != CHESS_SUCCESS)
    {
        return verify_input;
    }
    
    // Checks if players already exist in the system
    ChessResult player_create_result = chessAddGameCreatePlayersIfNeeded(
                                        chess, first_player, second_player);
    if (player_create_result != CHESS_SUCCESS)
    {
        return player_create_result;
    }

    // Get the players' structs
    Player first_player_struct = mapGet(chess->players, &first_player);
    Player second_player_struct = mapGet(chess->players, &second_player);
    if (first_player_struct == NULL || second_player_struct == NULL)
    {
        return CHESS_OUT_OF_MEMORY; // Shouldn't get here
    }

    // Handling cases of a player never played in the tournament before
    int amount_of_new_players = 0;
    Tournament tournament = mapGet(chess->tournaments, &tournament_id);
    int max_games_per_player = tournamentGetMaxGamesPerPlayer(tournament);

    // Creating new PlayerInTournaments for the players if needed
    chessAddGameCreatePlayerInTournamentsIfNeeded(tournament_id, first_player_struct,
                        second_player_struct, max_games_per_player, &amount_of_new_players);


    if (isGameBetweenPlayersExists(chess, tournament_id, first_player, second_player))
    {
        return CHESS_GAME_ALREADY_EXISTS;
    }

    // Check play time
    if (play_time < 0)
    {
        return CHESS_INVALID_PLAY_TIME;
    }

    // EXCEEDED GAMES
    if (isPlayerInTournamentExceededGames(chess, first_player, tournament_id) ||
        isPlayerInTournamentExceededGames(chess, second_player, tournament_id))
    {
        return CHESS_EXCEEDED_GAMES;
    }

    ChessResult add_game_result = chessAddGameTournamentAndPlayer(tournament, first_player_struct,
                            second_player_struct, winner, play_time,
                            amount_of_new_players, max_games_per_player);

    return add_game_result;
}



ChessResult chessRemoveTournament (ChessSystem chess, int tournament_id)
{
    if (chess == NULL)
    {
        return CHESS_NULL_ARGUMENT;
    }

    if (tournament_id <= 0)
    {
        return CHESS_INVALID_ID;
    }

    if (!(mapContains(chess->tournaments, &tournament_id)))
    {
        return CHESS_TOURNAMENT_NOT_EXIST;
    }

    // Remove the tournament 
    mapRemove(chess->tournaments, &tournament_id);

    // Remove tournament records and stats from players
    //////////Player player_iterator = mapGetFirst(chess->players);
    int  *player_iterator = mapGetFirst(chess->players);
    while (player_iterator != NULL)
    {
        Player player = mapGet(chess->players, player_iterator);
        if (playerIsPlayingInTournament(player, tournament_id))
        {
            playerRemoveTournament(player, tournament_id);
        }
        player_iterator = mapGetNext(chess->players);
    }

    return CHESS_SUCCESS;
}

ChessResult chessRemovePlayer(ChessSystem chess, int player_id)
{
    // Input verification
    ChessResult input_verification = chessRemovePlayerVerifyInput(chess, player_id);
    if (input_verification != CHESS_SUCCESS)
    {
        return input_verification;
    }

    // Get the player, initialize iterator for his tournaments
    Player player = mapGet(chess->players, &player_id);
    int *tournament_id_ptr = playerGetFirstTournamentID(player);
    
    // Scan the tournaments that the player participated in
    while (tournament_id_ptr != NULL)
    {
        int tournament_id = *tournament_id_ptr;
        free(tournament_id_ptr);
        tournament_id_ptr = playerGetNextTournamentID(player);
        Tournament tournament = mapGet(chess->tournaments, &tournament_id);

        // Tournament ended, advance to the next one
        if (tournamentGetWinner(tournament) != INVALID_PLAYER)
        {
            continue;
        }

        // Tournament active - scan games and update stats
        int *game_ids = playerGetGameIdsInTournament(player, tournament_id);
        int max_games_per_player = tournamentGetMaxGamesPerPlayer(tournament);

        // Scan games
        for (int i = 0 ; i < max_games_per_player ; i++)
        {
            if (game_ids[i] == INVALID_GAME_ID)
            {
                break;
            }
            
            // Update the opponent's stats 
            chessRemovePlayerUpdateGameResult(chess, tournament_id, game_ids[i], player_id);
        }

        // Remove player from tournament - updates all game records
        tournamentRemovePlayer(tournament, player_id, game_ids);
    }
    
    return CHESS_SUCCESS;
}

ChessResult chessEndTournament (ChessSystem chess, int tournament_id)
{
    if (chess == NULL)
    {
        return CHESS_NULL_ARGUMENT;
    }

    if (tournament_id <= 0)
    {
        return CHESS_INVALID_ID;
    }

    Tournament tournament = mapGet(chess->tournaments, &tournament_id);
    if (tournament == NULL)
    {
        return CHESS_TOURNAMENT_NOT_EXIST;
    }

    if (tournamentGetWinner(tournament) != INVALID_PLAYER)
    {
        return CHESS_TOURNAMENT_ENDED;
    }

    if (tournamentGetSizeGames(tournament) == 0)
    {
        return CHESS_NO_GAMES;
    }
    
    int tournament_winner = ChessTournamentCalculateWinner(chess, tournament_id);
    return tournamentEnd(tournament, tournament_winner);

}

double chessCalculateAveragePlayTime (ChessSystem chess, int player_id, ChessResult* chess_result)
{
    if (chess == NULL)
    {
        *chess_result = CHESS_NULL_ARGUMENT;
        return -1;
    }

    if (player_id <= 0)
    {
        *chess_result = CHESS_INVALID_ID;
        return -1;
    }

    Player player = mapGet(chess->players, &player_id);
    if (player == NULL)
    {
        *chess_result = CHESS_PLAYER_NOT_EXIST;
        return -1;
    }
    *chess_result = CHESS_SUCCESS;
    return playerGetFinishedGamesAverageTime(player);
    
}

//ChessResult chessSavePlayersLevels (ChessSystem chess, FILE* file)


ChessResult chessSaveTournamentStatistics (ChessSystem chess, char* path_file)
{
    if (chess == NULL)
    {
        return CHESS_NULL_ARGUMENT;
    }

    bool is_tournament_ended = false;
    int *tournament_id_iterator = mapGetFirst(chess->tournaments);
    FILE *output_file = fopen(path_file, "w+");

    while (tournament_id_iterator != NULL)
    {
        Tournament tournament = mapGet(chess->tournaments, tournament_id_iterator);
        if (tournamentGetWinner(tournament) == INVALID_PLAYER)
        {
            tournament_id_iterator = mapGetNext(chess->tournaments);
            continue;
        }

        is_tournament_ended = true;
        bool print_result = tournamentPrintStatsToFile(tournament, output_file);
        if (print_result == false)
        {
            return CHESS_SAVE_FAILURE;
        }
        tournament_id_iterator = mapGetNext(chess->tournaments);
    }

    if (!is_tournament_ended)
    {
        fclose(output_file);
        return CHESS_NO_TOURNAMENTS_ENDED;
    }

    // SAVE FAILURE???????????
    return CHESS_SUCCESS;
}




