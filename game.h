#ifndef _GAME_H
#define _GAME_H

#include <stdio.h>
#include "chessSystem.h"


typedef struct game_t *Game;



/**
 * gameCreate: create a new game.
 *
 * @return A new game in case of success, and NULL otherwise (e.g.
 *     in case of an allocation error)
 */
Game gameCreate(int first_player, int second_player, Winner winner, int play_time);



/**
 * gameDestroy: free a game, and all its contents, from
 * memory.
 *
 * @param game - the game to free from memory. A NULL value is
 *     allowed, and in that case the function does nothing.
 */
void gameDestroy(Game game);


#endif