#ifndef _MAP_UTIL_H
#define _MAP_UTIL_H

#include <stdio.h>




/**
 * createGamesMap: creates a games map.
 *
 * @return a map that uses int as keys and games as data
 */
Map createGamesMap();


/**
 * createPlayerInTournamentsMap: creates a playerInTournament map.
 *
 * @return a map that uses int as keys and PlayerInTournament as data
 */
Map createPlayerInTournamentsMap();

/**
 * createPlayersMap: creates a player map.
 *
 * @return a map that uses int as keys and player as data
 */
Map createPlayersMap();

/**
 * createTournamentsMap: creates a tournaments map.
 *
 * @return a map that uses int as keys and tournaments as data
 */
Map createTournamentsMap();


/**
 * intCopy: copy an integer.
 *
 * @param num - pointer to int
 * 
 * @return a pointer to the coppied int
 */
void* intCopy (void *num);

/**
 * intFree: free an integer.
 *
 * @param num - pointer to int
 */
void intFree (void *num);


/**
 * intCompare: compare 2 integers.
 *
 * @param num1 - pointer to first int
 * @param num2 - pointer to second int
 * 
 * @return a number greater than 0 if num1's value is bigger
 *         a number lesser  than 0 if num2's value is bigger
 *         0 if *num1 = *num2
 */
int intCompare (void *num1, void *num2);


/**
 * gamePtrCopy: copy a game.
 *
 * @param game - pointer to game
 * 
 * @return a pointer to the coppied game
 */
void** gamePtrCopy (void* game);

/**
 * gamePtrFree: free a game.
 *
 * @param game - pointer to game
 */
void gamePtrFree (void* game);



// ============================================================//
// === Wrapper functions - uses void* for generic ADT usage ===//
// ============================================================//

void* playerInTournamentCopyWrapper(void *player_in_tournament);

void playerInTournamentDestroyWrapper(void *player_in_tournament);

void* gameCopyWrapper(void *game);

void gameDestroyWrapper(void *game);

void* playerCopyWrapper(void *player);

void playerDestroyWrapper(void *player);

void* tournamentCopyWrapper(void *tournament);

void tournamentDestroyWrapper(void *tournament);

#endif //_MAP_UTIL_H