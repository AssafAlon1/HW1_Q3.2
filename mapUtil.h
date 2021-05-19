#include <stdio.h>

void* intCopy (void *num);

void intFree (void *num);

int intCompare (void *num1, void *num2);

void** gamePtrCopy (void* pointer); // Is it ok?

void gamePtrFree (void* pointer);

void* playerInTournamentCopyWrapper(void *player_in_tournament);

void playerInTournamentDestroyWrapper(void *player_in_tournament);