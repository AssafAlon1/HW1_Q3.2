#include <stdlib.h>
#include "ChessSystem.h"
#include "test_utilities.h"

/*The number of tests*/
#define NUMBER_TESTS 4


bool testChessAddTournament() {
    ChessSystem chess = chessCreate();
    ASSERT_TEST(chessAddTournament(chess, 3, 4, "C") == CHESS_SUCCESS);
    ASSERT_TEST(chessAddTournament(chess, 1, 10, "iondon") == CHESS_INVALID_LOCATION);
    ASSERT_TEST(chessAddTournament(chess, 1, 5, "A") == CHESS_SUCCESS);
    ASSERT_TEST(chessAddTournament(chess, 1, 6, "London") == CHESS_TOURNAMENT_ALREADY_EXISTS);
    ASSERT_TEST(chessAddTournament(chess, 4, 5, "D") == CHESS_SUCCESS);
    ASSERT_TEST(chessAddTournament(chess, 2, 4, "B") == CHESS_SUCCESS);
    ASSERT_TEST(chessAddTournament(chess, 5, 2, "E") == CHESS_SUCCESS);
    ASSERT_TEST(chessAddTournament(chess, 10, 2, "F") == CHESS_SUCCESS);
    ASSERT_TEST(chessEndTournament(chess, 10) == CHESS_NO_GAMES);
    ASSERT_TEST(chessAddGame(chess, 10, 1, 2, FIRST_PLAYER, 2) == CHESS_SUCCESS);
    ASSERT_TEST(chessAddGame(chess, 10, 1, 3, FIRST_PLAYER, 4) == CHESS_SUCCESS);
    ASSERT_TEST(chessAddGame(chess, 10, 2, 3, FIRST_PLAYER, 50) == CHESS_SUCCESS);
    ASSERT_TEST(chessAddGame(chess, 10, 30, 4, SECOND_PLAYER, 54) == CHESS_SUCCESS);
    ASSERT_TEST(chessAddGame(chess, 10, 4, 5, SECOND_PLAYER, 51) == CHESS_SUCCESS);
    ASSERT_TEST(chessAddGame(chess, 10, 1, 5, SECOND_PLAYER, 5) == CHESS_EXCEEDED_GAMES);
    ASSERT_TEST(chessAddGame(chess, 10, 40, 50, SECOND_PLAYER, 51) == CHESS_SUCCESS);
    ASSERT_TEST(chessAddGame(chess, 10, 47, 56, SECOND_PLAYER, 51) == CHESS_SUCCESS);
    ASSERT_TEST(chessAddGame(chess, 10, 45, 54, SECOND_PLAYER, 51) == CHESS_SUCCESS);
    ASSERT_TEST(chessAddGame(chess, 10, 44, 52, SECOND_PLAYER, 51) == CHESS_SUCCESS);
    ASSERT_TEST(chessAddGame(chess, 10, 45, 52, SECOND_PLAYER, 51) == CHESS_SUCCESS);
    ASSERT_TEST(chessRemovePlayer(chess, 1) == CHESS_SUCCESS);
    ASSERT_TEST(chessAddGame(chess, 10, 45, 52, SECOND_PLAYER, 51) == CHESS_GAME_ALREADY_EXISTS);
    ASSERT_TEST(chessRemovePlayer(chess, 45) == CHESS_SUCCESS);
    ASSERT_TEST(chessAddGame(chess, 10, 45, 52, SECOND_PLAYER, 51) == CHESS_EXCEEDED_GAMES);
    ASSERT_TEST(chessEndTournament(chess,10) == CHESS_SUCCESS);
    ChessResult r10;
    ASSERT_TEST(chessCalculateAveragePlayTime(chess, 2, &r10) == 26);
    ASSERT_TEST(chessCalculateAveragePlayTime(chess, 4, &r10) == 52.5);
    ASSERT_TEST(chessSaveTournamentStatistics(chess, "your_output/tournament_statistics_your_output10.txt") == CHESS_SUCCESS);
    FILE* file_levels10 = fopen("your_output/player_levels_your_output10.txt", "w");
    ASSERT_TEST(chessSavePlayersLevels(chess, file_levels10) == CHESS_SUCCESS);
    fclose(file_levels10);
    ASSERT_TEST(chessAddTournament(chess, -2, 6, "London") == CHESS_INVALID_ID);
    ASSERT_TEST(chessAddTournament(chess, 50, -2, "London") == CHESS_INVALID_MAX_GAMES);
    ASSERT_TEST(chessAddTournament(chess, 1, 1, "London") == CHESS_TOURNAMENT_ALREADY_EXISTS);
    ASSERT_TEST(chessAddGame(chess, 4, 10, 11, FIRST_PLAYER, 5) == CHESS_SUCCESS);
    ASSERT_TEST(chessAddGame(chess, 4, 11, 12, FIRST_PLAYER, 5) == CHESS_SUCCESS);
    ASSERT_TEST(chessAddGame(chess, 4, 12, 13, FIRST_PLAYER, 5) == CHESS_SUCCESS);
    ASSERT_TEST(chessAddGame(chess, 4, 182, 193, DRAW, 5) == CHESS_SUCCESS);
    ASSERT_TEST(chessAddGame(chess, 4, 12, 183, DRAW, 5) == CHESS_SUCCESS);
    ASSERT_TEST(chessAddGame(chess, 4, 182, 139, DRAW, 5) == CHESS_SUCCESS);
    ASSERT_TEST(chessAddGame(chess, 4, 162, 13, DRAW, 5) == CHESS_SUCCESS);
    ASSERT_TEST(chessAddGame(chess, 4, 12000, 13, DRAW, 5) == CHESS_SUCCESS);
    ASSERT_TEST(chessAddGame(chess, 4, 1200, 13, DRAW, 5) == CHESS_SUCCESS);
    ASSERT_TEST(chessAddGame(chess, 4, 120, 13, DRAW, 5) == CHESS_SUCCESS);
    ASSERT_TEST(chessAddGame(chess, 1, 10, 11, FIRST_PLAYER, 5) == CHESS_SUCCESS);
    ASSERT_TEST(chessAddGame(chess, 1, 11, 12, SECOND_PLAYER, 4) == CHESS_SUCCESS);
    ASSERT_TEST(chessAddGame(chess, 1, 15, 16, DRAW, 3) == CHESS_SUCCESS);
    ASSERT_TEST(chessAddGame(chess, 1, 10, 11, FIRST_PLAYER, 2) == CHESS_GAME_ALREADY_EXISTS);
    ASSERT_TEST(chessAddGame(chess, 1, 10, 12, FIRST_PLAYER, 100) == CHESS_SUCCESS);
    ASSERT_TEST(chessAddGame(chess, 1, 10, 13, DRAW, 57) == CHESS_SUCCESS);
    ASSERT_TEST(chessAddGame(chess, 1, 17, 20, FIRST_PLAYER, 55) == CHESS_SUCCESS);
    ASSERT_TEST(chessAddGame(chess, 1, 17, 12, SECOND_PLAYER, 53) == CHESS_SUCCESS);
    ASSERT_TEST(chessAddGame(chess, 1, 12, 17, FIRST_PLAYER, 52) == CHESS_GAME_ALREADY_EXISTS);
    ASSERT_TEST(chessAddGame(chess, 1, 10, 70, SECOND_PLAYER, 51) == CHESS_SUCCESS);
    ASSERT_TEST(chessAddGame(chess, 1, 5, 10, FIRST_PLAYER, 522) == CHESS_SUCCESS);
    ASSERT_TEST(chessAddGame(chess, 1, 5, 6, SECOND_PLAYER, 534) == CHESS_SUCCESS);
    ASSERT_TEST(chessAddGame(chess, 1, 5, 7, SECOND_PLAYER, 53) == CHESS_SUCCESS);
    ASSERT_TEST(chessAddGame(chess, 1, 10, 7, SECOND_PLAYER, 5321) == CHESS_EXCEEDED_GAMES);
    ASSERT_TEST(chessAddGame(chess, 2, 15, 16, FIRST_PLAYER, 52) == CHESS_SUCCESS);
    ASSERT_TEST(chessAddGame(chess, 2, 150, 161, FIRST_PLAYER, 52) == CHESS_SUCCESS);
    ASSERT_TEST(chessAddGame(chess, 2, 151, 161, FIRST_PLAYER, 52) == CHESS_SUCCESS);
    ASSERT_TEST(chessAddGame(chess, 2, 152, 162, FIRST_PLAYER, 52) == CHESS_SUCCESS);
    ASSERT_TEST(chessAddGame(chess, 2, 153, 163, FIRST_PLAYER, 52) == CHESS_SUCCESS);
    ASSERT_TEST(chessAddGame(chess, 2, 154, 164, FIRST_PLAYER, 52) == CHESS_SUCCESS);
    ASSERT_TEST(chessAddGame(chess, 2, 155, 165, FIRST_PLAYER, 52) == CHESS_SUCCESS);
    ASSERT_TEST(chessAddGame(chess, 2, 156, 166, FIRST_PLAYER, 52) == CHESS_SUCCESS);
    ASSERT_TEST(chessAddGame(chess, 2, 157, 167, FIRST_PLAYER, 52) == CHESS_SUCCESS);
    ASSERT_TEST(chessAddGame(chess, 2, 158, 168, FIRST_PLAYER, 52) == CHESS_SUCCESS);
    ASSERT_TEST(chessAddGame(chess, 2, 10, 11, SECOND_PLAYER, 52) == CHESS_SUCCESS);
    ASSERT_TEST(chessAddGame(chess, 5, 10, 11, FIRST_PLAYER, 5) == CHESS_SUCCESS);
    ASSERT_TEST(chessAddGame(chess, 5, 11, 12, FIRST_PLAYER, 5) == CHESS_SUCCESS);
    ASSERT_TEST(chessAddGame(chess, 5, 11, 120, FIRST_PLAYER, 5) == CHESS_EXCEEDED_GAMES);
    ASSERT_TEST(chessAddGame(chess, 2, 10, 12, DRAW, 52) == CHESS_SUCCESS);
    ASSERT_TEST(chessAddGame(chess, 2, 20, 30, DRAW, 52) == CHESS_SUCCESS);
    ASSERT_TEST(chessAddGame(chess, 2, 17, 10, DRAW, 53) == CHESS_SUCCESS);
    ASSERT_TEST(chessAddGame(chess, 2, 15, 12, SECOND_PLAYER, 5323) == CHESS_SUCCESS);
    ASSERT_TEST(chessAddGame(chess, 2, 100, 17, SECOND_PLAYER, -3) == CHESS_INVALID_PLAY_TIME);
    ASSERT_TEST(chessAddGame(chess, 2, 10, 17, SECOND_PLAYER, 523) == CHESS_GAME_ALREADY_EXISTS);
    ASSERT_TEST(chessAddGame(chess, 2, 31, 15, FIRST_PLAYER, 523) == CHESS_SUCCESS);
    ASSERT_TEST(chessAddGame(chess, 2, 6, 5, DRAW, 532) == CHESS_SUCCESS);
    ASSERT_TEST(chessAddGame(chess, 2, 8, 7, SECOND_PLAYER, 523) == CHESS_SUCCESS);
    ASSERT_TEST(chessAddGame(chess, 2, 10, 70, SECOND_PLAYER, 50) == CHESS_SUCCESS);
    ASSERT_TEST(chessAddGame(chess, 2, 10, 700, SECOND_PLAYER, 5321) == CHESS_EXCEEDED_GAMES);
    ASSERT_TEST(chessAddGame(chess, 3, 10, 1, SECOND_PLAYER, 523) == CHESS_SUCCESS);
    ASSERT_TEST(chessAddGame(chess, 3, 10, 2, SECOND_PLAYER, 523) == CHESS_SUCCESS);
    ASSERT_TEST(chessAddGame(chess, 3, 10, 3, FIRST_PLAYER, 523) == CHESS_SUCCESS);
    ASSERT_TEST(chessAddGame(chess, 3, 10, 30, DRAW, 534) == CHESS_SUCCESS);
    ASSERT_TEST(chessAddGame(chess, 3, 12, 17, SECOND_PLAYER, 523) == CHESS_SUCCESS);
    ASSERT_TEST(chessAddGame(chess, 3, 13, 15, DRAW, 532) == CHESS_SUCCESS);
    ASSERT_TEST(chessAddGame(chess, 3, 16, 15, DRAW, 5234) == CHESS_SUCCESS);
    ASSERT_TEST(chessAddGame(chess, 3, 5, 6, DRAW, 5333) == CHESS_SUCCESS);
    ASSERT_TEST(chessAddGame(chess, 3, 7, 8, FIRST_PLAYER, 512) == CHESS_SUCCESS);
    ASSERT_TEST(chessAddGame(chess, 3, 1, 2, FIRST_PLAYER, 5123) == CHESS_SUCCESS);
    ASSERT_TEST(chessAddGame(chess, 3, 10, 7, SECOND_PLAYER, 5321) == CHESS_EXCEEDED_GAMES);
    ASSERT_TEST(chessAddGame(chess, 3, 10, 10, SECOND_PLAYER, 5321) == CHESS_INVALID_ID);
    ASSERT_TEST(chessAddGame(chess, 3, 80, 70, SECOND_PLAYER, 49) == CHESS_SUCCESS);
    ASSERT_TEST(chessAddGame(chess, 50, 10, 12, SECOND_PLAYER, 5321) == CHESS_TOURNAMENT_NOT_EXIST);
    ASSERT_TEST(chessAddGame(chess, 48, 10, 13, SECOND_PLAYER, 5321) == CHESS_TOURNAMENT_NOT_EXIST);
    ASSERT_TEST(chessEndTournament(chess,1) == CHESS_SUCCESS);
    ASSERT_TEST(chessEndTournament(chess,2)== CHESS_SUCCESS);
    ASSERT_TEST(chessAddGame(chess, 1, 100, 10, SECOND_PLAYER, 5321) == CHESS_TOURNAMENT_ENDED);
    ASSERT_TEST(chessAddGame(chess, 2, 100, 10, SECOND_PLAYER, 5321) == CHESS_TOURNAMENT_ENDED);
    ChessResult r1;
    ASSERT_TEST(chessCalculateAveragePlayTime(chess, 70, &r1) == 50.0);
    ASSERT_TEST(chessSaveTournamentStatistics(chess, "your_output/tournament_statistics_your_output1.txt") == CHESS_SUCCESS);
    FILE* file_levels = fopen("your_output/player_levels_your_output1.txt", "w");
    ASSERT_TEST(chessSavePlayersLevels(chess, file_levels) == CHESS_SUCCESS);
    fclose(file_levels);
    ASSERT_TEST(chessRemovePlayer(chess, 10) == CHESS_SUCCESS);
    FILE* file_levels2 = fopen("your_output/player_levels_your_output2.txt", "w");
    ASSERT_TEST(chessSavePlayersLevels(chess, file_levels2) == CHESS_SUCCESS);
    fclose(file_levels2);
    ASSERT_TEST(chessSaveTournamentStatistics(chess, "your_output/tournament_statistics_your_output2.txt") == CHESS_SUCCESS);
    ASSERT_TEST(chessAddGame(chess, 3, 10, 1, SECOND_PLAYER, 49) == CHESS_SUCCESS);
    ASSERT_TEST(chessAddGame(chess, 3, 10, 3, SECOND_PLAYER, 49) == CHESS_SUCCESS);
    ASSERT_TEST(chessAddGame(chess, 3, 10, 2, SECOND_PLAYER, 49) == CHESS_SUCCESS);
    ASSERT_TEST(chessAddGame(chess, 3, 10, 4, SECOND_PLAYER, 49) == CHESS_SUCCESS);
    ASSERT_TEST(chessAddGame(chess, 3, 10, 5, SECOND_PLAYER, 49) == CHESS_EXCEEDED_GAMES);
    ASSERT_TEST(chessRemovePlayer(chess, 70) == CHESS_SUCCESS);
    ASSERT_TEST(chessRemovePlayer(chess, 15) == CHESS_SUCCESS);
    ASSERT_TEST(chessEndTournament(chess,3) == CHESS_SUCCESS);
    ASSERT_TEST(chessRemovePlayer(chess, 12) == CHESS_SUCCESS);
    ASSERT_TEST(chessEndTournament(chess,10)== CHESS_TOURNAMENT_ENDED);
    ASSERT_TEST(chessEndTournament(chess,4) == CHESS_SUCCESS);
    ASSERT_TEST(chessSaveTournamentStatistics(chess, "your_output/tournament_statistics_your_output3.txt") == CHESS_SUCCESS);
    ASSERT_TEST(chessRemoveTournament(chess,1) == CHESS_SUCCESS);
    ASSERT_TEST(chessRemoveTournament(chess,1) == CHESS_TOURNAMENT_NOT_EXIST);
    FILE* file_levels4 = fopen("your_output/player_levels_your_output4.txt", "w");
    ASSERT_TEST(chessSavePlayersLevels(chess, file_levels4) == CHESS_SUCCESS);
    ASSERT_TEST(chessSaveTournamentStatistics(chess, "your_output/tournament_statistics_your_output4.txt") == CHESS_SUCCESS);
    fclose(file_levels4);
    ASSERT_TEST(chessRemoveTournament(chess,2) == CHESS_SUCCESS);
    ASSERT_TEST(chessRemoveTournament(chess,10) == CHESS_SUCCESS);
    ASSERT_TEST(chessRemoveTournament(chess,3) == CHESS_SUCCESS);
    chessDestroy(chess);

    return true;
}

