#include <stdio.h>
#include "map.h"
#include "chessSystem.h"
#include "playerInTournament.h"
#include "game.h"
#include "mapUtil.h"

struct player_in_tournament_t {
    int tournament_id;
    int max_games_per_player;
    int player_id;
    int *game_ids;
    int wins;
    int draws;
    int losses;
    int total_game_time;
};



PlayerInTournament playerInTournamentCreate(int player_id, int tournament_id, int max_games_per_player)
{
    PlayerInTournament player_in_tournament = malloc(sizeof(player_in_tournament));
    
    // Check if the player in tournament allocation failed
    if (player_in_tournament == NULL)
    {
        return NULL;
    }

    player_in_tournament->player_id = player_id;
    player_in_tournament->tournament_id = tournament_id;
    player_in_tournament->max_games_per_player = max_games_per_player;
    player_in_tournament->wins   = 0;
    player_in_tournament->draws  = 0;
    player_in_tournament->losses = 0;
    player_in_tournament->total_game_time = 0;
    player_in_tournament->game_ids = malloc(max_games_per_player*sizeof(int));

    // If the allocation failed
    if (player_in_tournament->game_ids == NULL)
    {
        free(player_in_tournament);
        return NULL;
    }

    // Initialize game ids
    for (int i = 0 ; i < max_games_per_player ; i++)
    {
        player_in_tournament->game_ids[i] = INVALID_GAME_ID;
    }

    return player_in_tournament;
}

void playerInTournamentDestroy(PlayerInTournament player_in_tournament)
{
    free(player_in_tournament->game_ids);
    free(player_in_tournament);
}


PlayerInTournament playerInTournamentCopy(PlayerInTournament player_in_tournament)
{
    PlayerInTournament new_player_in_tournament = playerInTournamentCreate(
                    player_in_tournament->player_id, player_in_tournament->tournament_id, player_in_tournament->max_games_per_player);
    if (new_player_in_tournament == NULL)
    {
        return NULL;
    }

    new_player_in_tournament->draws  = player_in_tournament->draws;
    new_player_in_tournament->wins   = player_in_tournament->wins;
    new_player_in_tournament->losses = player_in_tournament->losses;
    new_player_in_tournament->total_game_time = player_in_tournament->total_game_time;

    // Copy games
    for (int i = 0 ; i < player_in_tournament->max_games_per_player ; i++)
    {
        // No more games were played
        if (player_in_tournament->game_ids[i] == INVALID_GAME_ID)
        {
            break;
        }
        new_player_in_tournament->game_ids[i] = player_in_tournament->game_ids[i];
    }

    return new_player_in_tournament;
}


/*ChessResult playerInTournamentAddGame(PlayerInTournament player_in_tournament, Game game)
{
    //if ()
    //
    int game_count = playerInTournamentGetTotalGames(player_in_tournament);
    (player_in_tournament->game_ids)[game_count] = 
}*/


int playerInTournamentGetWins (PlayerInTournament player_in_tournament)
{
    return player_in_tournament->wins;
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