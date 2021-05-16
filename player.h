#ifndef _PLAYER_H
#define _PLAYER_H

#include <stdio.h>
#include "chessSystem.h"

// typedef enum {
//     PLAYER_OUT_OF_MEMORY,
//     PLAYER_NULL_ARGUMENT,
//     PLAYER_INVALID_ID,
//     PLAYER_SUCCESS
// } PlayerResult ;


typedef struct player_t *Player;


/**
 * playerCreate: create a new player.
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
 * playerAddGame: register a game that the player has played
 *
 * @param player - the player that has played said game
 * @param game   - the game that the player has played
 * @return
 *      CHESS_NULL_ARGUMENT - if player is NULL or game is NULL
 *      CHESS_OUT_OF_MEMORY - if an allocation failed
 */
ChessResult playerAddGame(Player player, Game game);

#endif