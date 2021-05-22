#include <stdio.h>
#include <stdlib.h>

#include "chessSystem.h"
#include "map.h"
#include "mapUtil.h"
#include "tournament.h"
#include "game.h"
#include "player.h"

struct chess_system_t {
    Map tournaments;
    Map players;
};



ChessSystem chessCreate()
{
    ChessSystem chess_system = malloc(sizeof(*chess_system));
    
    if (chess_system == NULL)
    {
        return NULL;
    }

    Map players     = createPlayersMap();
    if (players == NULL)
    {
        free(chess_system);
        return NULL;
    }
    Map tournaments = createTournamentsMap();

    if (tournaments == NULL)
    {
        free(players);
        free(chess_system);
        return NULL;
    }

    chess_system->players = players;
    chess_system->tournaments = tournaments;

    return chess_system;
}

void chessDestroy(ChessSystem chess)
{
    if (chess == NULL)
    {
        return;
    }

    mapDestroy(chess->players);
    mapDestroy(chess->tournaments);
    free(chess);
}


ChessResult chessAddTournament (ChessSystem chess, int tournament_id,
                                int max_games_per_player, const char* tournament_location)
{
    if (chess == NULL)
    {
        return CHESS_NULL_ARGUMENT;
    }

    if (tournament_id <= 0)
    {
        return CHESS_INVALID_ID;
    }

    if (mapContains(chess->tournaments, &tournament_id))
    {
        return CHESS_TOURNAMENT_ALREADY_EXISTS;
    }

    if (!tournamentValidateLocation(tournament_location))
    {
        return CHESS_INVALID_LOCATION;
    }

    if (max_games_per_player <= 0)
    {
        return CHESS_INVALID_MAX_GAMES;
    }

    Tournament new_tournament = tournamentCreate(tournament_id, max_games_per_player, tournament_location);
    if (new_tournament == NULL)
    {
        return CHESS_OUT_OF_MEMORY;
    }
    MapResult tournament_put_result = mapPut(chess->tournaments, &tournament_id, new_tournament);
    tournamentDestroy(new_tournament); // A copy was sent to the map, no need for the object at this point

    if (tournament_put_result == MAP_OUT_OF_MEMORY)
    {
        return CHESS_OUT_OF_MEMORY;
    }

    return CHESS_SUCCESS;
}


//ChessResult chessAddGame(ChessSystem chess, int tournament_id, int first_player,
//                         int second_player, Winner winner, int play_time)
// CHECK IF GAME ALREADY EXISTS - STATIC HELPER FUNCTION
// CHECK AMOUNT OF NEW PLAYERS

//ChessResult chessRemoveTournament (ChessSystem chess, int tournament_id)


//ChessResult chessRemovePlayer(ChessSystem chess, int player_id)


//ChessResult chessEndTournament (ChessSystem chess, int tournament_id)
// calculate the winner

//double chessCalculateAveragePlayTime (ChessSystem chess, int player_id, ChessResult* chess_result)


//ChessResult chessSavePlayersLevels (ChessSystem chess, FILE* file)


//ChessResult chessSaveTournamentStatistics (ChessSystem chess, char* path_file)


//static int ChessTournamentCalculateWinner (Tournament tournament)
//{
    //TOURNAMENT_WIN_WEIGHT
//}
