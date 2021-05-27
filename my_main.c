#include <stdio.h>
#include <stdlib.h>
#include "chessSystem.h"
#include "tournament.h"
#include "player.h"
#include "game.h"
#include "playerInTournament.h"
#include "assert.h"
#include "string.h"

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

    assert(gameRemovePlayer(NULL, 11) == GAME_NULL_ARGUMENT);
    assert(gameRemovePlayer(game1, 13) == GAME_PLAYER_NOT_EXIST);
    assert(gameRemovePlayer(game1, 11) == GAME_SUCCESS);

    assert(gameGetIdOfWinner(game1) == 12);
    assert(gameRemovePlayer(game1, 12) == GAME_SUCCESS);
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
    assert(gameRemovePlayer(game2_2, 21) == GAME_SUCCESS);
    assert(gameGetIdOfWinner(game2) == INVALID_PLAYER);
    assert(gameGetIdOfWinner(game2_2) == 22);
    gameDestroy(game2_2);
    game2_2 = NULL;

    assert(gameRemovePlayer(game2, 21) == GAME_SUCCESS);
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
    assert(playerAddTournament(player1, 1, 3) == PLAYER_SUCCESS);
    assert(playerAddGame(player1, game1) == PLAYER_SUCCESS);
    assert(playerAddGame(player1, game2) == PLAYER_SUCCESS);
    gameDestroy(game1);
    gameDestroy(game2);
    assert(playerGetTotalGames(player1) == 2);
    assert(my_abs(playerGetFinishedGamesAverageTime(player1) - 450) < eps);
    assert(my_abs(playerGetLevel(player1) - 4) < eps);

    Game game3 = gameCreate(1, 1001, 1004, SECOND_PLAYER, 900, 103);
    assert(playerAddTournament(player1, 2, 3) == PLAYER_SUCCESS);
    Game game4 = gameCreate(2, 1001, 1004, SECOND_PLAYER, 900, 103); ////
    assert(game3 != NULL && game4 != NULL);
    assert(playerAddGame(player1, game3) == PLAYER_SUCCESS);
    assert(playerAddGame(player1, game3) == PLAYER_GAME_ALREADY_EXISTS);
    assert(playerAddGame(player1, game4) == PLAYER_SUCCESS);
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
    assert(playerAddGame(player1, game8) == PLAYER_EXCEEDED_GAMES);
    assert(playerAddGame(player1, game5) == PLAYER_SUCCESS);
    assert(playerAddGame(player1, game6) == PLAYER_SUCCESS);
    assert(playerAddGame(player1, game7) == PLAYER_EXCEEDED_GAMES);
    gameDestroy(game5);
    gameDestroy(game6);
    gameDestroy(game7);
    gameDestroy(game8);

    assert(playerRemoveTournament(player1, 1) == PLAYER_SUCCESS);
    assert(playerGetTotalGames(player1) == 3);
    assert(my_abs(playerGetFinishedGamesAverageTime(player1) - 900) < eps);
    assert(my_abs(playerGetLevel(player1) - (-10)) < eps);

    playerDestroy(player1);
    printf(" [OK]\n");


    printf(">>PLAYER Running extra tests player2...");

    Player player2 = playerCreate(1002);
    Game game11 = gameCreate(1, 1001, 1002, FIRST_PLAYER, 300, 101);
    Game game12 = gameCreate(1, 1003, 1001, DRAW, 600, 102);
    assert(game1 != NULL && game2 != NULL);
    assert(playerAddTournament(player1, 1, 3) == PLAYER_SUCCESS);
    assert(playerAddGame(player1, game11) == PLAYER_SUCCESS);
    assert(playerAddGame(player1, game12) == PLAYER_NOT_IN_GAME);
    gameDestroy(game11);
    gameDestroy(game12);

    Player player3 = playerCopy(player2);
    assert(player3 != NULL);
    assert(playerGetTotalGames(player3) == 1);
    assert(my_abs(playerGetFinishedGamesAverageTime(player3) - 300) < eps);
    assert(my_abs(playerGetLevel(player3) - (-10)) < eps);

    Game game13 = gameCreate(1, 1002, 1003, FIRST_PLAYER, 700, 103);
    assert(game13 != NULL);
    assert(playerAddGame(player2, game13) == PLAYER_SUCCESS);
    assert(playerAddGame(player2, game13) == PLAYER_GAME_ALREADY_EXISTS);
    assert(playerAddGame(player2, game13) == PLAYER_GAME_ALREADY_EXISTS);
    assert(playerAddGame(player2, game13) == PLAYER_GAME_ALREADY_EXISTS);
    assert(playerGetTotalGames(player3) == 1);
    assert(playerGetTotalGames(player2) == 2);
    assert(my_abs(playerGetLevel(player2) - (-2)) < eps);
    assert(playerAddGame(player3, game13) == PLAYER_SUCCESS);
    gameDestroy(game13);

    Game game14 = gameCreate(1, 1002, 1004, FIRST_PLAYER, 500, 104);
    Game game15 = gameCreate(1, 1002, 1005, FIRST_PLAYER, 346, 105);
    assert(game14 != NULL && game15 != NULL);
    assert(playerAddGame(player2, game14) == PLAYER_SUCCESS);
    assert(playerAddGame(player2, game14) == PLAYER_GAME_ALREADY_EXISTS);
    assert(playerAddGame(player2, game15) == PLAYER_EXCEEDED_GAMES);
    assert(playerAddGame(player3, game15) == PLAYER_SUCCESS);
    gameDestroy(game14);
    gameDestroy(game15);
    assert(playerGetTotalGames(player3) == 3);
    assert(playerGetTotalGames(player2) == 3);
    playerDestroy(player3);
    assert(my_abs(playerGetLevel(player2) - ((double)2/3)) < eps);
    assert(my_abs(playerGetFinishedGamesAverageTime(player2) - 500) < eps);
    
    playerDestroy(player2);
    printf(" [OK]\n");
}

void tournamentTests()
{
    printf(">>TRNMNT Running basic tests tour1...");
    Tournament tour1 = tournamentCreate(1, 3, "Russia");
    assert(tour1 != NULL);
    
    assert(tournamentGetAverageGameTime(tour1) == 0);
    assert(tournamentGetLongestGameTime(tour1) == 0);
    assert(tournamentGetSizeGames(tour1) == 0);
    assert(tournamentGetSizePlayers(tour1) == 0);
    assert(tournamentGetWinner(tour1) == INVALID_PLAYER);
    assert(strcmp(tournamentGetLocation(tour1), "Russia") == 0);

    assert(tournamentAddGame(tour1, 1001, 1002, FIRST_PLAYER, 300, 2) == TOURNAMENT_SUCCESS);
    assert(tournamentAddGame(tour1, 1001, 1003, FIRST_PLAYER, 303, 1) == TOURNAMENT_SUCCESS);
    assert(tournamentAddGame(tour1, 1002, 1003, DRAW,         300, 0) == TOURNAMENT_SUCCESS);
    assert(my_abs(tournamentGetAverageGameTime(tour1) - 301) < eps);
    assert(tournamentGetLongestGameTime(tour1) == 303);
    assert(tournamentGetSizeGames(tour1) == 3);
    assert(tournamentGetSizePlayers(tour1) == 3);
    assert(tournamentGetWinner(tour1) == INVALID_PLAYER);
    tournamentEnd(tour1, 1001);
    assert(tournamentGetWinner(tour1) == 1001);

    tournamentDestroy(tour1);
    printf("   [OK]\n");



    printf(">>TRNMNT Running extra tests tour2...");
    Tournament tour2 = tournamentCreate(2, 3, "Israel home");
    assert(tour2 != NULL);
    assert(tournamentAddGame(tour2, 1001, 1002, FIRST_PLAYER, 1000, 2) == TOURNAMENT_SUCCESS);
    assert(tournamentAddGame(tour2, 1001, 1003, FIRST_PLAYER, 900, 1) == TOURNAMENT_SUCCESS);
    assert(tournamentAddGame(tour2, 1002, 1003, DRAW,         800, 0) == TOURNAMENT_SUCCESS);
    assert(tournamentAddGame(tour2, 1004, 1001, FIRST_PLAYER, 700, 1) == TOURNAMENT_SUCCESS);
    assert(tournamentAddGame(tour2, 1004, 1002, FIRST_PLAYER, 600, 0) == TOURNAMENT_SUCCESS);
    assert(tournamentAddGame(tour2, 1004, 1003, FIRST_PLAYER, 500, 0) == TOURNAMENT_SUCCESS);

    assert(my_abs(tournamentGetAverageGameTime(tour2) - 750) < eps);
    assert(tournamentGetLongestGameTime(tour2) == 1000);
    assert(tournamentGetSizeGames(tour2) == 6);
    assert(tournamentGetSizePlayers(tour2) == 4);

    assert(tournamentAddGame(tour2, 1004, 1004, FIRST_PLAYER, 500, 1) == TOURNAMENT_INVALID_ID);
    assert(my_abs(tournamentGetAverageGameTime(tour2) - 750) < eps);
    assert(tournamentGetLongestGameTime(tour2) == 1000);
    assert(tournamentGetSizeGames(tour2) == 6);
    assert(tournamentGetSizePlayers(tour2) == 4);
    
    tournamentEnd(tour2, 1004);
    assert(tournamentGetWinner(tour1) == 1004);
    tournamentDestroy(tour2);

    assert(tournamentCreate(3, 10, "invalid name") == NULL);
    assert(tournamentCreate(3, 10, "Invalid Name") == NULL);
    assert(tournamentCreate(3, 10, "Invalid Name") == NULL);
    assert(tournamentCreate(3, 10, "Invalid name2") == NULL);
    assert(tournamentCreate(3, 10, "Invalid name! really") == NULL);

    printf("   [OK]\n");


    // printf(">>TRNMNT Running print tests tour3...");
    // Tournament tour3 = tournamentCreate(2, 3, "Taub one");
    // assert(tour3 != NULL);
    // assert(tournamentAddGame(tour3, 1001, 1002, FIRST_PLAYER, 1000, 2) == CHESS_SUCCESS);
    // assert(tournamentAddGame(tour3, 1001, 1003, FIRST_PLAYER, 900, 1) == CHESS_SUCCESS);
    // assert(tournamentAddGame(tour3, 1002, 1003, DRAW,         800, 0) == CHESS_SUCCESS);
    // assert(tournamentAddGame(tour3, 1004, 1001, FIRST_PLAYER, 700, 1) == CHESS_SUCCESS);
    // assert(tournamentAddGame(tour3, 1004, 1002, FIRST_PLAYER, 600, 0) == CHESS_SUCCESS);
    // assert(tournamentAddGame(tour3, 1004, 1003, FIRST_PLAYER, 500, 0) == CHESS_SUCCESS);
    // assert(tournamentPrintStatsToFile(tour3, stdout) == true);

    // tournamentDestroy(tour3);
    // printf("   [OK]\n");
}

void chessTests()
{
    // Checking NULL handling
    printf(">>CHESS verifying NULL handling...");

    assert(my_abs(chessCalculateAveragePlayTime(NULL, 4, NULL) + 10) < eps);
    assert(chessEndTournament(NULL, 3) == CHESS_NULL_ARGUMENT);
    assert(chessRemovePlayer(NULL, 101) == CHESS_NULL_ARGUMENT);
    assert(chessSavePlayersLevels(NULL, stdout) == CHESS_NULL_ARGUMENT);
    assert(chessSaveTournamentStatistics(NULL, "./output/actual/basic1.txt") == CHESS_NULL_ARGUMENT);
    assert(chessAddGame(NULL, 1, 2, 3, FIRST_PLAYER, 300) == CHESS_NULL_ARGUMENT);
    assert(chessAddTournament(NULL, 101, 5, "Nowhere") == CHESS_NULL_ARGUMENT);
    assert(chessAddTournament(NULL, 101, 5, "INVALID5NAME*") == CHESS_NULL_ARGUMENT);
    assert(chessRemoveTournament(NULL, 1) == CHESS_NULL_ARGUMENT);

    printf("      [OK]\n");


    // Chess creation and basic operations
    printf(">>CHESS verifying basic operation...");
    
    // Creation of chess & Add tournaments
    ChessSystem chess1 = chessCreate();
    assert(chess1 != NULL);
    assert(chessAddTournament(chess1, 1, 3, "Turkey") == CHESS_SUCCESS);
    assert(chessAddTournament(chess1, 2, 3, "THE UNKNOWN") == CHESS_INVALID_LOCATION);
    assert(chessAddTournament(chess1, 2, 3, "the unknown") == CHESS_INVALID_LOCATION);
    assert(chessAddTournament(chess1, 2, 3, "The unkn0wn") == CHESS_INVALID_LOCATION);
    assert(chessAddTournament(chess1, 2, 3, "a") == CHESS_INVALID_LOCATION);
    assert(chessAddTournament(chess1, 2, 5, "A") == CHESS_SUCCESS);
    assert(chessAddTournament(chess1, 2, 3, "A") == CHESS_TOURNAMENT_ALREADY_EXISTS);

    //Trying to get statistics when there are none
    ChessResult result_out = CHESS_SUCCESS;
    double avg = chessCalculateAveragePlayTime(chess1, 1, &result_out);
    if (1 == 0)
    {
        printf("avg %f", avg);
    }
    assert(result_out == CHESS_PLAYER_NOT_EXIST);
    avg = chessCalculateAveragePlayTime(chess1, -2, &result_out);
    assert(result_out == CHESS_INVALID_ID);
    //
    // ADD SOME MORE HERE
    //


    // Ending tournaments
    assert(chessEndTournament(chess1, -5) == CHESS_INVALID_ID);
    assert(chessEndTournament(chess1, 11) == CHESS_TOURNAMENT_NOT_EXIST);
    assert(chessEndTournament(chess1, 1) == CHESS_NO_GAMES);


    // Adding games
    assert(chessAddGame(chess1, -5, 1001, 1002, FIRST_PLAYER, 600) == CHESS_INVALID_ID);
    assert(chessAddGame(chess1, 0, 1001, 1002, FIRST_PLAYER, 600) == CHESS_INVALID_ID);
    assert(chessAddGame(chess1, 11, 1001, 1002, FIRST_PLAYER, 600) == CHESS_TOURNAMENT_NOT_EXIST);
    assert(chessAddGame(chess1, 1, 1001, 1002, FIRST_PLAYER, 600) == CHESS_SUCCESS);
    assert(my_abs(chessCalculateAveragePlayTime(chess1, 1001, &result_out) - 600) < eps);
    assert(chessEndTournament(chess1, 1) == CHESS_SUCCESS);
    assert(chessEndTournament(chess1, 1) == CHESS_TOURNAMENT_ENDED);
    assert(chessAddGame(chess1, 1, 1001, 1002, FIRST_PLAYER, 600) == CHESS_TOURNAMENT_ENDED);
    assert(chessSaveTournamentStatistics(chess1, "./output/actual/basic2.txt") == CHESS_SUCCESS);
    // tournament 1 has 1 match. output file should look like:
    // 1001 ; 600 ; 600.00 ; Turkey ; 1 ; 2

    assert(chessAddGame(chess1, 2, 1001, 1002, FIRST_PLAYER, 1) == CHESS_SUCCESS);
    assert(chessAddGame(chess1, 2, 1001, 1003, SECOND_PLAYER, 2) == CHESS_SUCCESS);
    assert(chessAddGame(chess1, 2, 1004, 1001, DRAW, 1) == CHESS_SUCCESS);
    assert(chessAddGame(chess1, 2, 1001, 1005, SECOND_PLAYER, 1) == CHESS_SUCCESS);
    assert(chessAddGame(chess1, 2, 1002, 1003, FIRST_PLAYER, 2) == CHESS_SUCCESS);
    assert(chessAddGame(chess1, 2, 1004, 1002, FIRST_PLAYER, 1) == CHESS_SUCCESS);
    assert(chessAddGame(chess1, 2, 1002, 1005, SECOND_PLAYER, 2) == CHESS_SUCCESS);
    assert(chessAddGame(chess1, 2, 1003, 1004, FIRST_PLAYER, 1) == CHESS_SUCCESS);
    assert(chessAddGame(chess1, 2, 1005, 1004, SECOND_PLAYER, 1) == CHESS_SUCCESS);
    FILE *file = fopen("./output/actual/basic3.txt", "w");
    assert(chessSavePlayersLevels(chess1, file) == CHESS_SUCCESS);
    fclose(file);
    assert(chessEndTournament(chess1, 2) == CHESS_SUCCESS);
    //assert(chessSaveTournamentStatistics(chess1, "./output/actual/basic4.txt") == CHESS_SUCCESS);
    // 1001: 3
    // 1002: 2
    // 1003: 4
    // 1004: 3
    // 1005: 6 <-
    // 2 ; 1.33 ; A ; 9 ; 5


    chessDestroy(chess1);
    printf("    [OK]\n");
}


