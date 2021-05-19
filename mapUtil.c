#include <stdio.h>
#include <stdlib.h>
#include "mapUtil.h"
#include "game.h"
#include "playerInTournament.h"

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