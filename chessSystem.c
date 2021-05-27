#include <stdio.h>
#include <stdlib.h>

#include "chessSystem.h"
#include "./mtm_map/map.h"
#include "mapUtil.h"
#include "tournament.h"
#include "game.h"
#include "player.h"
#include "playerInTournament.h"

#define CHESS_INVALID_INPUT -10
#define PLAYER_PLAYS_NO_GAMES_LVL -11

struct chess_system_t {
    Map tournaments;
    Map players;
};

//==============================================================//
//================== INTERNAL FUNCTIONS START ==================//
//==============================================================//


// Translate error code from tournament to chess
static ChessResult translateTournamentResultToChessResult(TournamentResult result)
{
    switch (result)
    {
        case TOURNAMENT_OUT_OF_MEMORY:
            return CHESS_OUT_OF_MEMORY;
        case TOURNAMENT_NULL_ARGUMENT:
            return CHESS_NULL_ARGUMENT;
        case TOURNAMENT_INVALID_ID:
            return CHESS_INVALID_ID;
        case TOURNAMENT_INVALID_LOCATION:
            return CHESS_INVALID_LOCATION;
        case TOURNAMENT_INVALID_MAX_GAMES:
            return CHESS_INVALID_MAX_GAMES;
        case TOURNAMENT_GAME_ALREADY_EXISTS:
            return CHESS_GAME_ALREADY_EXISTS;
        case TOURNAMENT_INVALID_PLAY_TIME:
            return CHESS_INVALID_PLAY_TIME;
        case TOURNAMENT_EXCEEDED_GAMES:
            return CHESS_EXCEEDED_GAMES;
        case TOURNAMENT_NO_GAMES:
            return CHESS_NO_GAMES;
        case TOURNAMENT_ENDED:
            return CHESS_TOURNAMENT_ENDED;
        case TOURNAMENT_SUCCESS:
            return CHESS_SUCCESS;
        default: // Shouldn't get here
            return CHESS_SUCCESS;
    }
}


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


// Checks if 2 players play against each other in a given tournament
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
    
    // Get id's of first_player's games in the tournament
    int *game_ids = playerGetGameIdsInTournament(first_player, tournament_id);
    if (game_ids == NULL)
    {
        return false;
    }

    // Scan game ids, check if second_player plays one of them
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


