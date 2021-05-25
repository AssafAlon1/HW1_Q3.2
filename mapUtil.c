#include <stdio.h>
#include <stdlib.h>
#include "./mtm_map/map.h"
#include "mapUtil.h"
#include "game.h"
#include "playerInTournament.h"
#include "player.h"
#include "tournament.h"

void* intCopy (void *num)
{
    int* new_num = malloc(sizeof(new_num));
    *new_num = *(int*)num;
    return new_num;
}

void intFree (void *num)
{
    free (num);
}

int intCompare (void *num1, void *num2)
{
    return *(int*)num1 - *(int*)num2;
}

void** gamePtrCopy (void* pointer) // Is it ok?
{
    Game* new_pointer = malloc(sizeof(new_pointer));
    new_pointer = (Game*)pointer;
    return (void**)new_pointer;
}

void gamePtrFree (void* pointer)
{
    free(pointer);
}

void* playerInTournamentCopyWrapper(void *player_in_tournament)
{
    return playerInTournamentCopy((PlayerInTournament)player_in_tournament);
}

void playerInTournamentDestroyWrapper(void *player_in_tournament)
{
    playerInTournamentDestroy((PlayerInTournament)player_in_tournament);
}

void* gameCopyWrapper(void *game)
{
    return gameCopy((Game)game);
}

void gameDestroyWrapper(void *game)
{
    gameDestroy((Game)game);
}

void* playerCopyWrapper(void *player)
{
    return playerCopy((Player)player);
}

void playerDestroyWrapper(void *player)
{
    playerDestroy((Player)player);
}

void* tournamentCopyWrapper(void *tournament)
{
    return tournamentCopy((Tournament)tournament);
}

void tournamentDestroyWrapper(void *tournament)
{
    tournamentDestroy((Tournament)tournament);
}


Map createGamesMap()
{
    Map map = mapCreate(gameCopyWrapper, intCopy,
                        gameDestroyWrapper, intFree, intCompare);
    return map;
}


Map createPlayerInTournamentsMap()
{
    Map map = mapCreate(playerInTournamentCopyWrapper, intCopy,
                        playerInTournamentDestroyWrapper, intFree, intCompare );
    return map;
}


Map createPlayersMap()
{
    Map map = mapCreate(playerCopyWrapper, intCopy,
                        playerDestroyWrapper, intFree, intCompare );
    return map;
}


Map createTournamentsMap()
{
    Map map = mapCreate(tournamentCopyWrapper, intCopy,
                        tournamentDestroyWrapper, intFree, intCompare );
    return map;
}