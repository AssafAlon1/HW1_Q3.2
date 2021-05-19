#ifndef _GAME_H
#define _GAME_H

#define INVALID_GAME_ID -1

#include <stdio.h>
#include <stdbool.h>
#include "chessSystem.h"


typedef struct game_t *Game;


/**
 * gameCreate: create a new game.
 * 
 * @param tournament_id - the id of the tournament in which the game is being held
 * @param first_player - the id of the first player
 * @param second_player - the id of the second player
 * @param Winner - the winner of the game
 * @param play_time - the length of the game in seconds
 * 
 * @return A new game in case of success, and NULL otherwise (e.g.
 *     in case of an allocation error)
 */
Game gameCreate(int tournament_id, int first_player, int second_player, Winner winner, int play_time, int game_id);



/**
 * gameDestroy: free a game, and all its contents, from
 * memory.
 * 
 * @param game - the game to free from memory. A NULL value is
 *     allowed, and in that case the function does nothing.
 */
void gameDestroy(Game game);


/**
 * gameCopy: copy a new game.
 *
 * @param game - the game to copy
 * 
 * @return The coppied game in case of success, and NULL otherwise (e.g.
 *     in case of an allocation error)
 */
Game gameCopy(Game game);


/**
 * gameRemovePlayer: Remove a player from a game, updating the winner if needed
 *
 * @param game - the game that should remove the player
 * @param player_id - the player that needs to be removed
 * 
 * @return
 *     CHESS_NULL_ARGUMENT - if game is NULL.
 *     CHESS_INVALID_ID - if the player ID number is invalid.
 *     CHESS_PLAYER_NOT_EXIST - if the player does not play in the game.
 *     CHESS_SUCCESS - if player was removed successfully.
 */
ChessResult gameRemovePlayer(Game game, int player_id);




/**
 * gameGetIdOfWinner: Get the id of the winner of a given game
 *
 * @param game - the game that should return it's winner
 * 
 * @return
 *     The winner of the game
 */
int gameGetIdOfWinner(Game game);


/**
 * gameGetID: Get the ID of a given game
 *
 * @param game - the game
 * 
 * @return
 *     The ID of the game
 */
int gameGetID(Game game);


/**
 * gameGetFirstPlayer: Get the first player of a game
 *
 * @param game - the game that should return the player
 * 
 * @return
 *     first player of said game
 */
//int gameGetFirstPlayer(Game game);


/**
 * gameGetSecondPlayer: Get the second player of a game
 *
 * @param game - the game that should return the player
 * 
 * @return
 *     second player of said game
 */
//int gameGetSecondPlayer(Game game);


// gameGetPlayTime
/**
 * GetPlayTime: Get the length of the game in seconds
 *
 * @param game - the game that should return it's length
 * 
 * @return
 *     the length of the game in seconds
 */
int gameGetPlayTime(Game game);



/**
 * gameisPlayerInGame: Checks if a given player has played in a given game
 *
 * @param game - the game that the check is being done on
 * @param player_id - the id of the player that the check if being done on
 * 
 * @return
 *     true - if the player has played in that game
 *     false - if the player hasn't played in that gmae
 */
bool gameisPlayerInGame(Game game, int player_id);


#endif