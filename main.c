#include <stdio.h>
#include <stdlib.h>
#include "chessSystem.h"
#include "tournament.h"
#include "player.h"
#include "game.h"
#include "playerInTournament.h"
#include "assert.h"

void gameTests()
{
    printf(">>GAME Running basic tests game1...");
    Game game1 = gameCreate(1, 11, 12, FIRST_PLAYER, 300, 1);
    
    assert(game1 != NULL);
    assert(gameGetID(game1) == 1);
    assert(gameGetIdOfWinner(game1) == 11);
    assert(gameGetTournamentID(game1) == 1);
    assert(gameGetPlayTime(game1) == 300);

    assert(gameRemovePlayer(NULL, 11) == CHESS_NULL_ARGUMENT);
    assert(gameRemovePlayer(game1, 13) == CHESS_PLAYER_NOT_EXIST);
    assert(gameRemovePlayer(game1, 11) == CHESS_SUCCESS);

    assert(gameGetIdOfWinner(game1) == 12);
    assert(gameRemovePlayer(game1, 12) == CHESS_SUCCESS);
    assert(gameGetIdOfWinner(game1) == DELETED_PLAYER);

    gameDestroy(game1);
    game1 = NULL;
    printf("     [OK]\n");

    printf(">>GAME Running copy tests game2... ");
    Game game2 = gameCreate(2, 21, 22, DRAW, 15000, 630);

    assert(game2 != NULL);
    assert(gameGetID(game2) == 630);
    assert(gameGetIdOfWinner(game2) == INVALID_PLAYER);
    assert(gameGetTournamentID(game2) == 2);
    assert(gameGetPlayTime(game2) == 15000);
    
    Game game2_2 = gameCopy(game2);
    assert(game2_2 != NULL);
    assert(gameGetID(game2_2) == 630);
    assert(gameGetIdOfWinner(game2_2) == INVALID_PLAYER);
    assert(gameGetTournamentID(game2_2) == 2);
    assert(gameGetPlayTime(game2_2) == 15000);
    assert(gameRemovePlayer(game2_2, 21) == CHESS_SUCCESS);
    assert(gameGetIdOfWinner(game2) == INVALID_PLAYER);
    assert(gameGetIdOfWinner(game2_2) == 22);
    gameDestroy(game2_2);
    game2_2 = NULL;

    assert(gameRemovePlayer(game2, 21) == CHESS_SUCCESS);
    assert(gameGetIdOfWinner(game2) == 22);

    Game game3 = gameCopy(game2);
    assert(game3 != NULL);
    assert(gameGetID(game3) == 630);
    assert(gameGetTournamentID(game3) == 2);
    assert(gameGetPlayTime(game3) == 15000);
    assert(gameGetIdOfWinner(game3) == 22);

    gameDestroy(game2);
    gameDestroy(game3);
    printf("     [OK]\n");

}

void playerInTournamentTests()
{
    printf(">>PIT Running basic tests PIT1...");
    PlayerInTournament PIT1 = playerInTournamentCreate(1001, 1, 3);
    printf("1");
    assert(PIT1 != NULL);
    assert(playerInTournamentGetDraws(PIT1) == 0);
    assert(playerInTournamentGetLosses(PIT1) == 0);
    assert(playerInTournamentGetWins(PIT1) == 0);

    printf("      [OK]\n");
}

void playerTests()
{
    printf(">>PLAYER Running basic tests player1...");
    Player player1 = playerCreate(1001);
    assert(player1 != NULL);
    assert(playerGetTotalGames(player1) == 0);
    assert(playerGetLevel(player1) == 0);
    assert(playerGetTotalGames(player1) == 0);
    assert(playerGetFinishedGamesAverageTime(player1) < 0.001);
    
    Game game1 = gameCreate(1, 1001, 1002, FIRST_PLAYER, 300, 101);
    Game game2 = gameCreate(1, 1003, 1001, DRAW, 600, 102);
    assert(game1 != NULL);
    assert(game2 != NULL);

    assert(playerAddGame(player1, game1, 3) == CHESS_SUCCESS);
    assert(playerAddGame(player1, game2, 3) == CHESS_SUCCESS);
    gameDestroy(game1);
    gameDestroy(game2);
    playerDestroy(player1);
    printf(" [OK]\n");
}

int main ()
{
    gameTests();
    playerInTournamentTests();
    playerTests();
    return 0;
}