#include <stdio.h>
#include <stdlib.h>
#include "chessSystem.h"
#include "tournament.h"
#include "player.h"
#include "game.h"
#include "playerInTournament.h"
#include "assert.h"


#define eps 0.0001

double my_abs(double x)
{
    return x > 0 ? x : -x;
}

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

    printf(">>GAME Running extra tests game2... ");
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
    printf("    [OK]\n");

}

void playerInTournamentTests()
{
    printf(">>PIT Running basic tests PIT1...");
    PlayerInTournament PIT1 = playerInTournamentCreate(1001, 1, 3);
    assert(PIT1 != NULL);
    assert(playerInTournamentGetDraws(PIT1) == 0);
    assert(playerInTournamentGetLosses(PIT1) == 0);
    assert(playerInTournamentGetWins(PIT1) == 0);
    assert(playerInTournamentGetTotalTime(PIT1) == 0);
    
    Game game1 = gameCreate(1, 1001, 1002, FIRST_PLAYER, 300, 101);
    assert(game1 != NULL);
    assert(playerInTournamentAddGame(PIT1, game1) == PLAYER_IN_TOURNAMENT_SUCCESS);
    gameDestroy(game1);

    assert(playerInTournamentGetDraws(PIT1) == 0);
    assert(playerInTournamentGetLosses(PIT1) == 0);
    assert(playerInTournamentGetWins(PIT1) == 1);
    assert(playerInTournamentGetTotalTime(PIT1) == 300);


    Game game2 = gameCreate(1, 1003, 1001, DRAW, 600, 102);
    assert(game2 != NULL);
    assert(playerInTournamentAddGame(PIT1, game2) == PLAYER_IN_TOURNAMENT_SUCCESS);
    gameDestroy(game2);

    assert(playerInTournamentGetDraws(PIT1) == 1);
    assert(playerInTournamentGetLosses(PIT1) == 0);
    assert(playerInTournamentGetWins(PIT1) == 1);
    assert(playerInTournamentGetTotalTime(PIT1) == 900);

    playerInTournamentDestroy(PIT1);
    printf("       [OK]\n");



    printf(">>PIT Running extra tests PIT2...");

    PlayerInTournament PIT2 = playerInTournamentCreate(1002, 2, 4);
    Game game3 = gameCreate(2, 1001, 1002, FIRST_PLAYER, 300, 101);
    Game game4 = gameCreate(2, 1002, 1003, DRAW, 601, 102);
    Game game5 = gameCreate(2, 1002, 1004, DRAW, 900, 103);
    assert(game1 != NULL && game2 != NULL && game3 != NULL);

    assert(playerInTournamentAddGame(NULL, NULL) == PLAYER_IN_TOURNAMENT_NULL_ARGUMENT);
    assert(playerInTournamentAddGame(PIT2, NULL) == PLAYER_IN_TOURNAMENT_NULL_ARGUMENT);
    assert(playerInTournamentAddGame(NULL, game3) == PLAYER_IN_TOURNAMENT_NULL_ARGUMENT);

    assert(playerInTournamentAddGame(PIT2, game3) == PLAYER_IN_TOURNAMENT_SUCCESS);
    assert(playerInTournamentAddGame(PIT2, game4) == PLAYER_IN_TOURNAMENT_SUCCESS);
    assert(playerInTournamentAddGame(PIT2, game5) == PLAYER_IN_TOURNAMENT_SUCCESS);
    assert(playerInTournamentGetDraws(PIT2) == 2);
    assert(playerInTournamentGetLosses(PIT2) == 1);
    assert(playerInTournamentGetWins(PIT2) == 0);
    assert(playerInTournamentGetTotalTime(PIT2) == 1801);

    Game game6 = gameCreate(2, 1005, 1001, SECOND_PLAYER, 999, 104);
    assert(playerInTournamentAddGame(PIT2, game6) == PLAYER_IN_TOURNAMENT_PLAYER_NOT_IN_GAME);
    gameDestroy(game6);

    Game game7 = gameCreate(1, 1002, 1007, FIRST_PLAYER, 612, 105);
    assert(playerInTournamentAddGame(PIT2, game7) == PLAYER_IN_TOURNAMENT_CONFLICT_ID);
    gameDestroy(game7);

    PlayerInTournament PIT3 = playerInTournamentCopy(PIT2);
    assert(playerInTournamentAddGame(PIT3, game3) == PLAYER_IN_TOURNAMENT_GAME_ALREADY_EXISTS);
    assert(playerInTournamentAddGame(PIT3, game4) == PLAYER_IN_TOURNAMENT_GAME_ALREADY_EXISTS);
    assert(playerInTournamentAddGame(PIT3, game5) == PLAYER_IN_TOURNAMENT_GAME_ALREADY_EXISTS);
    assert(playerInTournamentGetDraws(PIT3) == 2);
    assert(playerInTournamentGetLosses(PIT3) == 1);
    assert(playerInTournamentGetWins(PIT3) == 0);
    assert(playerInTournamentGetTotalTime(PIT3) == 1801);
    playerInTournamentDestroy(PIT3);
    gameDestroy(game3);
    gameDestroy(game4);
    gameDestroy(game5);

    Game game8 = gameCreate(2, 1002, 1003, FIRST_PLAYER, 43125, 102);
    assert(playerInTournamentAddGame(PIT2, game8) == PLAYER_IN_TOURNAMENT_GAME_ALREADY_EXISTS);
    gameDestroy(game8);

    Game game9 = gameCreate(2, 1002, 1011, FIRST_PLAYER, 43125, 110);
    assert(playerInTournamentAddGame(PIT2, game9) == PLAYER_IN_TOURNAMENT_SUCCESS);
    assert(playerInTournamentAddGame(PIT2, game9) == PLAYER_IN_TOURNAMENT_GAME_ALREADY_EXISTS);
    assert(playerInTournamentGetTotalGames(PIT2) == 4);
    assert(playerInTournamentGetTotalTime(PIT2) == 44926);
    gameDestroy(game9);

    Game game10 = gameCreate(2, 1002, 1012, FIRST_PLAYER, 10, 111);
    assert(playerInTournamentAddGame(PIT2, game10) == PLAYER_IN_TOURNAMENT_EXCEEDED_GAMES);
    gameDestroy(game10);
    
    playerInTournamentDestroy(PIT2);
    printf("       [OK]\n");

}

