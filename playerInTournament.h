#ifndef _PLAYER_IN_TOURNAMENT_H
#define _PLAYER_IN_TOURNAMENT_H

#include <stdio.h>
#include "game.h"

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
 *     The coppied player in tournament
 */
PlayerInTournament playerInTournamentCopy(PlayerInTournament player_in_tournament);


/**
 * playerInTournamentAddGame: Adds a game record to a player in a tournament
 *
 * @param player_in_tournament - the player in tournament that will contain the new game
 * @param game - the game that the player has played in the tournament
 * @return
 *     The coppied player in tournament
 *     CHESS_NULL_ARGUMENT - if player_in_tournament or game is NULL
       CHESS_GAME_ALREADY_EXISTS - if there's already a game with the same opponent
       CHESS_EXCEEDED_GAMES - if the player has reached his max allowed games in the tournament
       CHESS_PLAYER_NOT_EXIST - if the player is not one of the players in game
       CHESS_SUCCESS - if the action was performed successfully
 */
ChessResult playerInTournamentAddGame(PlayerInTournament player_in_tournament, Game game);



/**
 * playerInTournamentGetWins: Return the number of wins the player had in the tournament
 *
 * @param player_in_tournament - the relevant player in tournament.
 *
 * @return
 *     The number wins of the player in the tournament
 */
int playerInTournamentGetWins (PlayerInTournament player_in_tournament);


/**
 * playerInTournamentGetDraws: Return the number of draws the player had in the tournament
 *
 * @param player_in_tournament - the relevant player in tournament.
 *
 * @return
 *     The number draws of the player in the tournament
 */
int playerInTournamentGetDraws (PlayerInTournament player_in_tournament);


/**
 * playerInTournamentGetLosses: Return the number of losses the player had in the tournament
 *
 * @param player_in_tournament - the relevant player in tournament.
 *
 * @return
 *     The number losses of the player in the tournament
 */
int playerInTournamentGetLosses (PlayerInTournament player_in_tournament);


/**
 * playerInTournamentGetTotalTime: Return the total time in seconds the player has played
 *
 * @param player_in_tournament - the relevant player in tournament.
 *
 * @return
 *     the total time in seconds the player has played
 */
int playerInTournamentGetTotalTime (PlayerInTournament player_in_tournament);


/**
 * playerInTournamentGetTotalGames: Return the amount of games player has played
 *
 * @param player_in_tournament - the relevant player in tournament.
 *
 * @return
 *     the amount of games player has played
 */
int playerInTournamentGetTotalGames (PlayerInTournament player_in_tournament);


#endif