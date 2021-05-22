#include <stdio.h>
#include <stdlib.h>
#include "game.h"
#include "player.h"
#include "map.h"
#include "mapUtil.h"
#include "playerInTournament.h"


struct player_t {
    int player_id;
    int total_wins;
    int total_draws;
    int total_losses;
    int total_game_time;
    Map player_in_tournaments; 
};


// static Map createPlayerInTournamentsMap()
// {
//     Map map = mapCreate(playerInTournamentCopyWrapper, intCopy,
//                         playerInTournamentDestroyWrapper, intFree, intCompare );
//     return map;
// }


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


// Creates an instance of the player in a tournament
static PlayerResult playerNewTournament(Player player, int tournament_id, int max_games_per_player)
{
    // Create PlayerInTournament
    PlayerInTournament player_in_tournament = playerInTournamentCreate(
                player->player_id, tournament_id, max_games_per_player);
    
    if(player_in_tournament == NULL)
    {
        return PLAYER_OUT_OF_MEMORY;
    }

    // Put PlayerInTournament in map and verify success
    MapResult put_result = mapPut(player->player_in_tournaments, &tournament_id, player_in_tournament);
    if (put_result == MAP_OUT_OF_MEMORY)
    {
        playerInTournamentDestroy(player_in_tournament);
        return PLAYER_OUT_OF_MEMORY;
    }

    playerInTournamentDestroy(player_in_tournament); // A copy was sent to the map
    return PLAYER_SUCCESS;
}



Player playerCreate(int player_id)
{
    Player player = malloc(sizeof(*player));

    //check if the player allocation failed
    if (player == NULL)
    {
        return NULL;
    }

    player->player_in_tournaments = createPlayerInTournamentsMap();

    //check if the player in tournament allocation failed 
    if(player->player_in_tournaments == NULL)
    {
        free(player);
        return NULL;
    }

    player->player_id       = player_id;
    player->total_wins      = 0;
    player->total_draws     = 0;
    player->total_losses    = 0;
    player->total_game_time = 0;

    return player;
}


void playerDestroy(Player player)
{
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

    Player new_player = playerCreate(player->player_id);

    // Verify creation
    if (new_player == NULL)
    {
        return NULL;
    }

    new_player->player_in_tournaments = createPlayerInTournamentsMap();
    if (new_player->player_in_tournaments == NULL)
    {
        free(new_player);
        return NULL;
    }

    new_player->total_wins      = 0;
    new_player->total_draws     = 0;
    new_player->total_losses    = 0;
    new_player->total_game_time = 0;

    return new_player;
}


PlayerResult playerAddGame(Player player, Game game, int max_games_per_player)
{
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
    if (!mapContains(player->player_in_tournaments, &tournament_id))
    {
        PlayerResult result_new_tournament = playerNewTournament(player, tournament_id, max_games_per_player);
        if (result_new_tournament != PLAYER_SUCCESS)
        {
            return result_new_tournament;
        }
    }
    
    // Add the game's result to the corresponding tournament 
    PlayerInTournamentResult add_game_result = playerInTournamentAddGame(mapGet(
                                player->player_in_tournaments, &tournament_id), game);
    
    
    switch (add_game_result)
    {
    case PLAYER_IN_TOURNAMENT_SUCCESS:
        break;
    
    case PLAYER_IN_TOURNAMENT_OUT_OF_MEMORY:
        return PLAYER_OUT_OF_MEMORY;

    case PLAYER_IN_TOURNAMENT_GAME_ALREADY_EXISTS:
        return PLAYER_GAME_ALREADY_EXISTS;

    case PLAYER_IN_TOURNAMENT_EXCEEDED_GAMES:
        return PLAYER_EXCEEDED_GAMES;

    default:
        return PLAYER_GENERAL_ERROR;
        break;
    }

    // Update statistics for player in the case of success
    playerUpdateStatsForGame(player, game);
    return PLAYER_SUCCESS;
}


int playerGetTotalGames(Player player)
{
    return player->total_wins + player->total_losses + player->total_draws;
}

double playerGetFinishedGamesAverageTime(Player player)
{
    if (player == NULL)
    {
        return -1;
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
        return -1;
    }

    int amount_of_games = playerGetTotalGames(player);
    if (amount_of_games == 0)
    {
        return 0;
    }

    double level = (player->total_wins)*WIN_WEIGHT;
    level       += (player->total_losses)*LOSS_WEIGHT;
    level       += (player->total_draws)*DRAW_WEIGHT;
    level       /= amount_of_games;

    return level;
}



PlayerResult playerRemoveTournament(Player player, int tournament_id)
{
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
    MapResult remove_result = mapRemove(player->player_in_tournaments, &tournament_id);
    playerInTournamentDestroy(player_in_tournament);

    if (remove_result != MAP_SUCCESS)
    {
        return PLAYER_GENERAL_ERROR; //Shouldn't get here
    }

    return PLAYER_SUCCESS;
}