void playerTests()
{
    printf(">>PLAYER Running basic tests player1...");
    Player player1 = playerCreate(1001);
    assert(player1 != NULL);
    assert(playerGetTotalGames(player1) == 0);
    assert(playerGetLevel(player1) == 0);
    assert(playerGetTotalGames(player1) == 0);
    assert(my_abs(playerGetFinishedGamesAverageTime(player1)) < eps);

    Game game1 = gameCreate(1, 1001, 1002, FIRST_PLAYER, 300, 101);
    Game game2 = gameCreate(1, 1003, 1001, DRAW, 600, 102);
    assert(game1 != NULL && game2 != NULL);
    assert(playerAddGame(player1, game1, 3) == PLAYER_SUCCESS);
    assert(playerAddGame(player1, game2, 3) == PLAYER_SUCCESS);
    gameDestroy(game1);
    gameDestroy(game2);
    assert(playerGetTotalGames(player1) == 2);
    assert(my_abs(playerGetFinishedGamesAverageTime(player1) - 450) < eps);
    assert(my_abs(playerGetLevel(player1) - 4) < eps);

    Game game3 = gameCreate(1, 1001, 1004, SECOND_PLAYER, 900, 103);
    Game game4 = gameCreate(2, 1001, 1004, SECOND_PLAYER, 900, 103); ////
    assert(game3 != NULL && game4 != NULL);
    assert(playerAddGame(player1, game3, 3) == PLAYER_SUCCESS);
    assert(playerAddGame(player1, game3, 3) == PLAYER_GAME_ALREADY_EXISTS);
    assert(playerAddGame(player1, game4, 3) == PLAYER_SUCCESS);
    gameDestroy(game3);
    gameDestroy(game4);
    assert(playerGetTotalGames(player1) == 4);
    assert(my_abs(playerGetFinishedGamesAverageTime(player1) - 675) < eps);
    assert(my_abs(playerGetLevel(player1) - (-3)) < eps);

    Game game5 = gameCreate(2, 1001, 1005, SECOND_PLAYER, 900, 104); ////
    Game game6 = gameCreate(2, 1001, 1006, SECOND_PLAYER, 900, 105); ////
    Game game7 = gameCreate(2, 1001, 1007, SECOND_PLAYER, 900, 106);
    Game game8 = gameCreate(1, 1001, 1007, FIRST_PLAYER, 900, 120);
    assert (game5 != NULL && game6 != NULL && game7 != NULL && game8 != NULL);
    assert(playerAddGame(player1, game8, 3) == PLAYER_EXCEEDED_GAMES);
    assert(playerAddGame(player1, game5, 3) == PLAYER_SUCCESS);
    assert(playerAddGame(player1, game6, 3) == PLAYER_SUCCESS);
    assert(playerAddGame(player1, game7, 3) == PLAYER_EXCEEDED_GAMES);
    gameDestroy(game5);
    gameDestroy(game6);
    gameDestroy(game7);
    gameDestroy(game8);

    printf("1");
    assert(playerRemoveTournament(player1, 1) == PLAYER_SUCCESS);
    printf("2");
    assert(playerGetTotalGames(player1) == 2700);
    printf("3");
    assert(my_abs(playerGetFinishedGamesAverageTime(player1) - 900) < eps);
    printf("4");
    assert(my_abs(playerGetLevel(player1) - (-2)) < eps);
    printf("5");
    
    playerDestroy(player1);
    printf(" [OK]\n");

    //printf(">>PLAYER Running basic tests player1...");

}

int main ()
{
    gameTests();
    playerInTournamentTests();
    playerTests();
    return 0;
}