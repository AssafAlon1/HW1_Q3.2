#ifndef _PLAYER_H
#define _PLAYER_H

#define INVALID_PLAYER -1
#define DELETED_PLAYER -2

#define LOSE_TO_WIN 11
#define DRAW_TO_WIN 12

#define WIN_WEIGHT 6
#define LOSS_WEIGHT -10
#define DRAW_WEIGHT 2

#include <stdio.h>
#include "chessSystem.h"
#include "game.h"
#include "map.h"

typedef enum {
    PLAYER_OUT_OF_MEMORY,
    PLAYER_NULL_ARGUMENT,
    PLAYER_INVALID_ID,
    PLAYER_TOURNAMENT_NOT_EXIST,
    PLAYER_NOT_IN_GAME,
    PLAYER_GAME_ALREADY_EXISTS,
    PLAYER_EXCEEDED_GAMES,
    PLAYER_GENERAL_ERROR,
    PLAYER_SUCCESS
} PlayerResult ;


typedef struct player_t *Player;


/**
 * playerCreate: create a new player.
 *
 * @param player_id - the id of the player
 * 
 * @return A new player in case of success, and NULL otherwise (e.g.
 *     in case of an allocation error)
 */
Player playerCreate(int player_id);



/**
 * playerDestroy: free a player, and all its contents, from
 * memory.
 *
 * @param player - the player to free from memory. A NULL value is
 *     allowed, and in that case the function does nothing.
 */
void playerDestroy(Player player);


/**
 * playerCopy: copies a new player.
 *
 * @param player - the player
 * @return The coppied player in case of success, and NULL otherwise (e.g.
 *     in case of an allocation error)
 */
Player playerCopy(Player player);


/**
 * playerAddGame: register a game that the player has played
 *
 * @param player - the player that has played said game
 * @param game   - the game that the player has played
 * @param max_games_per_player - the max games a player get play in the tournament
 *                               in which the game was played
 * @return
 *      PLAYER_NULL_ARGUMENT - if player is NULL or game is NULL
 *      PLAYER_OUT_OF_MEMORY - if an allocation failed
 *      PLAYER_SUCCESS       - in the case of success
 */
PlayerResult playerAddGame(Player player, Game game, int max_games_per_player);



/**
 * playerGetTotalGames: Returns the amount of games the player has played
 *
 * @param player - the player
 * @return
 *      The amount of games the player has played
 */
int playerGetTotalGames(Player player);


/**
 * playerGetFinishedGamesAverageTime: Returns the average time
 * of a game the player has played
 *
 * @param player - the player
 * @return
 *      The average time of a game the player has played
 */
double playerGetFinishedGamesAverageTime(Player player);



/**
 * playerGetLevel: Calculates and returns the level of a given player
 *
 * @param player - the player
 * @return
 *      The level of said player
 */
double playerGetLevel(Player player);


/**
 * playerRemoveTournament: Removes tournament records from player and update statistics
 *
 * @param player - the player
 * @param tournament_id - the id of the tournament to remove
 * @return
 *      PLAYER_NULL_ARGUMENT - if player is NULL
 *      PLAYER_TOURNAMENT_NOT_EXIST - if the player never played in the tournament
 *      PLAYER_SUCCESS - in the case of success
 */
PlayerResult playerRemoveTournament(Player player, int tournament_id);


/**
 * playerGetID: Returns the ID of a given player
 *
 * @param player - the player
 * @return
 *      The ID of said player
 */
int playerGetID(Player player);


/**
 * playerIsPlayingInTournament: Returns whether a player plays in a tournament
 *
 * @param player - the player
 * @param tournament_id - the id of the tournamnet
 * @return
 *      true - if the player plays in the tournament
 *      false - if the player doesn't plays in the tournament
 */
bool playerIsPlayingInTournament(Player player, int tournament_id);



/**
 * playerGetGameIdsInTournament: Returns the array of game IDs the player
 *                               played in the tournament
 *
 * @param player - the player
 * @param tournament_id - the id of the tournamnet
 * @return
 *      the array of game IDs the player played in the tournament
 */
int* playerGetGameIdsInTournament(Player player, int tournament_id);


/**
 * playerCanPlayMoreGamesInTournament: Returns whether a player can play more
 *                                     games in a given tournament
 *
 * @param player - the player
 * @param tournament_id - the id of the tournamnet
 * @return
 *      true - if the player can play another game
 *      false - if the player can't play another game
 */
bool playerCanPlayMoreGamesInTournament(Player player, int tournament_id);


/**
 * playerAddTournament: Add a tournament record to a given player
 *
 * @param player - the player
 * @param tournament_id - the id of the tournamnet
 * @param max_games_per_player - the maximum amount of games allowed to play in the tournament
 * @return
 *      PLAYER_NULL_ARGUMENT - if player is NULL
 *      PLAYER_OUT_OF_MEMORY - in the case of memory issues
 *      PLAYER_GENERAL_ERROR - in the case of unexpected error
 *      PLAYER_SUCCESS       - if the tournament was added successfully
 */
PlayerResult playerAddTournament(Player player, int tournament_id, int max_games_per_player);


/**
 * playerGetPlayerInTournaments: Returns a map of the player in tournament records
 *
 * @param player - the player
 * @return
 *      a pointer to a map that contains the player in tournament records
 */
Map* playerGetPlayerInTournaments(Player player);


/**
 * playerCanPlayMoreGamesInTournament: Returns whether a player can play more
 *                                     games in a given tournament
 *
 * @param player - the player
 * @param tournament_id - the id of the tournamnet
 * @return
 *      true - if the operation completed successfully
 *      false - if the operation completed unsuccessfully
 */
bool playerUpdateResultsAfterOpponentDeletion(Player player, int tournament_id, int outcome_change);


/**
*	playerGetFirstTournamentID: Sets the internal iterator to the the first tournament
                              and return it
*
* @param player - The player that played the tournaments
* @return
* 	NULL if a NULL pointer was sent or the map is empty or allocation fails
* 	The first key tournament of the map otherwise
*/
int *playerGetFirstTournamentID(Player player);

/**
*	playerGetNextTournamentID: Advances the iterator to the next tournament and
                               returns the ID of it
* @param player - The player that played the tournaments
* @return
* 	NULL if reached the end of the map, or the iterator is at an invalid state
* 	,or a NULL sent as argument or allocation fails
* 	The next tournament ID on the map in case of success
*/
int *playerGetNextTournamentID(Player player);


int playerGetWinsInTournament(Player player, int tournament_id);

int playerGetDrawsInTournament(Player player, int tournament_id);

int playerGetLossesInTournament(Player player, int tournament_id);

#endif


