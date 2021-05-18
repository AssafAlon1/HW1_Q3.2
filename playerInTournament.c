#include <stdio.h>
#include "chessSystem.h"
#include "playerInTournament.h"
#include "game.h"
#include "map.h"
#include "mapUtil.h"

struct player_in_tournament_t {
    int tournament_id;
    int player_id;
    Map games;
    int wins;
    int draws;
    int losses;
    int total_game_time;
};



PlayerInTournament playerInTournamentCreate(int player_id, int tournament_id)
{
    PlayerInTournament player_in_tournament = malloc(sizeof(player_in_tournament));
    
    // Check if the player in tournament allocation failed
    if (player_in_tournament == NULL)
    {
        return NULL;
    }

    player_in_tournament->player_id = player_id;
    player_in_tournament->tournament_id = tournament_id;
    player_in_tournament->wins   = 0;
    player_in_tournament->draws  = 0;
    player_in_tournament->losses = 0;
    player_in_tournament->total_game_time = 0;
    player_in_tournament->games = mapCreate(gamePtrCopy, intCopy, gamePtrFree, intFree, intCompare);

    // If the map allocation failed
    if (player_in_tournament->games == NULL)
    {
        free(player_in_tournament);
        return NULL;
    }

    return player_in_tournament;
}

//void playerInTournamentDestroy(PlayerInTournament player_in_tournament)

//PlayerInTournament playerInTournamentCopy(PlayerInTournament player_in_tournament)

//ChessResult playerInTournamentAddGame(PlayerInTournament player_in_tournament, Game game)

int playerInTournamentGetWins (PlayerInTournament player_in_tournament)
{
    player_in_tournament->wins;
}

int playerInTournamentGetDraws (PlayerInTournament player_in_tournament)
{
    return player_in_tournament->draws;
}

int playerInTournamentGetLosses (PlayerInTournament player_in_tournament)
{
    return player_in_tournament->losses;
}

int playerInTournamentGetTotalTime (PlayerInTournament player_in_tournament)
{
    return player_in_tournament->total_game_time;
}

int playerInTournamentGetTotalGames (PlayerInTournament player_in_tournament)
{
    return player_in_tournament->wins + player_in_tournament->draws + player_in_tournament->losses;
}