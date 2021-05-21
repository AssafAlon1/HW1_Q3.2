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

Player playerCreate(int player_id)
{
    Player player = malloc(sizeof(player));

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


ChessResult playerAddGame(Player player, Game game, int max_games_per_player)
{
    if (player == NULL || game == NULL)
    {
        return CHESS_NULL_ARGUMENT;
    }

    if (!gameisPlayerInGame(game, player->player_id))
    {
        return CHESS_PLAYER_NOT_EXIST; // PlayerResult?
    }

    // If the player has never played in the tournament tournament,
    // create instance of playerInTournament for the specific tournament
    int tournament_id = gameGetTournamentID(game);
    if (!mapContains(player->player_in_tournaments, &tournament_id))
    {
        PlayerInTournament player_in_tournament = playerInTournamentCreate(
                    player->player_id, tournament_id, max_games_per_player);
        
        if(player_in_tournament == NULL)
        {
            return CHESS_OUT_OF_MEMORY;
        }

        // Put player in tournament map and verify success
        MapResult put_result = mapPut(player->player_in_tournaments, &tournament_id, player_in_tournament);
        if (put_result == MAP_OUT_OF_MEMORY)
        {
            playerInTournamentDestroy(player_in_tournament);
            return CHESS_OUT_OF_MEMORY;
        }

        playerInTournamentDestroy(player_in_tournament); // A copy was sent to the map
    }
    
    ChessResult add_game_result = playerInTournamentAddGame(mapGet(
                                player->player_in_tournaments, &tournament_id), game);

    return add_game_result;
}


int playerGetTotalGames(Player player)
{
    return player->total_wins + player->total_losses + player->total_draws;
}

double playerGetFinishedGamesAverageTime(Player player)
{
    int amountOfGames = playerGetTotalGames(player);
    double time = (double)(player->total_game_time)/amountOfGames;
    return time;
}


double playerGetLevel(Player player)
{
    double level = (player->total_wins)*WIN_WEIGHT;
    level       -= (player->total_losses)*LOSS_WEIGHT;
    level       += (player->total_draws)*DRAW_WEIGHT;
    level       /=  playerGetTotalGames(player);

    return level;
}