// Checks if a given player is about to  exceed the maximum amount of game in a tournament
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
    // Creating first_player if needed
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

    // Creating second_player if needed
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
            // Removing the first player if the operation failed
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
                            int amount_of_new_players)
{
    TournamentResult new_game_result =  tournamentAddGame(tournament, playerGetID(first_player_struct),
                        playerGetID(second_player_struct), winner, play_time, amount_of_new_players);
    
    if (new_game_result != TOURNAMENT_SUCCESS)
    {
        return translateTournamentResultToChessResult(new_game_result);
    }
    
    // Creates new game with its proper id
    int new_game_id = tournamentGetSizeGames(tournament) - 1;
    Game new_game = tournamentGetGame(tournament, new_game_id);

    // Add the game to the first player
    PlayerResult add_game_result = playerAddGame(first_player_struct, new_game);
    if (add_game_result != PLAYER_SUCCESS)
    {
        return CHESS_OUT_OF_MEMORY;
    }

    // Add game to 2nd player. In case of failure, remove the game from first_player
    add_game_result = playerAddGame(second_player_struct, new_game);
    if (add_game_result != PLAYER_SUCCESS)
    {
        playerRemoveLastGame(first_player_struct, new_game);
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
    Game current_game     = tournamentGetGame(tournament, game_id);
    int opponent_id       = gameGetPlayersOpponent(current_game, player_id);
    Player opponent       = mapGet(chess->players, &opponent_id);
    int game_winner       = gameGetIdOfWinner(current_game);
    if (game_winner  == opponent_id)
    {
        return;
    }
    if (game_winner == INVALID_PLAYER)
    {
        playerUpdateResultsAfterOpponentDeletion(opponent,tournament_id , DRAW_TO_WIN);
        return;
    }

    playerUpdateResultsAfterOpponentDeletion(opponent, tournament_id, LOSS_TO_WIN);
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
        return CHESS_INVALID_ID;
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
        return CHESS_INVALID_INPUT;
    }
    
    Player player = mapGet(chess->players, &player_id);
    if (player == NULL)
    {
        return CHESS_INVALID_INPUT;
    }

    int *game_ids = playerGetGameIdsInTournament(player, tournament_id);
    if (game_ids == NULL)
    {
        return CHESS_INVALID_INPUT;
    }

    Tournament tournament = mapGet(chess->tournaments, &tournament_id);
    if (tournament == NULL)
    {
        return CHESS_INVALID_INPUT;
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
    
    return winner_id < player_id ? winner_id : player_id;
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

    int winner_id     = *player_iterator;
    int winner_score  = chessCalculatePlayerScore(chess, winner_id, tournament_id);
    while (player_iterator)
    {
        bool is_playing = isPlayerPlayingInTournament(chess, *player_iterator, tournament_id);
        if (!is_playing)
        {
            free(player_iterator);
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
        free(player_iterator);
        player_iterator = mapGetNext(chess->players);
    }

    return winner_id;
}

//============================================================//
//================== INTERNAL FUNCTIONS END ==================//
//============================================================//

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
    if (chess == NULL || tournament_location == NULL)
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

    ChessResult add_game_result = chessAddGameTournamentAndPlayer(tournament,
                            first_player_struct, second_player_struct,
                            winner, play_time, amount_of_new_players);

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
    int  *player_iterator = mapGetFirst(chess->players);
    while (player_iterator != NULL)
    {
        Player player = mapGet(chess->players, player_iterator);
        if (playerIsPlayingInTournament(player, tournament_id))
        {
            playerRemoveTournament(player, tournament_id);
        }
        free(player_iterator);
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
    
    mapRemove(chess->players, &player_id);
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
    return translateTournamentResultToChessResult(tournamentEnd(tournament, tournament_winner));

}

double chessCalculateAveragePlayTime (ChessSystem chess, int player_id, ChessResult* chess_result)
{
    if (chess_result == NULL)
    {
        return CHESS_INVALID_INPUT;
    }

    if (chess == NULL)
    {
        *chess_result = CHESS_NULL_ARGUMENT;
        return CHESS_INVALID_INPUT;
    }

    if (player_id <= 0)
    {
        *chess_result = CHESS_INVALID_ID;
        return CHESS_INVALID_INPUT;
    }

    Player player = mapGet(chess->players, &player_id);
    if (player == NULL)
    {
        *chess_result = CHESS_PLAYER_NOT_EXIST;
        return CHESS_INVALID_INPUT;
    }
    *chess_result = CHESS_SUCCESS;
    return playerGetFinishedGamesAverageTime(player);
    
}


// Given a chess system, 2 arrays and their lengths, the function will get player
// information (id & level) and write it to the given arrays
static ChessResult buildPlayerIdAndLevelArrays(ChessSystem chess, int amount_of_players,
                                                 int* player_id, double* player_level)
{
    if (chess == NULL || player_id == NULL || player_level == NULL)
    {
        return CHESS_NULL_ARGUMENT;
    }

    int *player_iterator = mapGetFirst(chess->players);
    int current_index = 0;
    while (player_iterator)
    {
        if (playerGetTotalGames(mapGet(chess->players, player_iterator)) == 0)
        {
            player_id[current_index]    = INVALID_PLAYER;
            player_level[current_index] = PLAYER_PLAYS_NO_GAMES_LVL;
            free(player_iterator);
            player_iterator = mapGetNext(chess->players);
            current_index++;
            continue;
        }
        player_id[current_index]    = *player_iterator;
        player_level[current_index] = playerGetLevel(mapGet(chess->players, player_iterator));
        free(player_iterator);
        player_iterator             = mapGetNext(chess->players);
        current_index++;
    }
    
    return CHESS_SUCCESS;
}


// compares two doubles
static int chessDoubleCompare (double num1, double num2)
{
    if (num1 - num2 < 0.00001 && num2 - num1 < 0.00001)
    {
        return 0;
    }
    if (num1 > num2)
    {
        return 1;
    }

    return -1;
}


// Sorts 2 arrays, one of them being sorted in a regular fashion, the other one being
// sorted according to the first one (values from same cells in 2 arrays are linked)
static void sortLinkedArrays (double referenced_array[], int second_array[], int array_size)
{
    for (int i = 0 ; i < array_size ; i++)
    {
        for (int j = 0 ; j < array_size - i - 1 ; j++)
        {
            int comparasion = chessDoubleCompare(referenced_array[j], referenced_array[j+1]);
            if (comparasion < 0 || (comparasion == 0 && second_array[j] > second_array[j+1]))
            {
                double temp_double   = referenced_array[j];
                referenced_array[j]   = referenced_array[j+1];
                referenced_array[j+1] = temp_double;

                int temp_int = second_array[j];
                second_array[j]   = second_array[j+1];
                second_array[j+1] = temp_int;
            }
        }
    }
}

// static void PrintHelpPlease (double lvl[], int id[], int size)
// {
//     for (int i = 0 ; i < size ; i++)
//     {
//         printf("ID: %d,  LVL: %.2f\n", id[i], lvl[i]);
//     }
//     printf("\n\n");
// }

ChessResult chessSavePlayersLevels (ChessSystem chess, FILE* file)
{
    if (chess == NULL)
    {
        return CHESS_NULL_ARGUMENT;
    }
    
    int amount_of_players = mapGetSize(chess->players);
    int *player_id_array    = malloc(amount_of_players*sizeof(int));
    if (player_id_array == NULL)
    {
        return CHESS_OUT_OF_MEMORY;
    }
    double *player_level_array = malloc(amount_of_players*sizeof(double));
    if (player_level_array == NULL)
    {
        free(player_id_array);
        return CHESS_OUT_OF_MEMORY;
    }

    buildPlayerIdAndLevelArrays(chess, amount_of_players, player_id_array, player_level_array);
    sortLinkedArrays(player_level_array, player_id_array, amount_of_players);

    for (int i = 0 ; i < amount_of_players ; i++)
    {
        if (player_id_array[i] < 0)
        {
            break;
        }
        int result = fprintf(file, "%d %.2f\n", player_id_array[i], player_level_array[i]);
        if (result < 0)
        {
            free(player_level_array);
            free(player_id_array);
            return CHESS_SAVE_FAILURE;
        }
    }

    free(player_level_array);
    free(player_id_array);

    return CHESS_SUCCESS;
}


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
            free(tournament_id_iterator);
            tournament_id_iterator = mapGetNext(chess->tournaments);
            continue;
        }

        is_tournament_ended = true;
        bool print_result = tournamentPrintStatsToFile(tournament, output_file);
        if (print_result == false)
        {
            fclose(output_file);
            free(tournament_id_iterator);
            return CHESS_SAVE_FAILURE;
        }
        free(tournament_id_iterator);
        tournament_id_iterator = mapGetNext(chess->tournaments);
    }

    fclose(output_file);
    if (!is_tournament_ended)
    {
        return CHESS_NO_TOURNAMENTS_ENDED;
    }

    return CHESS_SUCCESS;
}




