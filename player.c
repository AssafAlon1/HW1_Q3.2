#include <stdio.h>
#include <stdlib.h>


#include "./mtm_map/map.h"
#include "mapUtil.h"
#include "player.h"
#include "game.h"
#include "playerInTournament.h"


struct player_t {
    int player_id;
    int total_wins;
    int total_draws;
    int total_losses;
    int total_game_time;
    Map player_in_tournaments; 
};


//==============================================================//
//================== INTERNAL FUNCTIONS START ==================//
//==============================================================//


// Updates Wins/Draws/Losses according to the game outcome
static bool playerUpdateStatsForGame(Player player, Game game)
{
    if (player == NULL || game == NULL)
    {
        return false;
    }
    int id_of_winner = gameGetIdOfWinner(game);
    if (id_of_winner == player->player_id)  // Win
    {
        player->total_wins += 1;
    }
    else
    {
        if (id_of_winner == INVALID_PLAYER) // Draw
        {
            player->total_draws += 1;
        }
        else                                // Loss
        {
            player->total_losses += 1;
        }
    }
    player->total_game_time += gameGetPlayTime(game);
    return true;
}


// Returns the playerInTournament for a given player and a given tournament id
static PlayerInTournament getPlayerInTournament(Player player, int tournament_id)
{
    if (player == NULL)
    {
        return NULL;
    }

    PlayerInTournament player_in_tournament = mapGet(player->player_in_tournaments, &tournament_id);
    return player_in_tournament;
}

// Translate error code from PlayerInTournament to Player
static PlayerResult translatePlayerInTournamentToPlayer(PlayerInTournamentResult result)
{
    switch (result)
    {
        case PLAYER_IN_TOURNAMENT_OUT_OF_MEMORY:
            return PLAYER_OUT_OF_MEMORY;
        case PLAYER_IN_TOURNAMENT_NULL_ARGUMENT:
            return PLAYER_NULL_ARGUMENT;
        case PLAYER_IN_TOURNAMENT_INVALID_ID:
            return PLAYER_INVALID_ID;
        case PLAYER_IN_TOURNAMENT_GAME_ALREADY_EXISTS:
            return PLAYER_GAME_ALREADY_EXISTS;
        case PLAYER_IN_TOURNAMENT_EXCEEDED_GAMES:
            return PLAYER_EXCEEDED_GAMES;
        case PLAYER_IN_TOURNAMENT_SUCCESS:
            return PLAYER_SUCCESS;
        default: // Shouldn't get here
            return PLAYER_GENERAL_ERROR;
    }

}

//============================================================//
//================== INTERNAL FUNCTIONS END ==================//
//============================================================//




Player playerCreate(int player_id)
{
    // Creates a new player
    Player player = malloc(sizeof(*player));
    if (player == NULL)
    {
        return NULL;
    }

    //Create a new playerInTournament map for the player
    player->player_in_tournaments = createPlayerInTournamentsMap();
    if(player->player_in_tournaments == NULL)
    {
        free(player);
        return NULL;
    }

    // Initializing fields
    player->player_id       = player_id;
    player->total_wins      = 0;
    player->total_draws     = 0;
    player->total_losses    = 0;
    player->total_game_time = 0;

    return player;
}


void playerDestroy(Player player)
{
    if (player == NULL)
    {
        return;
    }
   mapDestroy(player->player_in_tournaments);
   free(player); 
}


Player playerCopy(Player player)
{
    // Verify input
    if (player == NULL)
    {
        return NULL;
    }

    // Create new player
    Player new_player = playerCreate(player->player_id);
    if (new_player == NULL)
    {
        return NULL;
    }

    // Copy playerInTournament map
    Map player_in_tournament_map = mapCopy(player->player_in_tournaments);
    if (player_in_tournament_map == NULL)
    {
        free(new_player);
        return NULL;
    }
    mapDestroy(new_player->player_in_tournaments);
    new_player->player_in_tournaments = player_in_tournament_map;

    // Copy fields
    new_player->total_wins      = player->total_wins;
    new_player->total_draws     = player->total_draws;
    new_player->total_losses    = player->total_losses;
    new_player->total_game_time = player->total_game_time;

    return new_player;
}


