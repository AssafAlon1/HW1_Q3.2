#ifndef _TOURNAMENT_H
#define _TOURNAMENT_H

#define TOURNAMENT_WIN_WEIGHT 2
#define TOURNAMENT_DRAW_WEIGHT 1
#define TOURNAMENT_LOSS_WEIGHT 0
#define TOURNAMENT_INVALID_INPUT -1

#include <stdio.h>
#include <stdbool.h>
#include "game.h"

typedef enum {
    TOURNAMENT_OUT_OF_MEMORY,
    TOURNAMENT_NULL_ARGUMENT,
    TOURNAMENT_INVALID_ID,
    TOURNAMENT_INVALID_LOCATION,
    TOURNAMENT_INVALID_MAX_GAMES,
    TOURNAMENT_GAME_ALREADY_EXISTS,
    TOURNAMENT_INVALID_PLAY_TIME,
    TOURNAMENT_EXCEEDED_GAMES,
    TOURNAMENT_NO_GAMES,
    TOURNAMENT_ENDED,
    TOURNAMENT_SUCCESS
} TournamentResult ;

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
 *     TOURNAMENT_NULL_ARGUMENT - if tournament is NULL.
 *     TOURNAMENT_INVALID_ID - if either the players or the winner is invalid.
 *     TOURNAMENT_GAME_ALREADY_EXIST - if there is already a game in the tournament with the same two players.
 *     TOURNAMENT_INVALID_PLAY_TIME - if the play time is negative.
 *     TOURNAMENT_OUT_OF_MEMORY    - if there was an allocation issue
 *     TOURNAMENT_SUCCESS - if game was added successfully.
 */
TournamentResult tournamentAddGame(Tournament tournament, int first_player, int second_player,
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
 *     TOURNAMENT_NULL_ARGUMENT - if tournament is NULL.
 *     TOURNAMENT_INVALID_ID - if the player ID number is invalid.
 *     TOURNAMENT_SUCCESS - if player was removed successfully.
 */
TournamentResult tournamentRemovePlayer(Tournament tournament, int player_id, int game_ids[]);



/**
 * tournamentEnd: The function will end the tournament and calculate the id of the winner.
 *                     The winner of the tournament is the player with the highest score.
 *                     Once the tournament is over, no games can be added for that tournament.
 *
 * @param tournament - the tournament
 * @param winner_id  - the id of the winner in the tournament
 *
 * @return
 *     TOURNAMENT_NULL_ARGUMENT - if Tournament is NULL.
 *     TOURNAMENT_SUCCESS - if tournament was ended successfully.
 *     TOURNAMENT_ENDED - if the tournament has already ended
 */
TournamentResult tournamentEnd (Tournament tournament, int winner_id);



/**
 * tournamentCalculateWinner: The function will calculate the winner of the tournament
 *
 * @param tournament - the tournament
 *
 * @return
 *     the id of the winner
 */
int tournamentCalculateWinner (Tournament tournament);


/**
 * tournamentGetSizePlayers: The function will return the number of unique players
 *                  in a given tournament
 *
 * @param tournament - the tournament
 *
 * @return
 *     The number of unique players in the tournament
 */
int tournamentGetSizePlayers (Tournament tournament);



/**
 * tournamentGetSizeGames: The function will return the number of unique games
 *                  in a given tournament
 *
 * @param tournament - the tournament.
 *
 * @return
 *     The number of unique games in the tournament
 */
int tournamentGetSizeGames (Tournament tournament);




/**
 * tournamentGetWinner: The function will return the winner of
 *                     a given tournament
 *
 * @param tournament - the tournament
 *
 * @return
 *     The winner of the tournament
 */
int tournamentGetWinner(Tournament tournament);


/**
 * tournamentGetLocation: The function will return the location of
 *                     a given tournament
 *
 * @param tournament - the tournament
 *
 * @return
 *     The location of the tournament
 */
char* tournamentGetLocation(Tournament tournament);



/**
 * tournamentGetLongestGameTime: The function will return the longest game time of
 *                     a given tournament
 *
 * @param tournament - the tournament
 *
 * @return
 *     The longest game time of the tournament
 */
int tournamentGetLongestGameTime(Tournament tournament);



/**
 * tournamentGetAverageGameTime: The function will return the average game time of
 *                     a given tournament
 *
 * @param tournament - the tournament
 *
 * @return
 *     The average game time of the tournament
 */
double tournamentGetAverageGameTime(Tournament tournament);


/**
 * tournamentValidateLocation: The function checks whether a given location is valid
 *
 * @param location - a string of a location
 *
 * @return
 *     true - if the location is valid
 *     false - if the loctaion is invalid
 */
bool tournamentValidateLocation(const char *location);


/**
 * tournamentGetMaxGamesPerPlayer: The function returns the maximum allowed games per player
 *
 * @param tournament - the tournament
 *
 * @return
 *     the maximum allowed games per player
 */
int tournamentGetMaxGamesPerPlayer(Tournament tournament);


/**
 * tournamentGetGame: The function returns a game with a given ID from a given tournament
 *
 * @param tournament - the tournament
 * @param game_id    - the id of the needed game
 *
 * @return
 *     The game with the given ID
 */
Game tournamentGetGame(Tournament tournament, int game_id);



/**
 * tournamentPrintStatsToFile: The function print its relevant stats to a given file
 *
 * @param tournament - the tournament that will extract its stats
 * @param output_file - the file the output will be written to
 *
 * @return
 *     true - if the operation completed successfully
 *     false - if the operation failed
 */
bool tournamentPrintStatsToFile(Tournament tournament, FILE *output_file);

#endif //  _TOURNAMENT_H