void massiveTest()
{
    printf(">>FINAL check with massive input...");

    ChessSystem chess1 = chessCreate();
    assert (chess1 != NULL);
    
    // Add Tournaments
    assert(chessAddTournament(chess1, 1, 3, "Chile land")     == CHESS_SUCCESS);
    assert(chessAddTournament(chess1, 2, 10, "Mother russia") == CHESS_SUCCESS);
    assert(chessAddTournament(chess1, 2, 5, "Mother russia")  == CHESS_TOURNAMENT_ALREADY_EXISTS);
    assert(chessAddTournament(chess1, 3, 5, "Mother Russia")  == CHESS_INVALID_LOCATION);
    assert(chessAddTournament(chess1, 3, 10, "Mother russia") == CHESS_SUCCESS);

    // Add Games
    assert(chessAddGame(chess1, 1, 1001, 1002, FIRST_PLAYER, 90)    == CHESS_SUCCESS);
    assert(chessAddGame(chess1, 1, 1001, 1003, FIRST_PLAYER, 90)    == CHESS_SUCCESS);
    assert(chessAddGame(chess1, 1, 1004, 1001, SECOND_PLAYER, 90)   == CHESS_SUCCESS);
    assert(chessAddGame(chess1, 1, 1002, 1005, DRAW, 150)           == CHESS_SUCCESS);
    assert(chessAddGame(chess1, 1, 1003, 1002, DRAW, 420)           == CHESS_SUCCESS);
    assert(chessAddGame(chess1, 1, 1005, 1004, FIRST_PLAYER, 666)   == CHESS_SUCCESS);
    assert(chessAddGame(chess1, 1, 1003, 1005, SECOND_PLAYER, 6969) == CHESS_SUCCESS);
    assert(chessAddGame(chess1, 1, 1010, 1011, FIRST_PLAYER, 4500)  == CHESS_SUCCESS);
    assert(chessAddGame(chess1, 1, 1010, 1012, FIRST_PLAYER, 4499)  == CHESS_SUCCESS);
    assert(chessAddGame(chess1, 1, 1010, 1013, FIRST_PLAYER, 4499)  == CHESS_SUCCESS);
    
    // Verify AddGame error handling
    assert(chessAddGame(chess1, 7, -5, 1001,   FIRST_PLAYER, -4) == CHESS_INVALID_ID);
    assert(chessAddGame(chess1, 7, 5, 1001,    FIRST_PLAYER, -4) == CHESS_TOURNAMENT_NOT_EXIST);
    assert(chessAddGame(chess1, 1, 1011, 1011, FIRST_PLAYER, -4) == CHESS_INVALID_ID);
    assert(chessAddGame(chess1, 1, 1010, 1011, FIRST_PLAYER, -4) == CHESS_GAME_ALREADY_EXISTS);
    assert(chessAddGame(chess1, 1, 1010, 1015, FIRST_PLAYER, -4) == CHESS_INVALID_PLAY_TIME);
    assert(chessAddGame(chess1, 1, 1010, 1015, FIRST_PLAYER,  4) == CHESS_EXCEEDED_GAMES);

    // Output to file, verify EndTournament error handling
    FILE *file = fopen("./output/actual/levels1.txt", "w");
    assert(chessSavePlayersLevels(chess1, file) == CHESS_SUCCESS);
    fclose(file);

    assert(chessSaveTournamentStatistics(chess1, "./output/actual/tournaments0.txt") == CHESS_NO_TOURNAMENTS_ENDED);
    assert(chessEndTournament(chess1, 1) == CHESS_SUCCESS);
    assert(chessEndTournament(chess1, 1) == CHESS_TOURNAMENT_ENDED);
    assert(chessEndTournament(chess1, 2) == CHESS_NO_GAMES);
    assert(chessSaveTournamentStatistics(chess1, "./output/actual/tournaments1.txt") == CHESS_SUCCESS);

    // Verify AddGame error handling - TournamentEnded
    assert(chessAddGame(chess1, 7, -5, 1001,   FIRST_PLAYER, -4) == CHESS_INVALID_ID);
    assert(chessAddGame(chess1, 7, 5, 1001,    FIRST_PLAYER, -4) == CHESS_TOURNAMENT_NOT_EXIST);
    assert(chessAddGame(chess1, 1, 1011, 1011, FIRST_PLAYER, -4) == CHESS_INVALID_ID);
    assert(chessAddGame(chess1, 1, 1010, 1017, FIRST_PLAYER, -4) == CHESS_TOURNAMENT_ENDED);


    // Verify player remove
    assert(chessRemovePlayer(chess1, -2)   == CHESS_INVALID_ID);
    assert(chessRemovePlayer(chess1, 5)    == CHESS_PLAYER_NOT_EXIST);
    assert(chessRemovePlayer(chess1, 1001) == CHESS_SUCCESS);

    // Levels shouldn't be affected
    file = fopen("./output/actual/levels2.txt", "w");
    assert(chessSavePlayersLevels(chess1, file) == CHESS_SUCCESS);
    fclose(file);

    // Second tournament
    assert(chessAddGame(chess1, 2, 1001, 1002, FIRST_PLAYER, 90)    == CHESS_SUCCESS);
    assert(chessAddGame(chess1, 2, 1001, 1003, FIRST_PLAYER, 90)    == CHESS_SUCCESS);
    assert(chessAddGame(chess1, 2, 1004, 1001, SECOND_PLAYER, 90)   == CHESS_SUCCESS);
    assert(chessAddGame(chess1, 2, 1002, 1005, DRAW, 150)           == CHESS_SUCCESS);
    assert(chessAddGame(chess1, 2, 1003, 1002, DRAW, 420)           == CHESS_SUCCESS);
    assert(chessAddGame(chess1, 2, 1005, 1004, FIRST_PLAYER, 666)   == CHESS_SUCCESS);
    assert(chessAddGame(chess1, 2, 1003, 1005, SECOND_PLAYER, 6969) == CHESS_SUCCESS);
    assert(chessAddGame(chess1, 2, 1010, 1011, FIRST_PLAYER, 4500)  == CHESS_SUCCESS);
    assert(chessAddGame(chess1, 2, 1010, 1012, FIRST_PLAYER, 4499)  == CHESS_SUCCESS);
    assert(chessAddGame(chess1, 2, 1010, 1013, FIRST_PLAYER, 4499)  == CHESS_SUCCESS);

    // Levels shouldn't be affected
    file = fopen("./output/actual/levels3.txt", "w");
    assert(chessSavePlayersLevels(chess1, file) == CHESS_SUCCESS);
    fclose(file);

    // Levels should change
    assert(chessRemovePlayer(chess1, 1001) == CHESS_SUCCESS);
    file = fopen("./output/actual/levels4.txt", "w");
    assert(chessSavePlayersLevels(chess1, file) == CHESS_SUCCESS);
    fclose(file);

    // Cleaning
    assert(chessSaveTournamentStatistics(chess1, "./output/actual/tournaments2.txt") == CHESS_SUCCESS);
    assert(chessRemoveTournament(chess1, 1) == CHESS_SUCCESS);
    assert(chessSaveTournamentStatistics(chess1, "./output/actual/tournaments3.txt") == CHESS_NO_TOURNAMENTS_ENDED);
    assert(chessEndTournament(chess1, 2));
    assert(chessSaveTournamentStatistics(chess1, "./output/actual/tournaments3.txt") == CHESS_SUCCESS);

    chessDestroy(chess1);



    ChessSystem chess2 = chessCreate();
    assert (chess2 != NULL);
    
    // Add Tournaments
    assert(chessAddTournament(chess2, 1, 3, "Israel")  == CHESS_SUCCESS);
    assert(chessAddTournament(chess2, 2, 10, "Taub")   == CHESS_SUCCESS);
    assert(chessAddTournament(chess2, 3, 10, "Taub the second")   == CHESS_SUCCESS);
    assert(chessAddTournament(chess2, 4, 10, "Diamond is unbreakable") == CHESS_SUCCESS);
    assert(chessAddTournament(chess2, 5, 10, "Golden wind") == CHESS_SUCCESS);

    // Add Games
    assert(chessAddGame(chess2, 1, 1001, 1002, FIRST_PLAYER, 90)    == CHESS_SUCCESS);
    assert(chessAddGame(chess2, 1, 1001, 1003, FIRST_PLAYER, 90)    == CHESS_SUCCESS);
    assert(chessAddGame(chess2, 1, 1004, 1001, SECOND_PLAYER, 90)   == CHESS_SUCCESS);
    assert(chessAddGame(chess2, 1, 1002, 1005, DRAW, 150)           == CHESS_SUCCESS);
    assert(chessAddGame(chess2, 1, 1003, 1002, DRAW, 420)           == CHESS_SUCCESS);
    assert(chessAddGame(chess2, 1, 1005, 1004, FIRST_PLAYER, 666)   == CHESS_SUCCESS);
    assert(chessAddGame(chess2, 1, 1003, 1005, SECOND_PLAYER, 6969) == CHESS_SUCCESS);
    assert(chessAddGame(chess2, 1, 1010, 1011, FIRST_PLAYER, 4500)  == CHESS_SUCCESS);
    assert(chessAddGame(chess2, 1, 1010, 1012, FIRST_PLAYER, 4499)  == CHESS_SUCCESS);
    assert(chessAddGame(chess2, 1, 1010, 1013, FIRST_PLAYER, 4499)  == CHESS_SUCCESS);

    // Calculate average play time
    ChessResult chess_result = CHESS_SUCCESS;
    assert(chessCalculateAveragePlayTime(chess2, 1001, NULL) == -10);
    assert(chessCalculateAveragePlayTime(chess2, 1001, &chess_result) == 90);
    assert(chess_result == CHESS_SUCCESS);
    assert(chessCalculateAveragePlayTime(chess2, 1002, &chess_result) == 220);
    assert(chess_result == CHESS_SUCCESS);
    assert(chessCalculateAveragePlayTime(chess2, 1010, &chess_result) > 4499.33);
    assert(chess_result == CHESS_SUCCESS);
    assert(chessCalculateAveragePlayTime(chess2, 1010, &chess_result) < 4499.34);
    assert(chess_result == CHESS_SUCCESS);
    assert(chessCalculateAveragePlayTime(chess2, 1013, &chess_result) == 4499);
    assert(chess_result == CHESS_SUCCESS);
    assert(chessEndTournament(chess2, 1) == CHESS_SUCCESS);
    assert(chessRemoveTournament(chess2, 1) == CHESS_SUCCESS);
    assert(chessCalculateAveragePlayTime(chess2, 1010, &chess_result) == 0);
    assert(chess_result == CHESS_SUCCESS);
    assert(chessSaveTournamentStatistics(chess2, "./output/actual/tournaments0.txt") == CHESS_NO_TOURNAMENTS_ENDED);
    

    // Choose the correct winner check1
    assert(chessAddGame(chess2, 2, 2001, 2011, DRAW, 200)   == CHESS_SUCCESS);
    assert(chessAddGame(chess2, 2, 2001, 2012, DRAW, 90)    == CHESS_SUCCESS);
    assert(chessAddGame(chess2, 2, 2001, 2013, DRAW, 90)    == CHESS_SUCCESS);
    assert(chessAddGame(chess2, 2, 2001, 2014, DRAW, 90)    == CHESS_SUCCESS);
    assert(chessAddGame(chess2, 2, 2001, 2015, DRAW, 90)    == CHESS_SUCCESS);
    assert(chessAddGame(chess2, 2, 2001, 2016, DRAW, 90)    == CHESS_SUCCESS);
    assert(chessAddGame(chess2, 2, 2001, 2017, DRAW, 90)    == CHESS_SUCCESS);
    assert(chessAddGame(chess2, 2, 2001, 2018, DRAW, 90)    == CHESS_SUCCESS);
    assert(chessAddGame(chess2, 2, 2001, 2019, DRAW, 90)    == CHESS_SUCCESS);

    assert(chessAddGame(chess2, 2, 2002, 2021, FIRST_PLAYER, 90)    == CHESS_SUCCESS);
    assert(chessAddGame(chess2, 2, 2002, 2022, FIRST_PLAYER, 90)    == CHESS_SUCCESS);
    assert(chessAddGame(chess2, 2, 2002, 2023, FIRST_PLAYER, 90)    == CHESS_SUCCESS);
    assert(chessAddGame(chess2, 2, 2002, 2024, FIRST_PLAYER, 90)    == CHESS_SUCCESS);
    assert(chessEndTournament(chess2, 2) == CHESS_SUCCESS); // Winner: 2001


    // Choose the correct winner check2
    assert(chessAddGame(chess2, 3, 3001, 3011, FIRST_PLAYER, 200)   == CHESS_SUCCESS);
    assert(chessAddGame(chess2, 3, 3001, 3012, FIRST_PLAYER, 200)   == CHESS_SUCCESS);
    assert(chessAddGame(chess2, 3, 3001, 3013, FIRST_PLAYER, 200)   == CHESS_SUCCESS);
    assert(chessAddGame(chess2, 3, 3001, 3014, SECOND_PLAYER, 200)  == CHESS_SUCCESS);
    assert(chessAddGame(chess2, 3, 3001, 3015, SECOND_PLAYER, 200)  == CHESS_SUCCESS);
    assert(chessAddGame(chess2, 3, 3001, 3016, SECOND_PLAYER, 200)  == CHESS_SUCCESS);
    assert(chessAddGame(chess2, 3, 3001, 3017, SECOND_PLAYER, 200)  == CHESS_SUCCESS);
    assert(chessAddGame(chess2, 3, 3001, 3018, SECOND_PLAYER, 200)  == CHESS_SUCCESS);

    assert(chessAddGame(chess2, 3, 3002, 3021, FIRST_PLAYER, 100)   == CHESS_SUCCESS);
    assert(chessAddGame(chess2, 3, 3002, 3022, FIRST_PLAYER, 100)   == CHESS_SUCCESS);
    assert(chessAddGame(chess2, 3, 3002, 3023, FIRST_PLAYER, 100)   == CHESS_SUCCESS);
    assert(chessAddGame(chess2, 3, 3002, 3024, SECOND_PLAYER, 100)  == CHESS_SUCCESS);
    assert(chessAddGame(chess2, 3, 3002, 3025, SECOND_PLAYER, 100)  == CHESS_SUCCESS);
    
    assert(chessAddGame(chess2, 3, 3031, 3032, FIRST_PLAYER, 100)  == CHESS_SUCCESS);
    assert(chessAddGame(chess2, 3, 3031, 3033, FIRST_PLAYER, 100)  == CHESS_SUCCESS);
    assert(chessAddGame(chess2, 3, 3034, 3032, SECOND_PLAYER, 100) == CHESS_SUCCESS);
    assert(chessAddGame(chess2, 3, 3035, 3034, DRAW, 100)          == CHESS_SUCCESS);
    assert(chessAddGame(chess2, 3, 3037, 3036, FIRST_PLAYER, 100)  == CHESS_SUCCESS);
    assert(chessAddGame(chess2, 3, 3039, 3037, FIRST_PLAYER, 103)  == CHESS_SUCCESS);
    assert(chessAddGame(chess2, 3, 3040, 3041, DRAW, 100)          == CHESS_SUCCESS);
    assert(chessAddGame(chess2, 3, 3038, 3041, FIRST_PLAYER, 100)  == CHESS_SUCCESS);

    assert(chessEndTournament(chess2, 3) == CHESS_SUCCESS); // Winner: 3002


    // Choose the correct winner check3
    assert(chessAddGame(chess2, 4, 4001, 4011, FIRST_PLAYER, 200)   == CHESS_SUCCESS);
    assert(chessAddGame(chess2, 4, 4001, 4012, FIRST_PLAYER, 200)   == CHESS_SUCCESS);
    assert(chessAddGame(chess2, 4, 4001, 4013, FIRST_PLAYER, 200)   == CHESS_SUCCESS);
    assert(chessAddGame(chess2, 4, 4001, 4014, DRAW,         200)   == CHESS_SUCCESS);
    assert(chessAddGame(chess2, 4, 4001, 4015, DRAW,         200)   == CHESS_SUCCESS);
    assert(chessAddGame(chess2, 4, 4001, 4016, SECOND_PLAYER, 200)  == CHESS_SUCCESS);

    assert(chessAddGame(chess2, 4, 4002, 4021, FIRST_PLAYER, 2)    == CHESS_SUCCESS);
    assert(chessAddGame(chess2, 4, 4002, 4022, FIRST_PLAYER, 2)    == CHESS_SUCCESS);
    assert(chessAddGame(chess2, 4, 4002, 4023, FIRST_PLAYER, 2)    == CHESS_SUCCESS);
    assert(chessAddGame(chess2, 4, 4002, 4024, FIRST_PLAYER, 2)    == CHESS_SUCCESS);
    assert(chessAddGame(chess2, 4, 4002, 4025, SECOND_PLAYER, 2)   == CHESS_SUCCESS);

    assert(chessEndTournament(chess2, 4) == CHESS_SUCCESS); // Winner: 4002


    // Choose the correct winner check4
    assert(chessAddGame(chess2, 5, 5001, 5011, FIRST_PLAYER, 200)   == CHESS_SUCCESS);
    assert(chessAddGame(chess2, 5, 5001, 5012, FIRST_PLAYER, 200)   == CHESS_SUCCESS);
    assert(chessAddGame(chess2, 5, 5001, 5013, FIRST_PLAYER, 200)   == CHESS_SUCCESS);
    assert(chessAddGame(chess2, 5, 5001, 5014, FIRST_PLAYER, 200)   == CHESS_SUCCESS);
    assert(chessAddGame(chess2, 5, 5001, 5015, FIRST_PLAYER, 200)   == CHESS_SUCCESS);

    assert(chessAddGame(chess2, 5, 5002, 5011, FIRST_PLAYER, 123)   == CHESS_SUCCESS);
    assert(chessAddGame(chess2, 5, 5002, 5012, FIRST_PLAYER, 321)   == CHESS_SUCCESS);
    assert(chessAddGame(chess2, 5, 5002, 5013, FIRST_PLAYER, 153)   == CHESS_SUCCESS);
    assert(chessAddGame(chess2, 5, 5002, 5014, FIRST_PLAYER, 187)   == CHESS_SUCCESS);
    assert(chessAddGame(chess2, 5, 5002, 5015, FIRST_PLAYER, 388)   == CHESS_SUCCESS);

    assert(chessEndTournament(chess2, 5) == CHESS_SUCCESS); // Winner: 5001

    assert(chessSaveTournamentStatistics(chess2, "./output/actual/tournaments5.txt") == CHESS_SUCCESS);
    chessDestroy(chess2);
    
    


    // RANDOM BULLSHIT GO
    ChessSystem chess3 = chessCreate();
    assert (chess3 != NULL);
    
    // Add Tournaments
    assert(chessAddTournament(chess3, 1, 10,  "Germany")  == CHESS_SUCCESS);
    assert(chessAddTournament(chess3, 2, 10, "Olman")   == CHESS_SUCCESS);
    assert(chessAddTournament(chess3, 3, 10, "Students house")   == CHESS_SUCCESS);
    assert(chessAddTournament(chess3, 4, 10, "Makefile is overrated") == CHESS_SUCCESS);
    assert(chessAddTournament(chess3, 5, 10, "Greece") == CHESS_SUCCESS);


    // Add games
    assert(chessAddGame(chess3, 2, 1005, 1023, DRAW, 108) == CHESS_SUCCESS);
    assert(chessAddGame(chess3, 3, 1018, 1019, SECOND_PLAYER, 227) == CHESS_SUCCESS);
    assert(chessAddGame(chess3, 1, 1009, 1026, FIRST_PLAYER, 891) == CHESS_SUCCESS);
    assert(chessAddGame(chess3, 3, 1020, 1007, FIRST_PLAYER, 537) == CHESS_SUCCESS);
    assert(chessAddGame(chess3, 1, 1013, 1007, FIRST_PLAYER, 483) == CHESS_SUCCESS);
    assert(chessAddGame(chess3, 3, 1014, 1030, DRAW, 456) == CHESS_SUCCESS);
    assert(chessAddGame(chess3, 1, 1019, 1013, SECOND_PLAYER, 857) == CHESS_SUCCESS);
    assert(chessAddGame(chess3, 4, 1009, 1020, SECOND_PLAYER, 510) == CHESS_SUCCESS);
    assert(chessAddGame(chess3, 3, 1005, 1006, DRAW, 870) == CHESS_SUCCESS);
    assert(chessAddGame(chess3, 5, 1014, 1024, DRAW, 155) == CHESS_SUCCESS);
    assert(chessAddGame(chess3, 2, 1001, 1036, FIRST_PLAYER, 425) == CHESS_SUCCESS);
    assert(chessAddGame(chess3, 3, 1001, 1028, DRAW, 164) == CHESS_SUCCESS);
    assert(chessAddGame(chess3, 1, 1015, 1007, FIRST_PLAYER, 826) == CHESS_SUCCESS);
    assert(chessAddGame(chess3, 2, 1010, 1034, SECOND_PLAYER, 754) == CHESS_SUCCESS);
    assert(chessAddGame(chess3, 5, 1011, 1003, DRAW, 693) == CHESS_SUCCESS);
    assert(chessAddGame(chess3, 2, 1005, 1019, DRAW, 963) == CHESS_SUCCESS);
    assert(chessAddGame(chess3, 4, 1006, 1009, FIRST_PLAYER, 893) == CHESS_SUCCESS);
    assert(chessAddGame(chess3, 4, 1003, 1013, FIRST_PLAYER, 830) == CHESS_SUCCESS);
    assert(chessAddGame(chess3, 5, 1016, 1019, FIRST_PLAYER, 281) == CHESS_SUCCESS);
    assert(chessAddGame(chess3, 5, 1020, 1033, DRAW, 489) == CHESS_SUCCESS);
    assert(chessAddGame(chess3, 5, 1016, 1030, SECOND_PLAYER, 968) == CHESS_SUCCESS);
    assert(chessAddGame(chess3, 3, 1006, 1009, SECOND_PLAYER, 980) == CHESS_SUCCESS);
    assert(chessAddGame(chess3, 4, 1009, 1036, FIRST_PLAYER, 358) == CHESS_SUCCESS);
    assert(chessAddGame(chess3, 4, 1013, 1010, DRAW, 882) == CHESS_SUCCESS);
    assert(chessAddGame(chess3, 3, 1015, 1007, SECOND_PLAYER, 358) == CHESS_SUCCESS);
    assert(chessAddGame(chess3, 4, 1006, 1009, SECOND_PLAYER, 650) == CHESS_GAME_ALREADY_EXISTS);
    assert(chessAddGame(chess3, 4, 1002, 1040, SECOND_PLAYER, 570) == CHESS_SUCCESS);
    assert(chessAddGame(chess3, 4, 1003, 1025, SECOND_PLAYER, 664) == CHESS_SUCCESS);
    assert(chessAddGame(chess3, 2, 1004, 1008, SECOND_PLAYER, 999) == CHESS_SUCCESS);
    assert(chessAddGame(chess3, 4, 1011, 1002, DRAW, 728) == CHESS_SUCCESS);
    assert(chessAddGame(chess3, 1, 1006, 1032, SECOND_PLAYER, 292) == CHESS_SUCCESS);
    assert(chessAddGame(chess3, 5, 1002, 1014, DRAW, 995) == CHESS_SUCCESS);
    assert(chessAddGame(chess3, 4, 1018, 1030, DRAW, 164) == CHESS_SUCCESS);
    assert(chessAddGame(chess3, 2, 1011, 1001, SECOND_PLAYER, 173) == CHESS_SUCCESS);
    assert(chessAddGame(chess3, 3, 1011, 1015, SECOND_PLAYER, 379) == CHESS_SUCCESS);
    assert(chessAddGame(chess3, 4, 1004, 1031, SECOND_PLAYER, 716) == CHESS_SUCCESS);
    assert(chessAddGame(chess3, 4, 1018, 1002, DRAW, 188) == CHESS_SUCCESS);
    assert(chessAddGame(chess3, 3, 1013, 1032, FIRST_PLAYER, 727) == CHESS_SUCCESS);
    assert(chessAddGame(chess3, 2, 1020, 1025, FIRST_PLAYER, 509) == CHESS_SUCCESS);
    assert(chessAddGame(chess3, 1, 1018, 1009, FIRST_PLAYER, 306) == CHESS_SUCCESS);
    assert(chessAddGame(chess3, 2, 1016, 1039, FIRST_PLAYER, 862) == CHESS_SUCCESS);
    assert(chessAddGame(chess3, 3, 1016, 1004, FIRST_PLAYER, 670) == CHESS_SUCCESS);
    assert(chessAddGame(chess3, 5, 1011, 1012, DRAW, 357) == CHESS_SUCCESS);
    assert(chessAddGame(chess3, 1, 1013, 1008, SECOND_PLAYER, 565) == CHESS_SUCCESS);
    assert(chessAddGame(chess3, 4, 1009, 1001, SECOND_PLAYER, 950) == CHESS_SUCCESS);
    assert(chessAddGame(chess3, 5, 1006, 1021, SECOND_PLAYER, 641) == CHESS_SUCCESS);
    assert(chessAddGame(chess3, 5, 1011, 1029, SECOND_PLAYER, 217) == CHESS_SUCCESS);
    assert(chessAddGame(chess3, 1, 1020, 1021, FIRST_PLAYER, 487) == CHESS_SUCCESS);
    assert(chessAddGame(chess3, 1, 1008, 1026, DRAW, 870) == CHESS_SUCCESS);
    assert(chessAddGame(chess3, 5, 1006, 1034, SECOND_PLAYER, 957) == CHESS_SUCCESS);
    assert(chessAddGame(chess3, 4, 1005, 1013, SECOND_PLAYER, 709) == CHESS_SUCCESS);
    assert(chessAddGame(chess3, 5, 1008, 1006, FIRST_PLAYER, 629) == CHESS_SUCCESS);
    assert(chessAddGame(chess3, 4, 1011, 1037, FIRST_PLAYER, 956) == CHESS_SUCCESS);
    assert(chessAddGame(chess3, 4, 1006, 1009, DRAW, 806) == CHESS_GAME_ALREADY_EXISTS);
    assert(chessAddGame(chess3, 1, 1007, 1037, DRAW, 498) == CHESS_SUCCESS);
    assert(chessAddGame(chess3, 3, 1002, 1008, DRAW, 666) == CHESS_SUCCESS);
    assert(chessAddGame(chess3, 1, 1020, 1019, DRAW, 998) == CHESS_SUCCESS);
    assert(chessAddGame(chess3, 1, 1005, 1022, FIRST_PLAYER, 849) == CHESS_SUCCESS);
    assert(chessAddGame(chess3, 4, 1018, 1018, DRAW, 132) == CHESS_INVALID_ID);
    assert(chessAddGame(chess3, 2, 1010, 1016, DRAW, 201) == CHESS_SUCCESS);
    assert(chessAddGame(chess3, 4, 1019, 1032, FIRST_PLAYER, 181) == CHESS_SUCCESS);
    assert(chessAddGame(chess3, 3, 1010, 1023, SECOND_PLAYER, 790) == CHESS_SUCCESS);
    assert(chessAddGame(chess3, 1, 1011, 1003, DRAW, 899) == CHESS_SUCCESS);
    assert(chessAddGame(chess3, 2, 1003, 1007, DRAW, 911) == CHESS_SUCCESS);
    assert(chessAddGame(chess3, 3, 1008, 1007, DRAW, 182) == CHESS_SUCCESS);
    assert(chessAddGame(chess3, 3, 1007, 1032, DRAW, 849) == CHESS_SUCCESS);
    assert(chessAddGame(chess3, 4, 1014, 1001, DRAW, 743) == CHESS_SUCCESS);
    assert(chessAddGame(chess3, 2, 1012, 1035, SECOND_PLAYER, 471) == CHESS_SUCCESS);
    assert(chessAddGame(chess3, 3, 1001, 1028, SECOND_PLAYER, 750) == CHESS_GAME_ALREADY_EXISTS);
    assert(chessAddGame(chess3, 1, 1008, 1029, SECOND_PLAYER, 321) == CHESS_SUCCESS);
    assert(chessAddGame(chess3, 4, 1014, 1011, FIRST_PLAYER, 485) == CHESS_SUCCESS);
    assert(chessAddGame(chess3, 1, 1003, 1030, DRAW, 355) == CHESS_SUCCESS);
    assert(chessAddGame(chess3, 2, 1004, 1033, DRAW, 569) == CHESS_SUCCESS);
    assert(chessAddGame(chess3, 2, 1010, 1009, DRAW, 328) == CHESS_SUCCESS);
    assert(chessAddGame(chess3, 3, 1018, 1004, FIRST_PLAYER, 430) == CHESS_SUCCESS);
    assert(chessAddGame(chess3, 1, 1015, 1022, DRAW, 292) == CHESS_SUCCESS);
    assert(chessAddGame(chess3, 5, 1004, 1023, SECOND_PLAYER, 750) == CHESS_SUCCESS);
    assert(chessAddGame(chess3, 3, 1013, 1040, SECOND_PLAYER, 530) == CHESS_SUCCESS);
    assert(chessAddGame(chess3, 4, 1015, 1038, FIRST_PLAYER, 673) == CHESS_SUCCESS);
    assert(chessAddGame(chess3, 2, 1014, 1035, FIRST_PLAYER, 222) == CHESS_SUCCESS);
    assert(chessAddGame(chess3, 1, 1017, 1013, SECOND_PLAYER, 708) == CHESS_SUCCESS);
    assert(chessAddGame(chess3, 1, 1007, 1039, DRAW, 765) == CHESS_SUCCESS);
    assert(chessAddGame(chess3, 1, 1003, 1037, FIRST_PLAYER, 721) == CHESS_SUCCESS);
    assert(chessAddGame(chess3, 3, 1017, 1007, SECOND_PLAYER, 472) == CHESS_SUCCESS);
    assert(chessAddGame(chess3, 5, 1006, 1032, SECOND_PLAYER, 469) == CHESS_SUCCESS);
    assert(chessAddGame(chess3, 3, 1002, 1001, DRAW, 131) == CHESS_SUCCESS);
    assert(chessAddGame(chess3, 5, 1009, 1017, DRAW, 442) == CHESS_SUCCESS);
    assert(chessAddGame(chess3, 1, 1008, 1024, FIRST_PLAYER, 425) == CHESS_SUCCESS);
    assert(chessAddGame(chess3, 4, 1010, 1019, FIRST_PLAYER, 210) == CHESS_SUCCESS);
    assert(chessAddGame(chess3, 1, 1015, 1005, FIRST_PLAYER, 335) == CHESS_SUCCESS);
    assert(chessAddGame(chess3, 4, 1004, 1030, SECOND_PLAYER, 217) == CHESS_SUCCESS);
    assert(chessAddGame(chess3, 1, 1015, 1028, DRAW, 435) == CHESS_SUCCESS);
    assert(chessAddGame(chess3, 5, 1009, 1040, FIRST_PLAYER, 802) == CHESS_SUCCESS);
    assert(chessAddGame(chess3, 3, 1007, 1014, SECOND_PLAYER, 449) == CHESS_SUCCESS);
    assert(chessAddGame(chess3, 4, 1017, 1011, SECOND_PLAYER, 964) == CHESS_SUCCESS);
    assert(chessAddGame(chess3, 1, 1005, 1033, SECOND_PLAYER, 575) == CHESS_SUCCESS);
    assert(chessAddGame(chess3, 3, 1006, 1033, DRAW, 109) == CHESS_SUCCESS);
    assert(chessAddGame(chess3, 1, 1001, 1009, SECOND_PLAYER, 302) == CHESS_SUCCESS);
    assert(chessAddGame(chess3, 3, 1001, 1006, FIRST_PLAYER, 383) == CHESS_SUCCESS);
    assert(chessAddGame(chess3, 5, 1014, 1022, SECOND_PLAYER, 519) == CHESS_SUCCESS);
    assert(chessAddGame(chess3, 5, 1003, 1003, DRAW, 521) == CHESS_INVALID_ID);
    assert(chessAddGame(chess3, 4, 1008, 1026, DRAW, 428) == CHESS_SUCCESS);
    assert(chessAddGame(chess3, 2, 1005, 1003, SECOND_PLAYER, 254) == CHESS_SUCCESS);
    assert(chessAddGame(chess3, 4, 1005, 1033, FIRST_PLAYER, 418) == CHESS_SUCCESS);
    assert(chessAddGame(chess3, 4, 1013, 1036, DRAW, 687) == CHESS_SUCCESS);
    assert(chessAddGame(chess3, 4, 1002, 1002, SECOND_PLAYER, 911) == CHESS_INVALID_ID);
    assert(chessAddGame(chess3, 2, 1013, 1019, SECOND_PLAYER, 555) == CHESS_SUCCESS);
    assert(chessAddGame(chess3, 2, 1005, 1021, SECOND_PLAYER, 394) == CHESS_SUCCESS);
    assert(chessAddGame(chess3, 3, 1006, 1031, FIRST_PLAYER, 781) == CHESS_SUCCESS);
    assert(chessAddGame(chess3, 5, 1006, 1013, DRAW, 746) == CHESS_SUCCESS);
    assert(chessAddGame(chess3, 5, 1006, 1030, FIRST_PLAYER, 396) == CHESS_SUCCESS);
    assert(chessAddGame(chess3, 5, 1005, 1034, FIRST_PLAYER, 425) == CHESS_SUCCESS);
    assert(chessAddGame(chess3, 5, 1001, 1016, FIRST_PLAYER, 220) == CHESS_SUCCESS);
    assert(chessAddGame(chess3, 4, 1006, 1004, DRAW, 738) == CHESS_SUCCESS);
    assert(chessAddGame(chess3, 1, 1017, 1019, DRAW, 272) == CHESS_SUCCESS);
    assert(chessAddGame(chess3, 2, 1012, 1039, FIRST_PLAYER, 973) == CHESS_SUCCESS);
    assert(chessAddGame(chess3, 2, 1002, 1017, FIRST_PLAYER, 444) == CHESS_SUCCESS);
    assert(chessAddGame(chess3, 1, 1007, 1032, DRAW, 207) == CHESS_SUCCESS);
    assert(chessAddGame(chess3, 4, 1010, 1001, SECOND_PLAYER, 595) == CHESS_SUCCESS);
    assert(chessAddGame(chess3, 4, 1008, 1013, FIRST_PLAYER, 501) == CHESS_SUCCESS);
    assert(chessAddGame(chess3, 2, 1011, 1002, FIRST_PLAYER, 623) == CHESS_SUCCESS);
    assert(chessAddGame(chess3, 2, 1011, 1032, DRAW, 474) == CHESS_SUCCESS);
    assert(chessAddGame(chess3, 3, 1001, 1001, DRAW, 855) == CHESS_INVALID_ID);
    assert(chessAddGame(chess3, 5, 1014, 1038, SECOND_PLAYER, 207) == CHESS_SUCCESS);
    assert(chessAddGame(chess3, 4, 1011, 1013, DRAW, 396) == CHESS_SUCCESS);
    assert(chessAddGame(chess3, 5, 1010, 1032, SECOND_PLAYER, 643) == CHESS_SUCCESS);
    assert(chessAddGame(chess3, 1, 1018, 1012, DRAW, 102) == CHESS_SUCCESS);
    assert(chessAddGame(chess3, 2, 1007, 1035, DRAW, 516) == CHESS_SUCCESS);
    assert(chessAddGame(chess3, 4, 1014, 1015, FIRST_PLAYER, 576) == CHESS_SUCCESS);
    assert(chessAddGame(chess3, 4, 1005, 1035, SECOND_PLAYER, 783) == CHESS_SUCCESS);
    assert(chessAddGame(chess3, 4, 1017, 1026, FIRST_PLAYER, 431) == CHESS_SUCCESS);
    assert(chessAddGame(chess3, 3, 1001, 1025, SECOND_PLAYER, 624) == CHESS_SUCCESS);
    assert(chessAddGame(chess3, 5, 1001, 1008, SECOND_PLAYER, 813) == CHESS_SUCCESS);
    assert(chessAddGame(chess3, 3, 1018, 1039, FIRST_PLAYER, 118) == CHESS_SUCCESS);
    assert(chessAddGame(chess3, 3, 1005, 1023, DRAW, 152) == CHESS_SUCCESS);
    assert(chessAddGame(chess3, 3, 1008, 1032, SECOND_PLAYER, 107) == CHESS_SUCCESS);
    assert(chessAddGame(chess3, 2, 1008, 1010, DRAW, 432) == CHESS_SUCCESS);
    assert(chessAddGame(chess3, 3, 1009, 1023, DRAW, 476) == CHESS_SUCCESS);
    assert(chessAddGame(chess3, 3, 1012, 1015, FIRST_PLAYER, 152) == CHESS_SUCCESS);
    assert(chessAddGame(chess3, 2, 1015, 1038, FIRST_PLAYER, 588) == CHESS_SUCCESS);
    assert(chessAddGame(chess3, 1, 1020, 1022, DRAW, 884) == CHESS_SUCCESS);
    assert(chessAddGame(chess3, 4, 1019, 1019, DRAW, 827) == CHESS_INVALID_ID);
    assert(chessAddGame(chess3, 5, 1001, 1025, FIRST_PLAYER, 252) == CHESS_SUCCESS);
    assert(chessAddGame(chess3, 4, 1018, 1025, SECOND_PLAYER, 871) == CHESS_SUCCESS);
    assert(chessAddGame(chess3, 1, 1020, 1009, FIRST_PLAYER, 285) == CHESS_SUCCESS);
    assert(chessAddGame(chess3, 1, 1013, 1040, SECOND_PLAYER, 754) == CHESS_SUCCESS);
    assert(chessAddGame(chess3, 3, 1006, 1009, FIRST_PLAYER, 129) == CHESS_GAME_ALREADY_EXISTS);
    assert(chessAddGame(chess3, 1, 1013, 1017, FIRST_PLAYER, 584) == CHESS_GAME_ALREADY_EXISTS);
    assert(chessAddGame(chess3, 5, 1002, 1019, SECOND_PLAYER, 398) == CHESS_SUCCESS);
    assert(chessAddGame(chess3, 4, 1016, 1001, FIRST_PLAYER, 471) == CHESS_SUCCESS);
    assert(chessAddGame(chess3, 1, 1002, 1014, DRAW, 607) == CHESS_SUCCESS);
    assert(chessAddGame(chess3, 5, 1012, 1025, DRAW, 261) == CHESS_SUCCESS);
    assert(chessAddGame(chess3, 2, 1006, 1033, FIRST_PLAYER, 406) == CHESS_SUCCESS);
    assert(chessAddGame(chess3, 5, 1007, 1024, SECOND_PLAYER, 654) == CHESS_SUCCESS);
    assert(chessAddGame(chess3, 4, 1016, 1033, DRAW, 549) == CHESS_SUCCESS);
    assert(chessAddGame(chess3, 5, 1017, 1008, DRAW, 268) == CHESS_SUCCESS);
    assert(chessAddGame(chess3, 5, 1020, 1020, SECOND_PLAYER, 166) == CHESS_INVALID_ID);
    assert(chessAddGame(chess3, 4, 1016, 1016, DRAW, 651) == CHESS_INVALID_ID);
    assert(chessAddGame(chess3, 4, 1013, 1031, DRAW, 440) == CHESS_SUCCESS);
    assert(chessAddGame(chess3, 1, 1012, 1011, FIRST_PLAYER, 706) == CHESS_SUCCESS);
    assert(chessAddGame(chess3, 5, 1001, 1032, FIRST_PLAYER, 752) == CHESS_SUCCESS);
    assert(chessAddGame(chess3, 3, 1014, 1026, SECOND_PLAYER, 841) == CHESS_SUCCESS);
    assert(chessAddGame(chess3, 1, 1009, 1030, FIRST_PLAYER, 142) == CHESS_SUCCESS);
    assert(chessAddGame(chess3, 4, 1003, 1023, FIRST_PLAYER, 354) == CHESS_SUCCESS);
    assert(chessAddGame(chess3, 2, 1020, 1023, DRAW, 717) == CHESS_SUCCESS);
    assert(chessAddGame(chess3, 1, 1019, 1011, FIRST_PLAYER, 724) == CHESS_SUCCESS);
    assert(chessAddGame(chess3, 4, 1001, 1002, SECOND_PLAYER, 751) == CHESS_SUCCESS);
    assert(chessAddGame(chess3, 1, 1007, 1014, SECOND_PLAYER, 224) == CHESS_SUCCESS);
    assert(chessAddGame(chess3, 5, 1010, 1015, SECOND_PLAYER, 859) == CHESS_SUCCESS);
    assert(chessAddGame(chess3, 4, 1001, 1029, FIRST_PLAYER, 204) == CHESS_SUCCESS);
    assert(chessAddGame(chess3, 2, 1018, 1038, DRAW, 887) == CHESS_SUCCESS);
    assert(chessAddGame(chess3, 1, 1006, 1009, DRAW, 735) == CHESS_SUCCESS);
    assert(chessAddGame(chess3, 5, 1007, 1022, SECOND_PLAYER, 261) == CHESS_SUCCESS);
    assert(chessAddGame(chess3, 5, 1010, 1038, SECOND_PLAYER, 780) == CHESS_SUCCESS);
    assert(chessAddGame(chess3, 3, 1019, 1018, SECOND_PLAYER, 327) == CHESS_GAME_ALREADY_EXISTS);
    assert(chessAddGame(chess3, 5, 1013, 1023, DRAW, 209) == CHESS_SUCCESS);
    assert(chessAddGame(chess3, 2, 1011, 1020, DRAW, 688) == CHESS_SUCCESS);
    assert(chessAddGame(chess3, 5, 1001, 1033, SECOND_PLAYER, 902) == CHESS_SUCCESS);
    assert(chessAddGame(chess3, 5, 1011, 1034, SECOND_PLAYER, 698) == CHESS_SUCCESS);
    assert(chessAddGame(chess3, 3, 1008, 1010, FIRST_PLAYER, 397) == CHESS_SUCCESS);
    assert(chessAddGame(chess3, 3, 1019, 1036, DRAW, 287) == CHESS_SUCCESS);
    assert(chessAddGame(chess3, 4, 1003, 1020, DRAW, 648) == CHESS_SUCCESS);
    assert(chessAddGame(chess3, 4, 1005, 1031, SECOND_PLAYER, 462) == CHESS_SUCCESS);
    assert(chessAddGame(chess3, 3, 1013, 1037, FIRST_PLAYER, 633) == CHESS_SUCCESS);
    assert(chessAddGame(chess3, 2, 1015, 1007, DRAW, 789) == CHESS_SUCCESS);
    assert(chessAddGame(chess3, 3, 1016, 1009, DRAW, 241) == CHESS_SUCCESS);
    assert(chessAddGame(chess3, 3, 1006, 1040, FIRST_PLAYER, 423) == CHESS_SUCCESS);
    assert(chessAddGame(chess3, 1, 1016, 1040, DRAW, 777) == CHESS_SUCCESS);
    assert(chessAddGame(chess3, 5, 1015, 1023, FIRST_PLAYER, 752) == CHESS_SUCCESS);
    assert(chessAddGame(chess3, 5, 1009, 1017, SECOND_PLAYER, 207) == CHESS_GAME_ALREADY_EXISTS);
    assert(chessAddGame(chess3, 1, 1011, 1038, FIRST_PLAYER, 663) == CHESS_SUCCESS);
    assert(chessAddGame(chess3, 1, 1006, 1028, SECOND_PLAYER, 642) == CHESS_SUCCESS);
    assert(chessAddGame(chess3, 4, 1017, 1037, SECOND_PLAYER, 902) == CHESS_SUCCESS);
    assert(chessAddGame(chess3, 2, 1004, 1040, DRAW, 969) == CHESS_SUCCESS);
    assert(chessAddGame(chess3, 1, 1017, 1032, DRAW, 574) == CHESS_SUCCESS);
    assert(chessAddGame(chess3, 5, 1017, 1031, FIRST_PLAYER, 557) == CHESS_SUCCESS);
    assert(chessAddGame(chess3, 2, 1019, 1029, SECOND_PLAYER, 879) == CHESS_SUCCESS);
    assert(chessAddGame(chess3, 2, 1018, 1019, SECOND_PLAYER, 817) == CHESS_SUCCESS);
    assert(chessAddGame(chess3, 1, 1019, 1040, SECOND_PLAYER, 417) == CHESS_SUCCESS);
    assert(chessAddGame(chess3, 5, 1003, 1024, DRAW, 911) == CHESS_SUCCESS);
    assert(chessAddGame(chess3, 1, 1014, 1017, FIRST_PLAYER, 879) == CHESS_SUCCESS);
    assert(chessAddGame(chess3, 3, 1018, 1013, FIRST_PLAYER, 249) == CHESS_SUCCESS);
    assert(chessAddGame(chess3, 4, 1007, 1005, DRAW, 401) == CHESS_SUCCESS);
    assert(chessAddGame(chess3, 1, 1018, 1017, SECOND_PLAYER, 920) == CHESS_SUCCESS);
    assert(chessAddGame(chess3, 2, 1007, 1033, SECOND_PLAYER, 647) == CHESS_SUCCESS);
    assert(chessAddGame(chess3, 2, 1006, 1018, SECOND_PLAYER, 357) == CHESS_SUCCESS);
    assert(chessAddGame(chess3, 3, 1002, 1017, SECOND_PLAYER, 100) == CHESS_SUCCESS);
    assert(chessAddGame(chess3, 5, 1013, 1032, DRAW, 425) == CHESS_SUCCESS);
    assert(chessAddGame(chess3, 2, 1018, 1036, FIRST_PLAYER, 769) == CHESS_SUCCESS);
    assert(chessAddGame(chess3, 5, 1012, 1019, SECOND_PLAYER, 941) == CHESS_SUCCESS);
    assert(chessAddGame(chess3, 3, 1015, 1004, FIRST_PLAYER, 172) == CHESS_SUCCESS);
    assert(chessAddGame(chess3, 1, 1015, 1024, SECOND_PLAYER, 877) == CHESS_SUCCESS);
    assert(chessAddGame(chess3, 1, 1019, 1016, SECOND_PLAYER, 414) == CHESS_SUCCESS);
    assert(chessAddGame(chess3, 2, 1019, 1021, FIRST_PLAYER, 918) == CHESS_SUCCESS);
    assert(chessAddGame(chess3, 1, 1006, 1017, FIRST_PLAYER, 460) == CHESS_SUCCESS);
    assert(chessAddGame(chess3, 5, 1017, 1038, SECOND_PLAYER, 562) == CHESS_SUCCESS);
    assert(chessAddGame(chess3, 4, 1008, 1025, SECOND_PLAYER, 354) == CHESS_SUCCESS);
    assert(chessAddGame(chess3, 2, 1015, 1017, SECOND_PLAYER, 186) == CHESS_SUCCESS);
    assert(chessAddGame(chess3, 3, 1002, 1018, DRAW, 786) == CHESS_SUCCESS);
    assert(chessAddGame(chess3, 1, 1005, 1012, SECOND_PLAYER, 305) == CHESS_SUCCESS);
    assert(chessAddGame(chess3, 2, 1019, 1024, FIRST_PLAYER, 555) == CHESS_SUCCESS);
    assert(chessAddGame(chess3, 1, 1003, 1020, SECOND_PLAYER, 842) == CHESS_SUCCESS);
    assert(chessAddGame(chess3, 3, 1015, 1020, FIRST_PLAYER, 609) == CHESS_SUCCESS);
    assert(chessAddGame(chess3, 1, 1008, 1007, FIRST_PLAYER, 711) == CHESS_SUCCESS);
    assert(chessAddGame(chess3, 3, 1011, 1019, SECOND_PLAYER, 813) == CHESS_SUCCESS);
    assert(chessAddGame(chess3, 1, 1012, 1002, FIRST_PLAYER, 714) == CHESS_SUCCESS);
    assert(chessAddGame(chess3, 4, 1003, 1028, FIRST_PLAYER, 865) == CHESS_SUCCESS);
    assert(chessAddGame(chess3, 3, 1020, 1022, SECOND_PLAYER, 481) == CHESS_SUCCESS);
    assert(chessAddGame(chess3, 2, 1011, 1003, FIRST_PLAYER, 519) == CHESS_SUCCESS);
    assert(chessAddGame(chess3, 3, 1012, 1002, DRAW, 703) == CHESS_SUCCESS);
    assert(chessAddGame(chess3, 5, 1013, 1025, DRAW, 195) == CHESS_SUCCESS);
    assert(chessAddGame(chess3, 4, 1015, 1024, SECOND_PLAYER, 259) == CHESS_SUCCESS);
    assert(chessAddGame(chess3, 2, 1005, 1030, DRAW, 116) == CHESS_SUCCESS);
    assert(chessAddGame(chess3, 2, 1015, 1001, SECOND_PLAYER, 872) == CHESS_SUCCESS);
    assert(chessAddGame(chess3, 3, 1014, 1013, FIRST_PLAYER, 636) == CHESS_SUCCESS);
    assert(chessAddGame(chess3, 1, 1005, 1002, SECOND_PLAYER, 806) == CHESS_SUCCESS);
    assert(chessAddGame(chess3, 4, 1010, 1026, SECOND_PLAYER, 905) == CHESS_SUCCESS);
    assert(chessAddGame(chess3, 5, 1016, 1036, DRAW, 226) == CHESS_SUCCESS);
    assert(chessAddGame(chess3, 3, 1014, 1035, FIRST_PLAYER, 316) == CHESS_SUCCESS);
    assert(chessAddGame(chess3, 2, 1002, 1013, DRAW, 691) == CHESS_SUCCESS);
    assert(chessAddGame(chess3, 5, 1004, 1023, DRAW, 122) == CHESS_GAME_ALREADY_EXISTS);
    assert(chessAddGame(chess3, 4, 1019, 1019, SECOND_PLAYER, 480) == CHESS_INVALID_ID);
    assert(chessAddGame(chess3, 5, 1019, 1031, SECOND_PLAYER, 291) == CHESS_SUCCESS);
    assert(chessAddGame(chess3, 1, 1009, 1027, SECOND_PLAYER, 699) == CHESS_SUCCESS);
    assert(chessAddGame(chess3, 5, 1004, 1030, FIRST_PLAYER, 850) == CHESS_SUCCESS);
    assert(chessAddGame(chess3, 5, 1018, 1005, FIRST_PLAYER, 494) == CHESS_SUCCESS);
    assert(chessAddGame(chess3, 1, 1005, 1014, DRAW, 207) == CHESS_SUCCESS);
    assert(chessAddGame(chess3, 4, 1017, 1022, FIRST_PLAYER, 627) == CHESS_SUCCESS);
    assert(chessAddGame(chess3, 5, 1016, 1012, SECOND_PLAYER, 355) == CHESS_SUCCESS);
    assert(chessAddGame(chess3, 5, 1001, 1039, SECOND_PLAYER, 381) == CHESS_SUCCESS);
    assert(chessAddGame(chess3, 4, 1001, 1006, FIRST_PLAYER, 941) == CHESS_SUCCESS);
    assert(chessAddGame(chess3, 3, 1011, 1030, FIRST_PLAYER, 704) == CHESS_SUCCESS);
    assert(chessAddGame(chess3, 2, 1012, 1032, FIRST_PLAYER, 296) == CHESS_SUCCESS);
    assert(chessAddGame(chess3, 2, 1006, 1008, FIRST_PLAYER, 874) == CHESS_SUCCESS);
    assert(chessAddGame(chess3, 3, 1019, 1010, FIRST_PLAYER, 954) == CHESS_SUCCESS);
    assert(chessAddGame(chess3, 1, 1006, 1016, DRAW, 881) == CHESS_SUCCESS);
    assert(chessAddGame(chess3, 3, 1017, 1030, FIRST_PLAYER, 487) == CHESS_SUCCESS);
    assert(chessAddGame(chess3, 2, 1017, 1024, FIRST_PLAYER, 823) == CHESS_SUCCESS);
    assert(chessAddGame(chess3, 1, 1004, 1030, SECOND_PLAYER, 570) == CHESS_SUCCESS);
    assert(chessAddGame(chess3, 2, 1013, 1001, SECOND_PLAYER, 551) == CHESS_SUCCESS);
    assert(chessAddGame(chess3, 2, 1004, 1016, FIRST_PLAYER, 722) == CHESS_SUCCESS);
    assert(chessAddGame(chess3, 5, 1017, 1022, FIRST_PLAYER, 583) == CHESS_SUCCESS);
    assert(chessAddGame(chess3, 1, 1013, 1036, SECOND_PLAYER, 620) == CHESS_SUCCESS);
    assert(chessAddGame(chess3, 1, 1009, 1037, FIRST_PLAYER, 838) == CHESS_SUCCESS);
    assert(chessAddGame(chess3, 4, 1009, 1036, FIRST_PLAYER, 339) == CHESS_GAME_ALREADY_EXISTS);
    assert(chessAddGame(chess3, 5, 1004, 1010, FIRST_PLAYER, 675) == CHESS_SUCCESS);
    assert(chessAddGame(chess3, 1, 1012, 1038, DRAW, 179) == CHESS_SUCCESS);
    assert(chessAddGame(chess3, 3, 1005, 1007, FIRST_PLAYER, 277) == CHESS_SUCCESS);
    assert(chessAddGame(chess3, 2, 1002, 1022, SECOND_PLAYER, 975) == CHESS_SUCCESS);
    assert(chessAddGame(chess3, 3, 1009, 1029, DRAW, 396) == CHESS_SUCCESS);
    assert(chessAddGame(chess3, 4, 1005, 1035, DRAW, 750) == CHESS_GAME_ALREADY_EXISTS);
    assert(chessAddGame(chess3, 1, 1009, 1003, FIRST_PLAYER, 532) == CHESS_SUCCESS);
    assert(chessAddGame(chess3, 2, 1012, 1020, SECOND_PLAYER, 787) == CHESS_SUCCESS);
    assert(chessAddGame(chess3, 1, 1007, 1013, DRAW, 827) == CHESS_GAME_ALREADY_EXISTS);
    assert(chessAddGame(chess3, 3, 1015, 1005, FIRST_PLAYER, 578) == CHESS_SUCCESS);
    assert(chessAddGame(chess3, 1, 1010, 1035, DRAW, 351) == CHESS_SUCCESS);
    assert(chessAddGame(chess3, 3, 1016, 1002, FIRST_PLAYER, 115) == CHESS_SUCCESS);
    assert(chessAddGame(chess3, 4, 1005, 1028, DRAW, 654) == CHESS_SUCCESS);
    assert(chessAddGame(chess3, 4, 1007, 1015, FIRST_PLAYER, 309) == CHESS_SUCCESS);
    assert(chessAddGame(chess3, 3, 1019, 1038, FIRST_PLAYER, 766) == CHESS_SUCCESS);
    assert(chessAddGame(chess3, 3, 1010, 1026, SECOND_PLAYER, 841) == CHESS_SUCCESS);
    assert(chessAddGame(chess3, 1, 1015, 1015, SECOND_PLAYER, 517) == CHESS_INVALID_ID);
    assert(chessAddGame(chess3, 3, 1001, 1002, DRAW, 194) == CHESS_GAME_ALREADY_EXISTS);
    assert(chessAddGame(chess3, 3, 1003, 1037, FIRST_PLAYER, 182) == CHESS_SUCCESS);
    assert(chessAddGame(chess3, 4, 1007, 1020, DRAW, 186) == CHESS_SUCCESS);
    assert(chessAddGame(chess3, 2, 1002, 1012, FIRST_PLAYER, 821) == CHESS_SUCCESS);
    assert(chessAddGame(chess3, 5, 1016, 1015, SECOND_PLAYER, 322) == CHESS_SUCCESS);
    assert(chessAddGame(chess3, 1, 1001, 1037, SECOND_PLAYER, 948) == CHESS_SUCCESS);
    assert(chessAddGame(chess3, 2, 1017, 1040, FIRST_PLAYER, 175) == CHESS_SUCCESS);
    assert(chessAddGame(chess3, 5, 1007, 1016, FIRST_PLAYER, 925) == CHESS_SUCCESS);
    assert(chessAddGame(chess3, 3, 1017, 1011, DRAW, 829) == CHESS_SUCCESS);
    assert(chessAddGame(chess3, 4, 1014, 1030, FIRST_PLAYER, 370) == CHESS_SUCCESS);
    assert(chessAddGame(chess3, 4, 1016, 1013, FIRST_PLAYER, 756) == CHESS_SUCCESS);
    assert(chessAddGame(chess3, 1, 1002, 1003, DRAW, 194) == CHESS_SUCCESS);
    assert(chessAddGame(chess3, 4, 1009, 1033, FIRST_PLAYER, 867) == CHESS_SUCCESS);
    assert(chessAddGame(chess3, 3, 1005, 1014, DRAW, 354) == CHESS_SUCCESS);
    assert(chessAddGame(chess3, 2, 1011, 1030, SECOND_PLAYER, 910) == CHESS_SUCCESS);
    assert(chessAddGame(chess3, 1, 1005, 1011, DRAW, 999) == CHESS_SUCCESS);
    assert(chessAddGame(chess3, 1, 1005, 1033, SECOND_PLAYER, 866) == CHESS_GAME_ALREADY_EXISTS);
    assert(chessAddGame(chess3, 3, 1015, 1035, FIRST_PLAYER, 393) == CHESS_SUCCESS);
    assert(chessAddGame(chess3, 5, 1016, 1009, DRAW, 607) == CHESS_SUCCESS);

    assert(chessEndTournament(chess3, 1) == CHESS_SUCCESS);
    assert(chessEndTournament(chess3, 2) == CHESS_SUCCESS);
    assert(chessEndTournament(chess3, 3) == CHESS_SUCCESS);
    assert(chessEndTournament(chess3, 4) == CHESS_SUCCESS);
    assert(chessEndTournament(chess3, 5) == CHESS_SUCCESS);

    file = fopen("./output/actual/levels6.txt", "w");
    assert(chessSavePlayersLevels(chess3, file) == CHESS_SUCCESS);
    fclose(file);
    assert(chessSaveTournamentStatistics(chess3, "./output/actual/tournaments6.txt") == CHESS_SUCCESS);
    chessDestroy(chess3);


    // RANDOM BULLSHIT GO
    ChessSystem chess4 = chessCreate();
    assert (chess4 != NULL);
    
    // Add Tournaments
    assert(chessAddTournament(chess4, 1, 10,  "Germany")  == CHESS_SUCCESS);
    assert(chessAddTournament(chess4, 2, 10, "Olman")   == CHESS_SUCCESS);
    assert(chessAddTournament(chess4, 3, 10, "Students house")   == CHESS_SUCCESS);
    assert(chessAddTournament(chess4, 4, 10, "Makefile is overrated") == CHESS_SUCCESS);
    assert(chessAddTournament(chess4, 5, 10, "Greece") == CHESS_SUCCESS);


    // Add games
    assert(chessAddGame(chess4, 2, 1005, 1023, DRAW, 108) == CHESS_SUCCESS);
    assert(chessAddGame(chess4, 3, 1018, 1019, SECOND_PLAYER, 227) == CHESS_SUCCESS);
    assert(chessAddGame(chess4, 1, 1009, 1026, FIRST_PLAYER, 891) == CHESS_SUCCESS);
    assert(chessAddGame(chess4, 3, 1020, 1007, FIRST_PLAYER, 537) == CHESS_SUCCESS);
    assert(chessAddGame(chess4, 1, 1013, 1007, FIRST_PLAYER, 483) == CHESS_SUCCESS);
    assert(chessAddGame(chess4, 3, 1014, 1030, DRAW, 456) == CHESS_SUCCESS);
    assert(chessAddGame(chess4, 1, 1019, 1013, SECOND_PLAYER, 857) == CHESS_SUCCESS);
    assert(chessAddGame(chess4, 4, 1009, 1020, SECOND_PLAYER, 510) == CHESS_SUCCESS);
    assert(chessAddGame(chess4, 3, 1005, 1006, DRAW, 870) == CHESS_SUCCESS);
    assert(chessAddGame(chess4, 5, 1014, 1024, DRAW, 155) == CHESS_SUCCESS);
    assert(chessAddGame(chess4, 2, 1001, 1036, FIRST_PLAYER, 425) == CHESS_SUCCESS);
    assert(chessAddGame(chess4, 3, 1001, 1028, DRAW, 164) == CHESS_SUCCESS);
    assert(chessAddGame(chess4, 1, 1015, 1007, FIRST_PLAYER, 826) == CHESS_SUCCESS);
    assert(chessAddGame(chess4, 2, 1010, 1034, SECOND_PLAYER, 754) == CHESS_SUCCESS);
    assert(chessAddGame(chess4, 5, 1011, 1003, DRAW, 693) == CHESS_SUCCESS);
    assert(chessAddGame(chess4, 2, 1005, 1019, DRAW, 963) == CHESS_SUCCESS);
    assert(chessAddGame(chess4, 4, 1006, 1009, FIRST_PLAYER, 893) == CHESS_SUCCESS);
    assert(chessAddGame(chess4, 4, 1003, 1013, FIRST_PLAYER, 830) == CHESS_SUCCESS);
    assert(chessAddGame(chess4, 5, 1016, 1019, FIRST_PLAYER, 281) == CHESS_SUCCESS);
    assert(chessAddGame(chess4, 5, 1020, 1033, DRAW, 489) == CHESS_SUCCESS);
    assert(chessAddGame(chess4, 5, 1016, 1030, SECOND_PLAYER, 968) == CHESS_SUCCESS);
    assert(chessAddGame(chess4, 3, 1006, 1009, SECOND_PLAYER, 980) == CHESS_SUCCESS);
    assert(chessAddGame(chess4, 4, 1009, 1036, FIRST_PLAYER, 358) == CHESS_SUCCESS);
    assert(chessAddGame(chess4, 4, 1013, 1010, DRAW, 882) == CHESS_SUCCESS);
    assert(chessAddGame(chess4, 3, 1015, 1007, SECOND_PLAYER, 358) == CHESS_SUCCESS);
    assert(chessAddGame(chess4, 4, 1006, 1009, SECOND_PLAYER, 650) == CHESS_GAME_ALREADY_EXISTS);
    assert(chessAddGame(chess4, 4, 1002, 1040, SECOND_PLAYER, 570) == CHESS_SUCCESS);
    assert(chessAddGame(chess4, 4, 1003, 1025, SECOND_PLAYER, 664) == CHESS_SUCCESS);
    assert(chessAddGame(chess4, 2, 1004, 1008, SECOND_PLAYER, 999) == CHESS_SUCCESS);
    assert(chessAddGame(chess4, 4, 1011, 1002, DRAW, 728) == CHESS_SUCCESS);
    assert(chessAddGame(chess4, 1, 1006, 1032, SECOND_PLAYER, 292) == CHESS_SUCCESS);
    assert(chessAddGame(chess4, 5, 1002, 1014, DRAW, 995) == CHESS_SUCCESS);
    assert(chessAddGame(chess4, 4, 1018, 1030, DRAW, 164) == CHESS_SUCCESS);
    assert(chessAddGame(chess4, 2, 1011, 1001, SECOND_PLAYER, 173) == CHESS_SUCCESS);
    assert(chessAddGame(chess4, 3, 1011, 1015, SECOND_PLAYER, 379) == CHESS_SUCCESS);
    assert(chessAddGame(chess4, 4, 1004, 1031, SECOND_PLAYER, 716) == CHESS_SUCCESS);
    assert(chessAddGame(chess4, 4, 1018, 1002, DRAW, 188) == CHESS_SUCCESS);
    assert(chessAddGame(chess4, 3, 1013, 1032, FIRST_PLAYER, 727) == CHESS_SUCCESS);
    assert(chessAddGame(chess4, 2, 1020, 1025, FIRST_PLAYER, 509) == CHESS_SUCCESS);
    assert(chessAddGame(chess4, 1, 1018, 1009, FIRST_PLAYER, 306) == CHESS_SUCCESS);
    assert(chessAddGame(chess4, 2, 1016, 1039, FIRST_PLAYER, 862) == CHESS_SUCCESS);
    assert(chessAddGame(chess4, 3, 1016, 1004, FIRST_PLAYER, 670) == CHESS_SUCCESS);
    assert(chessAddGame(chess4, 5, 1011, 1012, DRAW, 357) == CHESS_SUCCESS);
    assert(chessAddGame(chess4, 1, 1013, 1008, SECOND_PLAYER, 565) == CHESS_SUCCESS);
    assert(chessAddGame(chess4, 4, 1009, 1001, SECOND_PLAYER, 950) == CHESS_SUCCESS);
    assert(chessAddGame(chess4, 5, 1006, 1021, SECOND_PLAYER, 641) == CHESS_SUCCESS);
    assert(chessAddGame(chess4, 5, 1011, 1029, SECOND_PLAYER, 217) == CHESS_SUCCESS);
    assert(chessAddGame(chess4, 1, 1020, 1021, FIRST_PLAYER, 487) == CHESS_SUCCESS);
    assert(chessAddGame(chess4, 1, 1008, 1026, DRAW, 870) == CHESS_SUCCESS);
    assert(chessAddGame(chess4, 5, 1006, 1034, SECOND_PLAYER, 957) == CHESS_SUCCESS);
    assert(chessAddGame(chess4, 4, 1005, 1013, SECOND_PLAYER, 709) == CHESS_SUCCESS);
    assert(chessAddGame(chess4, 5, 1008, 1006, FIRST_PLAYER, 629) == CHESS_SUCCESS);
    assert(chessAddGame(chess4, 4, 1011, 1037, FIRST_PLAYER, 956) == CHESS_SUCCESS);
    assert(chessAddGame(chess4, 4, 1006, 1009, DRAW, 806) == CHESS_GAME_ALREADY_EXISTS);
    assert(chessAddGame(chess4, 1, 1007, 1037, DRAW, 498) == CHESS_SUCCESS);
    assert(chessAddGame(chess4, 3, 1002, 1008, DRAW, 666) == CHESS_SUCCESS);
    assert(chessAddGame(chess4, 1, 1020, 1019, DRAW, 998) == CHESS_SUCCESS);
    assert(chessAddGame(chess4, 1, 1005, 1022, FIRST_PLAYER, 849) == CHESS_SUCCESS);
    assert(chessAddGame(chess4, 4, 1018, 1018, DRAW, 132) == CHESS_INVALID_ID);
    assert(chessAddGame(chess4, 2, 1010, 1016, DRAW, 201) == CHESS_SUCCESS);
    assert(chessAddGame(chess4, 4, 1019, 1032, FIRST_PLAYER, 181) == CHESS_SUCCESS);
    assert(chessAddGame(chess4, 3, 1010, 1023, SECOND_PLAYER, 790) == CHESS_SUCCESS);
    assert(chessAddGame(chess4, 1, 1011, 1003, DRAW, 899) == CHESS_SUCCESS);
    assert(chessAddGame(chess4, 2, 1003, 1007, DRAW, 911) == CHESS_SUCCESS);
    assert(chessAddGame(chess4, 3, 1008, 1007, DRAW, 182) == CHESS_SUCCESS);
    assert(chessAddGame(chess4, 3, 1007, 1032, DRAW, 849) == CHESS_SUCCESS);
    assert(chessAddGame(chess4, 4, 1014, 1001, DRAW, 743) == CHESS_SUCCESS);
    assert(chessAddGame(chess4, 2, 1012, 1035, SECOND_PLAYER, 471) == CHESS_SUCCESS);
    assert(chessAddGame(chess4, 3, 1001, 1028, SECOND_PLAYER, 750) == CHESS_GAME_ALREADY_EXISTS);
    assert(chessAddGame(chess4, 1, 1008, 1029, SECOND_PLAYER, 321) == CHESS_SUCCESS);
    assert(chessAddGame(chess4, 4, 1014, 1011, FIRST_PLAYER, 485) == CHESS_SUCCESS);
    assert(chessAddGame(chess4, 1, 1003, 1030, DRAW, 355) == CHESS_SUCCESS);
    assert(chessAddGame(chess4, 2, 1004, 1033, DRAW, 569) == CHESS_SUCCESS);
    assert(chessAddGame(chess4, 2, 1010, 1009, DRAW, 328) == CHESS_SUCCESS);
    assert(chessAddGame(chess4, 3, 1018, 1004, FIRST_PLAYER, 430) == CHESS_SUCCESS);
    assert(chessAddGame(chess4, 1, 1015, 1022, DRAW, 292) == CHESS_SUCCESS);
    assert(chessAddGame(chess4, 5, 1004, 1023, SECOND_PLAYER, 750) == CHESS_SUCCESS);
    assert(chessAddGame(chess4, 3, 1013, 1040, SECOND_PLAYER, 530) == CHESS_SUCCESS);
    assert(chessAddGame(chess4, 4, 1015, 1038, FIRST_PLAYER, 673) == CHESS_SUCCESS);
    assert(chessAddGame(chess4, 2, 1014, 1035, FIRST_PLAYER, 222) == CHESS_SUCCESS);
    assert(chessAddGame(chess4, 1, 1017, 1013, SECOND_PLAYER, 708) == CHESS_SUCCESS);
    assert(chessAddGame(chess4, 1, 1007, 1039, DRAW, 765) == CHESS_SUCCESS);
    assert(chessAddGame(chess4, 1, 1003, 1037, FIRST_PLAYER, 721) == CHESS_SUCCESS);
    assert(chessAddGame(chess4, 3, 1017, 1007, SECOND_PLAYER, 472) == CHESS_SUCCESS);
    assert(chessAddGame(chess4, 5, 1006, 1032, SECOND_PLAYER, 469) == CHESS_SUCCESS);
    assert(chessAddGame(chess4, 3, 1002, 1001, DRAW, 131) == CHESS_SUCCESS);
    assert(chessAddGame(chess4, 5, 1009, 1017, DRAW, 442) == CHESS_SUCCESS);
    assert(chessAddGame(chess4, 1, 1008, 1024, FIRST_PLAYER, 425) == CHESS_SUCCESS);
    assert(chessAddGame(chess4, 4, 1010, 1019, FIRST_PLAYER, 210) == CHESS_SUCCESS);
    assert(chessAddGame(chess4, 1, 1015, 1005, FIRST_PLAYER, 335) == CHESS_SUCCESS);
    assert(chessAddGame(chess4, 4, 1004, 1030, SECOND_PLAYER, 217) == CHESS_SUCCESS);
    assert(chessAddGame(chess4, 1, 1015, 1028, DRAW, 435) == CHESS_SUCCESS);
    assert(chessAddGame(chess4, 5, 1009, 1040, FIRST_PLAYER, 802) == CHESS_SUCCESS);
    assert(chessAddGame(chess4, 3, 1007, 1014, SECOND_PLAYER, 449) == CHESS_SUCCESS);
    assert(chessAddGame(chess4, 4, 1017, 1011, SECOND_PLAYER, 964) == CHESS_SUCCESS);
    assert(chessAddGame(chess4, 1, 1005, 1033, SECOND_PLAYER, 575) == CHESS_SUCCESS);
    assert(chessAddGame(chess4, 3, 1006, 1033, DRAW, 109) == CHESS_SUCCESS);
    assert(chessAddGame(chess4, 1, 1001, 1009, SECOND_PLAYER, 302) == CHESS_SUCCESS);
    assert(chessAddGame(chess4, 3, 1001, 1006, FIRST_PLAYER, 383) == CHESS_SUCCESS);
    assert(chessAddGame(chess4, 5, 1014, 1022, SECOND_PLAYER, 519) == CHESS_SUCCESS);
    assert(chessAddGame(chess4, 5, 1003, 1003, DRAW, 521) == CHESS_INVALID_ID);
    assert(chessAddGame(chess4, 4, 1008, 1026, DRAW, 428) == CHESS_SUCCESS);
    assert(chessAddGame(chess4, 2, 1005, 1003, SECOND_PLAYER, 254) == CHESS_SUCCESS);
    assert(chessAddGame(chess4, 4, 1005, 1033, FIRST_PLAYER, 418) == CHESS_SUCCESS);
    assert(chessAddGame(chess4, 4, 1013, 1036, DRAW, 687) == CHESS_SUCCESS);
    assert(chessAddGame(chess4, 4, 1002, 1002, SECOND_PLAYER, 911) == CHESS_INVALID_ID);
    assert(chessAddGame(chess4, 2, 1013, 1019, SECOND_PLAYER, 555) == CHESS_SUCCESS);
    assert(chessAddGame(chess4, 2, 1005, 1021, SECOND_PLAYER, 394) == CHESS_SUCCESS);
    assert(chessAddGame(chess4, 3, 1006, 1031, FIRST_PLAYER, 781) == CHESS_SUCCESS);
    assert(chessAddGame(chess4, 5, 1006, 1013, DRAW, 746) == CHESS_SUCCESS);
    assert(chessAddGame(chess4, 5, 1006, 1030, FIRST_PLAYER, 396) == CHESS_SUCCESS);
    assert(chessAddGame(chess4, 5, 1005, 1034, FIRST_PLAYER, 425) == CHESS_SUCCESS);
    assert(chessAddGame(chess4, 5, 1001, 1016, FIRST_PLAYER, 220) == CHESS_SUCCESS);
    assert(chessAddGame(chess4, 4, 1006, 1004, DRAW, 738) == CHESS_SUCCESS);
    assert(chessAddGame(chess4, 1, 1017, 1019, DRAW, 272) == CHESS_SUCCESS);
    assert(chessAddGame(chess4, 2, 1012, 1039, FIRST_PLAYER, 973) == CHESS_SUCCESS);
    assert(chessAddGame(chess4, 2, 1002, 1017, FIRST_PLAYER, 444) == CHESS_SUCCESS);
    assert(chessAddGame(chess4, 1, 1007, 1032, DRAW, 207) == CHESS_SUCCESS);
    assert(chessAddGame(chess4, 4, 1010, 1001, SECOND_PLAYER, 595) == CHESS_SUCCESS);
    assert(chessAddGame(chess4, 4, 1008, 1013, FIRST_PLAYER, 501) == CHESS_SUCCESS);
    assert(chessAddGame(chess4, 2, 1011, 1002, FIRST_PLAYER, 623) == CHESS_SUCCESS);
    assert(chessAddGame(chess4, 2, 1011, 1032, DRAW, 474) == CHESS_SUCCESS);
    assert(chessAddGame(chess4, 3, 1001, 1001, DRAW, 855) == CHESS_INVALID_ID);
    assert(chessAddGame(chess4, 5, 1014, 1038, SECOND_PLAYER, 207) == CHESS_SUCCESS);
    assert(chessAddGame(chess4, 4, 1011, 1013, DRAW, 396) == CHESS_SUCCESS);
    assert(chessAddGame(chess4, 5, 1010, 1032, SECOND_PLAYER, 643) == CHESS_SUCCESS);
    assert(chessAddGame(chess4, 1, 1018, 1012, DRAW, 102) == CHESS_SUCCESS);
    assert(chessAddGame(chess4, 2, 1007, 1035, DRAW, 516) == CHESS_SUCCESS);
    assert(chessAddGame(chess4, 4, 1014, 1015, FIRST_PLAYER, 576) == CHESS_SUCCESS);
    assert(chessAddGame(chess4, 4, 1005, 1035, SECOND_PLAYER, 783) == CHESS_SUCCESS);
    assert(chessAddGame(chess4, 4, 1017, 1026, FIRST_PLAYER, 431) == CHESS_SUCCESS);
    assert(chessAddGame(chess4, 3, 1001, 1025, SECOND_PLAYER, 624) == CHESS_SUCCESS);
    assert(chessAddGame(chess4, 5, 1001, 1008, SECOND_PLAYER, 813) == CHESS_SUCCESS);
    assert(chessAddGame(chess4, 3, 1018, 1039, FIRST_PLAYER, 118) == CHESS_SUCCESS);
    assert(chessAddGame(chess4, 3, 1005, 1023, DRAW, 152) == CHESS_SUCCESS);
    assert(chessAddGame(chess4, 3, 1008, 1032, SECOND_PLAYER, 107) == CHESS_SUCCESS);
    assert(chessAddGame(chess4, 2, 1008, 1010, DRAW, 432) == CHESS_SUCCESS);
    assert(chessAddGame(chess4, 3, 1009, 1023, DRAW, 476) == CHESS_SUCCESS);
    assert(chessAddGame(chess4, 3, 1012, 1015, FIRST_PLAYER, 152) == CHESS_SUCCESS);
    assert(chessAddGame(chess4, 2, 1015, 1038, FIRST_PLAYER, 588) == CHESS_SUCCESS);
    assert(chessAddGame(chess4, 1, 1020, 1022, DRAW, 884) == CHESS_SUCCESS);
    assert(chessAddGame(chess4, 4, 1019, 1019, DRAW, 827) == CHESS_INVALID_ID);
    assert(chessAddGame(chess4, 5, 1001, 1025, FIRST_PLAYER, 252) == CHESS_SUCCESS);
    assert(chessAddGame(chess4, 4, 1018, 1025, SECOND_PLAYER, 871) == CHESS_SUCCESS);
    assert(chessAddGame(chess4, 1, 1020, 1009, FIRST_PLAYER, 285) == CHESS_SUCCESS);
    assert(chessAddGame(chess4, 1, 1013, 1040, SECOND_PLAYER, 754) == CHESS_SUCCESS);
    assert(chessAddGame(chess4, 3, 1006, 1009, FIRST_PLAYER, 129) == CHESS_GAME_ALREADY_EXISTS);
    assert(chessAddGame(chess4, 1, 1013, 1017, FIRST_PLAYER, 584) == CHESS_GAME_ALREADY_EXISTS);
    assert(chessAddGame(chess4, 5, 1002, 1019, SECOND_PLAYER, 398) == CHESS_SUCCESS);
    assert(chessAddGame(chess4, 4, 1016, 1001, FIRST_PLAYER, 471) == CHESS_SUCCESS);
    assert(chessAddGame(chess4, 1, 1002, 1014, DRAW, 607) == CHESS_SUCCESS);
    assert(chessAddGame(chess4, 5, 1012, 1025, DRAW, 261) == CHESS_SUCCESS);
    assert(chessAddGame(chess4, 2, 1006, 1033, FIRST_PLAYER, 406) == CHESS_SUCCESS);
    assert(chessAddGame(chess4, 5, 1007, 1024, SECOND_PLAYER, 654) == CHESS_SUCCESS);
    assert(chessAddGame(chess4, 4, 1016, 1033, DRAW, 549) == CHESS_SUCCESS);
    assert(chessAddGame(chess4, 5, 1017, 1008, DRAW, 268) == CHESS_SUCCESS);
    assert(chessAddGame(chess4, 5, 1020, 1020, SECOND_PLAYER, 166) == CHESS_INVALID_ID);
    assert(chessAddGame(chess4, 4, 1016, 1016, DRAW, 651) == CHESS_INVALID_ID);
    assert(chessAddGame(chess4, 4, 1013, 1031, DRAW, 440) == CHESS_SUCCESS);
    assert(chessAddGame(chess4, 1, 1012, 1011, FIRST_PLAYER, 706) == CHESS_SUCCESS);
    assert(chessAddGame(chess4, 5, 1001, 1032, FIRST_PLAYER, 752) == CHESS_SUCCESS);
    assert(chessAddGame(chess4, 3, 1014, 1026, SECOND_PLAYER, 841) == CHESS_SUCCESS);
    assert(chessAddGame(chess4, 1, 1009, 1030, FIRST_PLAYER, 142) == CHESS_SUCCESS);
    assert(chessAddGame(chess4, 4, 1003, 1023, FIRST_PLAYER, 354) == CHESS_SUCCESS);
    assert(chessAddGame(chess4, 2, 1020, 1023, DRAW, 717) == CHESS_SUCCESS);
    assert(chessAddGame(chess4, 1, 1019, 1011, FIRST_PLAYER, 724) == CHESS_SUCCESS);
    assert(chessAddGame(chess4, 4, 1001, 1002, SECOND_PLAYER, 751) == CHESS_SUCCESS);
    assert(chessAddGame(chess4, 1, 1007, 1014, SECOND_PLAYER, 224) == CHESS_SUCCESS);
    assert(chessAddGame(chess4, 5, 1010, 1015, SECOND_PLAYER, 859) == CHESS_SUCCESS);
    assert(chessAddGame(chess4, 4, 1001, 1029, FIRST_PLAYER, 204) == CHESS_SUCCESS);
    assert(chessAddGame(chess4, 2, 1018, 1038, DRAW, 887) == CHESS_SUCCESS);
    assert(chessAddGame(chess4, 1, 1006, 1009, DRAW, 735) == CHESS_SUCCESS);
    assert(chessAddGame(chess4, 5, 1007, 1022, SECOND_PLAYER, 261) == CHESS_SUCCESS);
    assert(chessAddGame(chess4, 5, 1010, 1038, SECOND_PLAYER, 780) == CHESS_SUCCESS);
    assert(chessAddGame(chess4, 3, 1019, 1018, SECOND_PLAYER, 327) == CHESS_GAME_ALREADY_EXISTS);
    assert(chessAddGame(chess4, 5, 1013, 1023, DRAW, 209) == CHESS_SUCCESS);
    assert(chessAddGame(chess4, 2, 1011, 1020, DRAW, 688) == CHESS_SUCCESS);
    assert(chessAddGame(chess4, 5, 1001, 1033, SECOND_PLAYER, 902) == CHESS_SUCCESS);
    assert(chessAddGame(chess4, 5, 1011, 1034, SECOND_PLAYER, 698) == CHESS_SUCCESS);
    assert(chessAddGame(chess4, 3, 1008, 1010, FIRST_PLAYER, 397) == CHESS_SUCCESS);
    assert(chessAddGame(chess4, 3, 1019, 1036, DRAW, 287) == CHESS_SUCCESS);
    assert(chessAddGame(chess4, 4, 1003, 1020, DRAW, 648) == CHESS_SUCCESS);
    assert(chessAddGame(chess4, 4, 1005, 1031, SECOND_PLAYER, 462) == CHESS_SUCCESS);
    assert(chessAddGame(chess4, 3, 1013, 1037, FIRST_PLAYER, 633) == CHESS_SUCCESS);
    assert(chessAddGame(chess4, 2, 1015, 1007, DRAW, 789) == CHESS_SUCCESS);
    assert(chessAddGame(chess4, 3, 1016, 1009, DRAW, 241) == CHESS_SUCCESS);
    assert(chessAddGame(chess4, 3, 1006, 1040, FIRST_PLAYER, 423) == CHESS_SUCCESS);
    assert(chessAddGame(chess4, 1, 1016, 1040, DRAW, 777) == CHESS_SUCCESS);
    assert(chessAddGame(chess4, 5, 1015, 1023, FIRST_PLAYER, 752) == CHESS_SUCCESS);
    assert(chessAddGame(chess4, 5, 1009, 1017, SECOND_PLAYER, 207) == CHESS_GAME_ALREADY_EXISTS);
    assert(chessAddGame(chess4, 1, 1011, 1038, FIRST_PLAYER, 663) == CHESS_SUCCESS);
    assert(chessAddGame(chess4, 1, 1006, 1028, SECOND_PLAYER, 642) == CHESS_SUCCESS);
    assert(chessAddGame(chess4, 4, 1017, 1037, SECOND_PLAYER, 902) == CHESS_SUCCESS);
    assert(chessAddGame(chess4, 2, 1004, 1040, DRAW, 969) == CHESS_SUCCESS);
    assert(chessAddGame(chess4, 1, 1017, 1032, DRAW, 574) == CHESS_SUCCESS);
    assert(chessAddGame(chess4, 5, 1017, 1031, FIRST_PLAYER, 557) == CHESS_SUCCESS);
    assert(chessAddGame(chess4, 2, 1019, 1029, SECOND_PLAYER, 879) == CHESS_SUCCESS);
    assert(chessAddGame(chess4, 2, 1018, 1019, SECOND_PLAYER, 817) == CHESS_SUCCESS);
    assert(chessAddGame(chess4, 1, 1019, 1040, SECOND_PLAYER, 417) == CHESS_SUCCESS);
    assert(chessAddGame(chess4, 5, 1003, 1024, DRAW, 911) == CHESS_SUCCESS);
    assert(chessAddGame(chess4, 1, 1014, 1017, FIRST_PLAYER, 879) == CHESS_SUCCESS);
    assert(chessAddGame(chess4, 3, 1018, 1013, FIRST_PLAYER, 249) == CHESS_SUCCESS);
    assert(chessAddGame(chess4, 4, 1007, 1005, DRAW, 401) == CHESS_SUCCESS);
    assert(chessAddGame(chess4, 1, 1018, 1017, SECOND_PLAYER, 920) == CHESS_SUCCESS);
    assert(chessAddGame(chess4, 2, 1007, 1033, SECOND_PLAYER, 647) == CHESS_SUCCESS);
    assert(chessAddGame(chess4, 2, 1006, 1018, SECOND_PLAYER, 357) == CHESS_SUCCESS);
    assert(chessAddGame(chess4, 3, 1002, 1017, SECOND_PLAYER, 100) == CHESS_SUCCESS);
    assert(chessAddGame(chess4, 5, 1013, 1032, DRAW, 425) == CHESS_SUCCESS);
    assert(chessAddGame(chess4, 2, 1018, 1036, FIRST_PLAYER, 769) == CHESS_SUCCESS);
    assert(chessAddGame(chess4, 5, 1012, 1019, SECOND_PLAYER, 941) == CHESS_SUCCESS);
    assert(chessAddGame(chess4, 3, 1015, 1004, FIRST_PLAYER, 172) == CHESS_SUCCESS);
    assert(chessAddGame(chess4, 1, 1015, 1024, SECOND_PLAYER, 877) == CHESS_SUCCESS);
    assert(chessAddGame(chess4, 1, 1019, 1016, SECOND_PLAYER, 414) == CHESS_SUCCESS);
    assert(chessAddGame(chess4, 2, 1019, 1021, FIRST_PLAYER, 918) == CHESS_SUCCESS);
    assert(chessAddGame(chess4, 1, 1006, 1017, FIRST_PLAYER, 460) == CHESS_SUCCESS);
    assert(chessAddGame(chess4, 5, 1017, 1038, SECOND_PLAYER, 562) == CHESS_SUCCESS);
    assert(chessAddGame(chess4, 4, 1008, 1025, SECOND_PLAYER, 354) == CHESS_SUCCESS);
    assert(chessAddGame(chess4, 2, 1015, 1017, SECOND_PLAYER, 186) == CHESS_SUCCESS);
    assert(chessAddGame(chess4, 3, 1002, 1018, DRAW, 786) == CHESS_SUCCESS);
    assert(chessAddGame(chess4, 1, 1005, 1012, SECOND_PLAYER, 305) == CHESS_SUCCESS);
    assert(chessAddGame(chess4, 2, 1019, 1024, FIRST_PLAYER, 555) == CHESS_SUCCESS);
    assert(chessAddGame(chess4, 1, 1003, 1020, SECOND_PLAYER, 842) == CHESS_SUCCESS);
    assert(chessAddGame(chess4, 3, 1015, 1020, FIRST_PLAYER, 609) == CHESS_SUCCESS);
    assert(chessAddGame(chess4, 1, 1008, 1007, FIRST_PLAYER, 711) == CHESS_SUCCESS);
    assert(chessAddGame(chess4, 3, 1011, 1019, SECOND_PLAYER, 813) == CHESS_SUCCESS);
    assert(chessAddGame(chess4, 1, 1012, 1002, FIRST_PLAYER, 714) == CHESS_SUCCESS);
    assert(chessAddGame(chess4, 4, 1003, 1028, FIRST_PLAYER, 865) == CHESS_SUCCESS);
    assert(chessAddGame(chess4, 3, 1020, 1022, SECOND_PLAYER, 481) == CHESS_SUCCESS);
    assert(chessAddGame(chess4, 2, 1011, 1003, FIRST_PLAYER, 519) == CHESS_SUCCESS);
    assert(chessAddGame(chess4, 3, 1012, 1002, DRAW, 703) == CHESS_SUCCESS);
    assert(chessAddGame(chess4, 5, 1013, 1025, DRAW, 195) == CHESS_SUCCESS);
    assert(chessAddGame(chess4, 4, 1015, 1024, SECOND_PLAYER, 259) == CHESS_SUCCESS);
    assert(chessAddGame(chess4, 2, 1005, 1030, DRAW, 116) == CHESS_SUCCESS);
    assert(chessAddGame(chess4, 2, 1015, 1001, SECOND_PLAYER, 872) == CHESS_SUCCESS);
    assert(chessAddGame(chess4, 3, 1014, 1013, FIRST_PLAYER, 636) == CHESS_SUCCESS);
    assert(chessAddGame(chess4, 1, 1005, 1002, SECOND_PLAYER, 806) == CHESS_SUCCESS);
    assert(chessAddGame(chess4, 4, 1010, 1026, SECOND_PLAYER, 905) == CHESS_SUCCESS);
    assert(chessAddGame(chess4, 5, 1016, 1036, DRAW, 226) == CHESS_SUCCESS);
    assert(chessAddGame(chess4, 3, 1014, 1035, FIRST_PLAYER, 316) == CHESS_SUCCESS);
    assert(chessAddGame(chess4, 2, 1002, 1013, DRAW, 691) == CHESS_SUCCESS);
    assert(chessAddGame(chess4, 5, 1004, 1023, DRAW, 122) == CHESS_GAME_ALREADY_EXISTS);
    assert(chessAddGame(chess4, 4, 1019, 1019, SECOND_PLAYER, 480) == CHESS_INVALID_ID);
    assert(chessAddGame(chess4, 5, 1019, 1031, SECOND_PLAYER, 291) == CHESS_SUCCESS);
    assert(chessAddGame(chess4, 1, 1009, 1027, SECOND_PLAYER, 699) == CHESS_SUCCESS);
    assert(chessAddGame(chess4, 5, 1004, 1030, FIRST_PLAYER, 850) == CHESS_SUCCESS);
    assert(chessAddGame(chess4, 5, 1018, 1005, FIRST_PLAYER, 494) == CHESS_SUCCESS);
    assert(chessAddGame(chess4, 1, 1005, 1014, DRAW, 207) == CHESS_SUCCESS);
    assert(chessAddGame(chess4, 4, 1017, 1022, FIRST_PLAYER, 627) == CHESS_SUCCESS);
    assert(chessAddGame(chess4, 5, 1016, 1012, SECOND_PLAYER, 355) == CHESS_SUCCESS);
    assert(chessAddGame(chess4, 5, 1001, 1039, SECOND_PLAYER, 381) == CHESS_SUCCESS);
    assert(chessAddGame(chess4, 4, 1001, 1006, FIRST_PLAYER, 941) == CHESS_SUCCESS);
    assert(chessAddGame(chess4, 3, 1011, 1030, FIRST_PLAYER, 704) == CHESS_SUCCESS);
    assert(chessAddGame(chess4, 2, 1012, 1032, FIRST_PLAYER, 296) == CHESS_SUCCESS);
    assert(chessAddGame(chess4, 2, 1006, 1008, FIRST_PLAYER, 874) == CHESS_SUCCESS);
    assert(chessAddGame(chess4, 3, 1019, 1010, FIRST_PLAYER, 954) == CHESS_SUCCESS);
    assert(chessAddGame(chess4, 1, 1006, 1016, DRAW, 881) == CHESS_SUCCESS);
    assert(chessAddGame(chess4, 3, 1017, 1030, FIRST_PLAYER, 487) == CHESS_SUCCESS);
    assert(chessAddGame(chess4, 2, 1017, 1024, FIRST_PLAYER, 823) == CHESS_SUCCESS);
    assert(chessAddGame(chess4, 1, 1004, 1030, SECOND_PLAYER, 570) == CHESS_SUCCESS);
    assert(chessAddGame(chess4, 2, 1013, 1001, SECOND_PLAYER, 551) == CHESS_SUCCESS);
    assert(chessAddGame(chess4, 2, 1004, 1016, FIRST_PLAYER, 722) == CHESS_SUCCESS);
    assert(chessAddGame(chess4, 5, 1017, 1022, FIRST_PLAYER, 583) == CHESS_SUCCESS);
    assert(chessAddGame(chess4, 1, 1013, 1036, SECOND_PLAYER, 620) == CHESS_SUCCESS);
    assert(chessAddGame(chess4, 1, 1009, 1037, FIRST_PLAYER, 838) == CHESS_SUCCESS);
    assert(chessAddGame(chess4, 4, 1009, 1036, FIRST_PLAYER, 339) == CHESS_GAME_ALREADY_EXISTS);
    assert(chessAddGame(chess4, 5, 1004, 1010, FIRST_PLAYER, 675) == CHESS_SUCCESS);
    assert(chessAddGame(chess4, 1, 1012, 1038, DRAW, 179) == CHESS_SUCCESS);
    assert(chessAddGame(chess4, 3, 1005, 1007, FIRST_PLAYER, 277) == CHESS_SUCCESS);
    assert(chessAddGame(chess4, 2, 1002, 1022, SECOND_PLAYER, 975) == CHESS_SUCCESS);
    assert(chessAddGame(chess4, 3, 1009, 1029, DRAW, 396) == CHESS_SUCCESS);
    assert(chessAddGame(chess4, 4, 1005, 1035, DRAW, 750) == CHESS_GAME_ALREADY_EXISTS);
    assert(chessAddGame(chess4, 1, 1009, 1003, FIRST_PLAYER, 532) == CHESS_SUCCESS);
    assert(chessAddGame(chess4, 2, 1012, 1020, SECOND_PLAYER, 787) == CHESS_SUCCESS);
    assert(chessAddGame(chess4, 1, 1007, 1013, DRAW, 827) == CHESS_GAME_ALREADY_EXISTS);
    assert(chessAddGame(chess4, 3, 1015, 1005, FIRST_PLAYER, 578) == CHESS_SUCCESS);
    assert(chessAddGame(chess4, 1, 1010, 1035, DRAW, 351) == CHESS_SUCCESS);
    assert(chessAddGame(chess4, 3, 1016, 1002, FIRST_PLAYER, 115) == CHESS_SUCCESS);
    assert(chessAddGame(chess4, 4, 1005, 1028, DRAW, 654) == CHESS_SUCCESS);
    assert(chessAddGame(chess4, 4, 1007, 1015, FIRST_PLAYER, 309) == CHESS_SUCCESS);
    assert(chessAddGame(chess4, 3, 1019, 1038, FIRST_PLAYER, 766) == CHESS_SUCCESS);
    assert(chessAddGame(chess4, 3, 1010, 1026, SECOND_PLAYER, 841) == CHESS_SUCCESS);
    assert(chessAddGame(chess4, 1, 1015, 1015, SECOND_PLAYER, 517) == CHESS_INVALID_ID);
    assert(chessAddGame(chess4, 3, 1001, 1002, DRAW, 194) == CHESS_GAME_ALREADY_EXISTS);
    assert(chessAddGame(chess4, 3, 1003, 1037, FIRST_PLAYER, 182) == CHESS_SUCCESS);
    assert(chessAddGame(chess4, 4, 1007, 1020, DRAW, 186) == CHESS_SUCCESS);
    assert(chessAddGame(chess4, 2, 1002, 1012, FIRST_PLAYER, 821) == CHESS_SUCCESS);
    assert(chessAddGame(chess4, 5, 1016, 1015, SECOND_PLAYER, 322) == CHESS_SUCCESS);
    assert(chessAddGame(chess4, 1, 1001, 1037, SECOND_PLAYER, 948) == CHESS_SUCCESS);
    assert(chessAddGame(chess4, 2, 1017, 1040, FIRST_PLAYER, 175) == CHESS_SUCCESS);
    assert(chessAddGame(chess4, 5, 1007, 1016, FIRST_PLAYER, 925) == CHESS_SUCCESS);
    assert(chessAddGame(chess4, 3, 1017, 1011, DRAW, 829) == CHESS_SUCCESS);
    assert(chessAddGame(chess4, 4, 1014, 1030, FIRST_PLAYER, 370) == CHESS_SUCCESS);
    assert(chessAddGame(chess4, 4, 1016, 1013, FIRST_PLAYER, 756) == CHESS_SUCCESS);
    assert(chessAddGame(chess4, 1, 1002, 1003, DRAW, 194) == CHESS_SUCCESS);
    assert(chessAddGame(chess4, 4, 1009, 1033, FIRST_PLAYER, 867) == CHESS_SUCCESS);
    assert(chessAddGame(chess4, 3, 1005, 1014, DRAW, 354) == CHESS_SUCCESS);
    assert(chessAddGame(chess4, 2, 1011, 1030, SECOND_PLAYER, 910) == CHESS_SUCCESS);
    assert(chessAddGame(chess4, 1, 1005, 1011, DRAW, 999) == CHESS_SUCCESS);
    assert(chessAddGame(chess4, 1, 1005, 1033, SECOND_PLAYER, 866) == CHESS_GAME_ALREADY_EXISTS);
    assert(chessAddGame(chess4, 3, 1015, 1035, FIRST_PLAYER, 393) == CHESS_SUCCESS);
    assert(chessAddGame(chess4, 5, 1016, 1009, DRAW, 607) == CHESS_SUCCESS);

    assert(chessRemoveTournament(chess4, 2) == CHESS_SUCCESS);
    assert(chessRemovePlayer(chess4, 1026)  == CHESS_SUCCESS);
    assert(chessRemovePlayer(chess4, 1015)  == CHESS_SUCCESS);
    assert(chessEndTournament(chess4, 1)    == CHESS_SUCCESS);
    assert(chessEndTournament(chess4, 2)    == CHESS_TOURNAMENT_NOT_EXIST);
    assert(chessEndTournament(chess4, 3)    == CHESS_SUCCESS);
    assert(chessEndTournament(chess4, 4)    == CHESS_SUCCESS);
    assert(chessEndTournament(chess4, 5)    == CHESS_SUCCESS);

    file = fopen("./output/actual/levels7.txt", "w");
    assert(chessSavePlayersLevels(chess4, file) == CHESS_SUCCESS);
    fclose(file);
    assert(chessSaveTournamentStatistics(chess4, "./output/actual/tournaments7.txt") == CHESS_SUCCESS);
    chessDestroy(chess4);

    printf("     [OK]\n");
    printf(">>COMPARE MANUALLY OUTPUT FILES...\n");
}


int main ()
{
    gameTests();
    playerInTournamentTests();
    playerTests();
    tournamentTests();
    chessTests();
    massiveTest();
    return 0;
}