PlayerResult playerAddGame(Player player, Game game /*int max_games_per_player*/)
{
    // Verify input
    if (player == NULL || game == NULL)
    {
        return PLAYER_NULL_ARGUMENT;
    }

    if (!gameisPlayerInGame(game, player->player_id))
    {
        return PLAYER_NOT_IN_GAME;
    }

    // If the player has never played in the tournament tournament,
    // create instance of playerInTournament for the specific tournament
    int tournament_id = gameGetTournamentID(game);
    if(!(playerIsPlayingInTournament(player, tournament_id)))
    {
        return PLAYER_TOURNAMENT_NOT_EXIST;
    }
    
    // Add the game's result to the relevant playerInTournament struct
    PlayerInTournamentResult add_game_result = playerInTournamentAddGame(mapGet(
                                player->player_in_tournaments, &tournament_id), game);
    
    
    PlayerResult translated_add_game_result = translatePlayerInTournamentToPlayer(add_game_result);
    if (translated_add_game_result != PLAYER_SUCCESS)
    {
        return translated_add_game_result;
    }

    // Update statistics for player in the case of success
    playerUpdateStatsForGame(player, game);
    return PLAYER_SUCCESS;
}


int playerGetTotalGames(Player player)
{
    if (player == NULL)
    {
        return PLAYER_INVALID_INPUT;
    }
    return player->total_wins + player->total_losses + player->total_draws;
}


double playerGetFinishedGamesAverageTime(Player player)
{
    if (player == NULL)
    {
        return PLAYER_INVALID_INPUT;
    }

    int amount_of_games = playerGetTotalGames(player);
    if (amount_of_games == 0)
    {
        return 0;
    }

    double time = (double)(player->total_game_time) / amount_of_games;
    return time;
}


double playerGetLevel(Player player)
{
    if (player == NULL)
    {
        return PLAYER_INVALID_INPUT;
    }
    if (player->player_id == 30)
    {
        printf("win %d  \ndraw %d  \nloss %d\n", player->total_wins, player->total_draws, player->total_losses);
    }
    int amount_of_games = playerGetTotalGames(player);
    if (amount_of_games == 0)
    {
        return 0;
    }

    double level = (player->total_wins)   *WIN_WEIGHT;
    level       += (player->total_losses) *LOSS_WEIGHT;
    level       += (player->total_draws)  *DRAW_WEIGHT;
    level       /= amount_of_games;

    return level;
}


PlayerResult playerRemoveTournament(Player player, int tournament_id)
{
    // Verify input
    if (player == NULL)
    {
        return PLAYER_NULL_ARGUMENT;
    }

    PlayerInTournament player_in_tournament = mapGet(player->player_in_tournaments, &tournament_id);
    if (player_in_tournament == NULL)
    {
        return PLAYER_TOURNAMENT_NOT_EXIST;
    }

    // Update stats
    player->total_wins      -= playerInTournamentGetWins(player_in_tournament);
    player->total_draws     -= playerInTournamentGetDraws(player_in_tournament);
    player->total_losses    -= playerInTournamentGetLosses(player_in_tournament);
    player->total_game_time -= playerInTournamentGetTotalTime(player_in_tournament);

    // Cleanup
    mapRemove(player->player_in_tournaments, &tournament_id);
    player_in_tournament    = NULL;

    return PLAYER_SUCCESS;
}


int playerGetID(Player player)
{
    if (player == NULL)
    {
        return PLAYER_INVALID_INPUT;
    }
    return player->player_id;
}


bool playerIsPlayingInTournament(Player player, int tournament_id)
{
    PlayerInTournament player_in_tournament = getPlayerInTournament(player, tournament_id);
    return player_in_tournament != NULL ? true : false;
}