bool testChessRemoveTournament() {
    ChessSystem chess = chessCreate();
    ASSERT_TEST(chessAddTournament(chess, 1, 4, "London") == CHESS_SUCCESS);
    ASSERT_TEST(chessAddTournament(chess, 2, 4, "London") == CHESS_SUCCESS);
    ASSERT_TEST(chessRemoveTournament(chess, 1) == CHESS_SUCCESS);
    ASSERT_TEST(chessRemoveTournament(chess, 1) == CHESS_TOURNAMENT_NOT_EXIST);
    ASSERT_TEST(chessAddTournament(chess, 1, 4, "Paris") == CHESS_SUCCESS);

    chessDestroy(chess);
    return true;
}

bool testChessAddGame(){
    ChessSystem chess = chessCreate();
    ASSERT_TEST(chessAddGame(chess, 1, 1, 2, FIRST_PLAYER, 2000) == CHESS_TOURNAMENT_NOT_EXIST);
    ASSERT_TEST(chessAddTournament(chess, 1, 4, "London") == CHESS_SUCCESS);
    ASSERT_TEST(chessAddGame(chess, 1, 1, 2, FIRST_PLAYER, 2000) == CHESS_SUCCESS);

    chessDestroy(chess);
    return true;
}

bool testChessPrintLevelsAndTournamentStatistics(){
    return true;
}



/*The functions for the tests should be added here*/
bool (*tests[]) (void) = {
        testChessAddTournament,
        testChessRemoveTournament,
        testChessAddGame,
        testChessPrintLevelsAndTournamentStatistics
};

/*The names of the test functions should be added here*/
const char* testNames[] = {
        "testChessAddTournament",
        "testChessRemoveTournament",
        "testChessAddGame",
        "testChessPrintLevelsAndTournamentStatistics"
};

int main(int argc, char *argv[]) {
    if (argc == 1) {
        for (int test_idx = 0; test_idx < NUMBER_TESTS; test_idx++) {
            RUN_TEST(tests[test_idx], testNames[test_idx]);
        }
        return 0;
    }
    if (argc != 2) {
        fprintf(stdout, "Usage: chessSystem <test index>\n");
        return 0;
    }

    int test_idx = strtol(argv[1], NULL, 10);
    if (test_idx < 1 || test_idx > NUMBER_TESTS) {
        fprintf(stderr, "Invalid test index %d\n", test_idx);
        return 0;
    }

    RUN_TEST(tests[test_idx - 1], testNames[test_idx - 1]);
    return 0;
}
