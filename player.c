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


static Map createPlayerInTournamentsMap()
{
    Map map = mapCreate(playerInTournamentCopyWrapper, intCopy,
                        playerInTournamentDestroyWrapper, intFree, intCompare );
    return map;
}

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


ChessResult playerAddGame(Player player, Game game)
{
    if (player == NULL || game == NULL)
    {
        return CHESS_NULL_ARGUMENT;
    }

    if (!gameisPlayerInGame(game, player->player_id))
    {
        return CHESS_PLAYER_NOT_EXIST; // PlayerResult?
    }

    int tournament_id = gameGetTournamentID(game);
    if (mapContains(player->player_in_tournaments, &tournament_id))
    {
        printf("continue here\n");
    }

    //playerInTournamentAddGame();

    return CHESS_SUCCESS;

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