int* playerGetGameIdsInTournament(Player player, int tournament_id)
{
    if (player == NULL)
    {
        return NULL;
    }

    PlayerInTournament player_in_tournament = mapGet(player->player_in_tournaments, &tournament_id);
    if (player_in_tournament == NULL)
    {
        return NULL;
    }

    return playerInTournamentGetGameIds(player_in_tournament);
}


bool playerCanPlayMoreGamesInTournament(Player player, int tournament_id)
{
    if (player == NULL)
    {
        return false;
    }

    PlayerInTournament player_in_tournament = mapGet(player->player_in_tournaments, &tournament_id);
    
    return playerInTournamentCanPlayMore(player_in_tournament);
}


PlayerResult playerAddTournament(Player player, int tournament_id, int max_games_per_player)
{
    // Verify input
    if (player == NULL)
    {
        return PLAYER_NULL_ARGUMENT;
    }

    if (mapContains(player->player_in_tournaments, &tournament_id))
    {
        return PLAYER_TOURNAMENT_ALREADY_EXISTS;
    }
    
    // Create new playerInTournament
    PlayerInTournament player_in_tournament = playerInTournamentCreate(
                       player->player_id, tournament_id, max_games_per_player);
    
    if (player_in_tournament == NULL)
    {
        return PLAYER_OUT_OF_MEMORY;
    }

    MapResult put_result = mapPut(player->player_in_tournaments,
                                 &tournament_id, player_in_tournament);
    
    playerInTournamentDestroy(player_in_tournament);
    if (put_result != MAP_SUCCESS)
    {
        return PLAYER_OUT_OF_MEMORY;
    }

    return PLAYER_SUCCESS;
}


bool playerUpdateResultsAfterOpponentDeletion(Player player, int tournament_id, int outcome_change)
{
    if (player == NULL)
    {
        return false;
    }

    PlayerInTournament player_in_tournament = mapGet(player->player_in_tournaments, &tournament_id);
    if (player_in_tournament == NULL)
    {
        return false;
    }


    if (outcome_change == DRAW_TO_WIN)
    {
        bool result = playerInTournamentUpdateDrawToWin(player_in_tournament);
        if (!result)
        {
            return false;
        }
        player->total_draws -= 1;
        player->total_wins  += 1;
    }

    if (outcome_change == LOSS_TO_WIN)
    {
        bool result = playerInTournamentUpdateLossToWin(player_in_tournament);
        if (!result)
        {
            return false;
        }
        player->total_losses -= 1;
        player->total_wins  += 1;
    }

    return true;
}


int *playerGetFirstTournamentID(Player player)
{
    if (player == NULL)
    {
        return NULL;
    }
    return mapGetFirst(player->player_in_tournaments);
}

int *playerGetNextTournamentID(Player player)
{
    if (player == NULL)
    {
        return NULL;
    }
    return mapGetNext(player->player_in_tournaments);
}


int playerGetWinsInTournament(Player player, int tournament_id)
{
    if (player == NULL)
    {
        return PLAYER_INVALID_INPUT;
    }

    PlayerInTournament player_in_tournament = mapGet(player->player_in_tournaments, &tournament_id);
    return playerInTournamentGetWins(player_in_tournament);
}

int playerGetDrawsInTournament(Player player, int tournament_id)
{
    if (player == NULL)
    {
        return PLAYER_INVALID_INPUT;
    }

    PlayerInTournament player_in_tournament = mapGet(player->player_in_tournaments, &tournament_id);
    return playerInTournamentGetDraws(player_in_tournament);
}

int playerGetLossesInTournament(Player player, int tournament_id)
{
    if (player == NULL)
    {
        return PLAYER_INVALID_INPUT;
    }
    PlayerInTournament player_in_tournament = mapGet(player->player_in_tournaments, &tournament_id);
    return playerInTournamentGetLosses(player_in_tournament);
}

