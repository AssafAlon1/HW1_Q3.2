#ifndef _GAME_H
#define _GAME_H

#define INVALID_GAME_ID -1
#define INVALID_PLAYER -3
#define DELETED_PLAYER -2

#include <stdio.h>
#include <stdbool.h>


typedef enum {
    GAME_OUT_OF_MEMORY,
    GAME_NULL_ARGUMENT,
    GAME_INVALID_ID,
    GAME_PLAYER_NOT_EXIST,
    GAME_SUCCESS
} GameResult ;

typedef enum {
    GAME_FIRST_PLAYER,
    GAME_SECOND_PLAYER,
    GAME_DRAW
} GameWinner;

typedef struct game_t *Game;


/**
 * gameCreate: create a new game.
 * 
 * @param tournament_id - the id of the tournament in which the game is being held
 * @param first_player - the id of the first player
 * @param second_player - the id of the second player
 * @param GameWinner - the winner of the game
 * @param play_time - the length of the game in seconds
 * 
 * @return A new game in case of success, and NULL otherwise (e.g.
 *     in case of an allocation error)
 */
Game gameCreate(int tournament_id, int first_player, int second_player, GameWinner winner, int play_time, int game_id);



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
 * @param game - the game from which we remove the player
 * @param player_id - the player that needs to be removed
 * 
 * @return
 *     GAME_NULL_ARGUMENT - if game is NULL.
 *     GAME_PLAYER_NOT_EXIST - if the player does not play in the game.
 *     GAME_INVALID_ID - if the player id is invalid
 *     GAME_SUCCESS - if player was removed successfully.
 */
GameResult gameRemovePlayer(Game game, int player_id);


/**
 * gameGetIdOfWinner: Get the id of the winner of a given game
 *
 * @param game - the game that should return it's winner
 * 
 * @return
 *     The winner of the game
 *     -1 - if the game is NULL
 *     INVALID_PLAYER - if the game ended in a tie
 */
int gameGetIdOfWinner(Game game);


/**
 * gameGetID: Get the ID of a given game
 *
 * @param game - the game
 * 
 * @return
 *     The ID of the game
 *     INVALID_GAME_ID - if the game is NULL
 */
int gameGetID(Game game);



/**
 * GetPlayTime: Get the length of the game in seconds
 *
 * @param game - the game that should return it's length
 * 
 * @return
 *     the length of the game in seconds
 *     GAME_NULL_ARGUMENT - if the game is NULL
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
 *     false - if the player hasn't played in that game
 */
bool gameisPlayerInGame(Game game, int player_id);



/**
 * gameGetTournamentID: Get the tournament ID of the game
 *
 * @param game - the game that should return it's tournament ID
 * 
 * @return
 *     the tournament ID of the game
 *     -1 - if the game is NULL
 */
int gameGetTournamentID(Game game);


/**
 * gameGetPlayersOpponent: Get the ID of a player's opponent in a given game
 *
 * @param game - the relevant game
 * @param player_id - the id of the player that shouldn't be returned
 * 
 * @return
 *     DELETED_PLAYER in the case that the opponent was deleted
 *     the ID of the opponent otherwise
 *     -1 - if the game is NULL
 *     INVALID_PLAYER - if the player did not play in that game
 */
int gameGetPlayersOpponent(Game game, int player_id);

#endif