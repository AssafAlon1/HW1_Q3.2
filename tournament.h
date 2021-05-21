#ifndef _TOURNAMENT_H
#define _TOURNAMENT_H

#define TOURNAMENT_WIN_WEIGHT 2
#define TOURNAMENT_DRAW_WEIGHT 1
#define TOURNAMENT_LOSS_WEIGHT 0

#include <stdio.h>
#include <stdbool.h>
#include "chessSystem.h"


// typedef enum {
//     TOURNAMENT_OUT_OF_MEMORY,
//     TOURNAMENT_NULL_ARGUMENT,
//     TOURNAMENT_INVALID_ID,
//     TOURNAMENT_INVALID_LOCATION,
//     TOURNAMENT_INVALID_MAX_GAMES,
//     TOURNAMENT_GAME_ALREADY_EXISTS,
//     TOURNAMENT_INVALID_PLAY_TIME,
//     TOURNAMENT_EXCEEDED_GAMES,
//     TOURNAMENT_ENDED,
//     TOURNAMENT_SUCCESS
// } TournamentResult ;

/** Type for representing a tournament that will be managed by a Chess System */
typedef struct tournament_t *Tournament;


/**
 * tournamentCreate: create an empty tournament.
 *
 * @param tournament_id - the tournament id
 * @param max_games_per_player - the maximum games allowed per player
 * @param tournament_location - the tournament location
 * 
 * @return A new tournament in case of success, and NULL otherwise (e.g.
 *     in case of an allocation error)
 */
Tournament tournamentCreate(int tournament_id, int max_games_per_player,
                            const char *tournament_location);


/**
 * tournamentDestroy: free a tournament, and all its contents, from
 * memory.
 *
 * @param tournament - the tournament to free from memory. A NULL value is
 *     allowed, and in that case the function does nothing.
 */
void tournamentDestroy(Tournament tournament);


/**
 * tournamentCopy: copies a given tournament
 *
 * @param tournament - the tournament to copy
 * @return
 *     The coppied tournament
 */
Tournament tournamentCopy(Tournament tournament);


/**
 * tournamentAddGame: add a new match to a tournament.
 *
 * @param tournament - tournament system that contains the tournament. Must be non-NULL.
 * @param first_player - first player id. Must be non-negative.
 * @param second_player - second player id. Must be non-negative.
 * @param winner - indicates the winner in the match. if it is FIRST_PLAYER, then the first player won.
 *                 if it is SECOND_PLAYER, then the second player won, otherwise the match has ended with a draw.
 * @param play_time - duration of the match in seconds. Must be non-negative.
 * @param amount_of_new_players - 0/1/2 - the amount of players that this is their first game in the tournament
 * 
 * @return
 *     CHESS_NULL_ARGUMENT - if tournament is NULL.
 *     CHESS_INVALID_ID - if either the players or the winner is invalid.
 *     CHESS_GAME_ALREADY_EXIST - if there is already a game in the tournament with the same two players.
 *     CHESS_INVALID_PLAY_TIME - if the play time is negative.
 *     CHESS_SUCCESS - if game was added successfully.
 */
ChessResult tournamentAddGame(Tournament tournament, int first_player, int second_player,
                              Winner winner, int play_time, int amount_of_new_players);



/**
 * tournamentRemovePlayer: removes the player from the tournament.
 *                      In games where the player has participated and not yet ended,
 *                      the opponent is the winner automatically after removal.
 *
 * @param tournament - The tournament that should remove specified player. Must be non-NULL.
 * @param player_id - the player id. Must be non-negative.
 * @param game_ids  - array of game ids that the player has played in the tournament.
 *                    array's size is max_games_per_player, none games are INVALID_GAME_ID
 *
 * @return
 *     CHESS_NULL_ARGUMENT - if tournament is NULL.
 *     CHESS_INVALID_ID - if the player ID number is invalid.
 *     CHESS_PLAYER_NOT_EXIST - if the player does not play in the tournament.
 *     CHESS_SUCCESS - if player was removed successfully.
 */
ChessResult tournamentRemovePlayer(Tournament tournament, int player_id, int game_ids[]);



/**
 * tournamentEnd: The function will end the tournament and calculate the id of the winner.
 *                     The winner of the tournament is the player with the highest score.
 *                     Once the tournament is over, no games can be added for that tournament.
 *
 * @param tournament - the tournament id. Must be non-negative, and unique.
 * @param winner_id  - the id of the winner in the tournament
 *
 * @return
 *     CHESS_NULL_ARGUMENT - if Tournament is NULL.
 *     CHESS_SUCCESS - if tournament was ended successfully.
 *     CHESS_TOURNAMENT_ENDED - if the tournament has already ended
 */
ChessResult tournamentEnd (Tournament tournament, int winner_id);



/**
 * tournamentCalculateWinner: The function will calculate the winner of the tournament
 *
 * @param tournament - the tournament id. Must be non-negative, and unique.
 *
 * @return
 *     the id of the winner
 */
int tournamentCalculateWinner (Tournament tournament);


/**
 * tournamentGetSizePlayers: The function will return the number of unique players
 *                  in a given tournament
 *
 * @param tournament - the tournament id. Must be non-negative, and unique.
 *
 * @return
 *     The number of unique players in the tournament
 */
int tournamentGetSizePlayers (Tournament tournament);



/**
 * tournamentGetSizeGames: The function will return the number of unique games
 *                  in a given tournament
 *
 * @param tournament - the tournament id. Must be non-negative, and unique.
 *
 * @return
 *     The number of unique games in the tournament
 */
int tournamentGetSizeGames (Tournament tournament);


// /**
//  * tournamentUpdatePlayerInTournmnt: ???????
//  *
//  * @param tournament - the tournament id. Must be non-negative, and unique.
//  *
//  * @return
//  *     ?????
//  */
// void tournamentUpdatePlayerStatsInTournmnt (Tournament tournament);


// /**
//  * tournamentUpdateStatistics: ???????
//  *
//  * @param tournament - the tournament id. Must be non-negative, and unique.
//  *
//  * @return
//  *     ?????
//  */
// void tournamentUpdateStatistics (Tournament tournament);

#endif

