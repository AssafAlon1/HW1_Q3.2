#ifndef _TOURNAMENT_H
#define _TOURNAMENT_H

#include <stdio.h>
#include "chessSystem.h"


/** Type for representing a tournament that will be managed by a Chess System */
typedef struct tournament_t *Tournament;


/**
 * tournamentCreate: create an empty tournament.
 *
 * @return A new tournament in case of success, and NULL otherwise (e.g.
 *     in case of an allocation error)
 */
Tournament tournamentCreate();


/**
 * tournamentDestroy: free a tournament, and all its contents, from
 * memory.
 *
 * @param tournament - the tournament to free from memory. A NULL value is
 *     allowed, and in that case the function does nothing.
 */
void tournamentDestroy(Tournament tournament);



/**
 * tournamentAddGame: add a new match to a tournament.
 *
 * @param tournament - tournament system that contains the tournament. Must be non-NULL.
 * @param first_player - first player id. Must be non-negative.
 * @param second_player - second player id. Must be non-negative.
 * @param winner - indicates the winner in the match. if it is FIRST_PLAYER, then the first player won.
 *                 if it is SECOND_PLAYER, then the second player won, otherwise the match has ended with a draw.
 * @param play_time - duration of the match in seconds. Must be non-negative.
 * 
 * @return
 *     CHESS_NULL_ARGUMENT - if tournament is NULL.
 *     CHESS_INVALID_ID - if either the players or the winner is invalid.
 *     CHESS_GAME_ALREADY_EXIST - if there is already a game in the tournament with the same two players.
 *     CHESS_INVALID_PLAY_TIME - if the play time is negative.
 *     CHESS_SUCCESS - if game was added successfully.
 */
ChessResult chessAddGame(Tournament tournament, int first_player,
                         int second_player, Winner winner, int play_time);





/**
 * tournamentAddGame: add a new match to a chess tournament.
 *
 * @param tournament - tournament that will host the game. Must be non-NULL.
 * @param first_player - first player id. Must be non-negative.
 * @param second_player - second player id. Must be non-negative.
 * @param winner - indicates the winner in the match. if it is FIRST_PLAYER, then the first player won.
 *                 if it is SECOND_PLAYER, then the second player won, otherwise the match has ended with a draw.
 * @param play_time - duration of the match in seconds. Must be non-negative.
 *
 * @return
 *     CHESS_NULL_ARGUMENT - if tournament is NULL.
 *     CHESS_INVALID_ID - if either the players or the winner is invalid.
 *     CHESS_GAME_ALREADY_EXIST - if there is already a game in the tournament with the same two players.
 *     CHESS_INVALID_PLAY_TIME - if the play time is negative.
 *     CHESS_SUCCESS - if game was added successfully.
 */
ChessResult chessAddGame(Tournament, int first_player,
                         int second_player, Winner winner, int play_time);





/**
 * tournamentRemovePlayer: removes the player from the tournament.
 *                      In games where the player has participated and not yet ended,
 *                      the opponent is the winner automatically after removal.
 *
 * @param tournament - The tournament that should remove specified player. Must be non-NULL.
 * @param player_id - the player id. Must be non-negative.
 *
 * @return
 *     CHESS_NULL_ARGUMENT - if tournament is NULL.
 *     CHESS_INVALID_ID - if the player ID number is invalid.
 *     CHESS_PLAYER_NOT_EXIST - if the player does not play in the tournament.
 *     CHESS_SUCCESS - if player was removed successfully.
 */
ChessResult tournamentRemovePlayer(Tournament tournament, int player_id);



/**
 * tournamentEnd: The function will end the tournament and calculate the id of the winner.
 *                     The winner of the tournament is the player with the highest score.
 *                     Once the tournament is over, no games can be added for that tournament.
 *
 * @param tournament - the tournament id. Must be non-negative, and unique.
 *
 * @return
 *     CHESS_NULL_ARGUMENT - if Tournament is NULL.
 *     CHESS_SUCCESS - if tournament was ended successfully.
 */
ChessResult tournamentEnd (Tournament tournament);


#endif