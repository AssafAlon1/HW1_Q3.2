#ifndef _PLAYER_IN_TOURNAMENT_H
#define _PLAYER_IN_TOURNAMENT_H

#include <stdio.h>
#include "game.h"

#define PLAYER_IN_TOURNAMENT_INVALID_INPUT -1

typedef enum {
    PLAYER_IN_TOURNAMENT_OUT_OF_MEMORY,
    PLAYER_IN_TOURNAMENT_NULL_ARGUMENT,
    PLAYER_IN_TOURNAMENT_INVALID_ID,
    PLAYER_IN_TOURNAMENT_INVALID_MAX_GAMES,
    PLAYER_IN_TOURNAMENT_GAME_ALREADY_EXISTS,
    PLAYER_IN_TOURNAMENT_EXCEEDED_GAMES,
    PLAYER_IN_TOURNAMENT_PLAYER_NOT_IN_GAME,
    PLAYER_IN_TOURNAMENT_CONFLICT_ID,
    PLAYER_IN_TOURNAMENT_SUCCESS
} PlayerInTournamentResult ;


/** Type for representing a player's games in a specific tournament */
typedef struct player_in_tournament_t *PlayerInTournament;

/**
 * playerInTournamentCreate: create an empty player in tournament.
 *
 * @param player_id - the id of the player
 * @param tournament_id - the id of the tournament
 * @param max_games_per_player - the max games per player
 * 
 * @return A new PlayerInTournament in case of success, and NULL otherwise (e.g.
 *     in case of an allocation error)
 */
PlayerInTournament playerInTournamentCreate(int player_id, int tournament_id, int max_games_per_player);


/**
 * playerInTournamentDestroy: free a player in tournament, and all its contents, from
 * memory.
 *
 * @param player_in_tournament - the PlayerInTournament to free from memory. A NULL value is
 *     allowed, and in that case the function does nothing.
 */
void playerInTournamentDestroy(PlayerInTournament player_in_tournament);


/**
 * playerInTournamentCopy: copies a given player in tournament
 *
 * @param player_in_tournament - the player in tournament to copy
 * @return
 *     The coppied player in tournament if the function Succeeded
 *     if the allocation failed the function return NULL
 */
PlayerInTournament playerInTournamentCopy(PlayerInTournament player_in_tournament);


/**
 * playerInTournamentAddGame: Adds a game record to a player in a tournament
 *
 * @param player_in_tournament - the player in tournament that will contain the new game
 * @param game - the game that the player has played in the tournament
 * @return
 *     The coppied player in tournament
 *     PLAYER_IN_TOURNAMENT_NULL_ARGUMENT - if player_in_tournament or game is NULL
       PLAYER_IN_TOURNAMENT_GAME_ALREADY_EXISTS - if there's already a game with the same opponent
       PLAYER_IN_TOURNAMENT_EXCEEDED_GAMES - if the player has reached his max allowed games in the tournament
       PLAYER_IN_TOURNAMENT_PLAYER_NOT_IN_GAME - if the player is not one of the players in game
       PLAYER_IN_TOURNAMENT_SUCCESS - if the action was performed successfully
 */
PlayerInTournamentResult playerInTournamentAddGame(PlayerInTournament player_in_tournament, Game game);



/**
 * playerInTournamentGetWins: Return the number of wins the player had in the tournament
 *
 * @param player_in_tournament - the relevant player in tournament.
 *
 * @return
 *     The number wins of the player in the tournament
 *     PLAYER_IN_TOURNAMENT_INVALID_INPUT - if the input is not valid
 */
int playerInTournamentGetWins (PlayerInTournament player_in_tournament);


/**
 * playerInTournamentGetDraws: Return the number of draws the player had in the tournament
 *
 * @param player_in_tournament - the relevant player in tournament.
 *
 * @return
 *     The number draws of the player in the tournament
 *     PLAYER_IN_TOURNAMENT_INVALID_INPUT - if the input is not valid
 */
int playerInTournamentGetDraws (PlayerInTournament player_in_tournament);


/**
 * playerInTournamentGetLosses: Return the number of losses the player had in the tournament
 *
 * @param player_in_tournament - the relevant player in tournament.
 *
 * @return
 *     The number losses of the player in the tournament
 *     PLAYER_IN_TOURNAMENT_INVALID_INPUT - if the input is not valid
 */
int playerInTournamentGetLosses (PlayerInTournament player_in_tournament);


/**
 * playerInTournamentGetTotalTime: Return the total time in seconds the player has played
 *
 * @param player_in_tournament - the relevant player in tournament.
 *
 * @return
 *     the total time in seconds the player has played
 *     PLAYER_IN_TOURNAMENT_INVALID_INPUT - if the input is not valid
 */
int playerInTournamentGetTotalTime (PlayerInTournament player_in_tournament);


/**
 * playerInTournamentGetTotalGames: Return the amount of games player has played
 *
 * @param player_in_tournament - the relevant player in tournament.
 *
 * @return
 *     the amount of games player has played
 *     PLAYER_IN_TOURNAMENT_INVALID_INPUT - if the input is not valid
 */
int playerInTournamentGetTotalGames (PlayerInTournament player_in_tournament);


/**
 * playerInTournamentGetGameIds: Return the array of game IDs
 *
 * @param player_in_tournament - the relevant player in tournament.
 *
 * @return
 *     the game ID array of said player in tournament
 *     NULL - if the player in tournament is NULL
 */
int *playerInTournamentGetGameIds(PlayerInTournament player_in_tournament);


/**
 * playerInTournamentCanPlayMore: Return whether a player can play more games in the tournament
 *
 * @param player_in_tournament - the relevant player in tournament.
 *
 * @return
 *     true - if the player can play more games in the tournament
 *     false - if the player cannot play more games in the tournament 
 *              or the player in tournament is NULL
 */
bool playerInTournamentCanPlayMore(PlayerInTournament player_in_tournament);


/**
 * playerInTournamentGetTournamentID: Return the ID of the relevant tournament 
 *
 * @param player_in_tournament - the relevant player in tournament.
 *
 * @return
 *     the ID of the tournament in which the player plays
 *     PLAYER_IN_TOURNAMENT_INVALID_INPUT - if the input is not valid
 */
int playerInTournamentGetTournamentID(PlayerInTournament player_in_tournament);


/**
 * playerInTournamentUpdateDrawToWin: Removes a draw from the record and adds a win 
 *
 * @param player_in_tournament - the relevant player in tournament.
 *
 * @return
 *     true - if the operation finished successfully
 *     false - if player_in_tournament is NULL / there are no draws in record
 */
bool playerInTournamentUpdateDrawToWin(PlayerInTournament player_in_tournament);


/**
 * playerInTournamentUpdateLossToWin: Removes a loss from the record and adds a win 
 *
 * @param player_in_tournament - the relevant player in tournament.
 *
 * @return
 *     true - if the operation finished successfully
 *     false - if player_in_tournament is NULL / there are no losses in record
 */
bool playerInTournamentUpdateLossToWin(PlayerInTournament player_in_tournament);

#endif