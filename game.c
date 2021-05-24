#include <stdio.h>
#include <stdlib.h>
#include "game.h"

struct game_t {
    int tournament_id;
    int game_id;
    int first_player;
    int second_player;
    int play_time;
    GameWinner winner;
};


Game gameCreate(int tournament_id, int first_player, int second_player, GameWinner winner, int play_time, int game_id)
{
    Game game = malloc(sizeof(*game));
    if (game == NULL)
    {
        return NULL;
    }

    game->tournament_id = tournament_id;
    game->game_id       = game_id;
    game->first_player  = first_player;
    game->second_player = second_player;
    game->play_time     = play_time;
    game->winner        = winner;

    return game;
}


void gameDestroy(Game game)
{
    if (game == NULL)
    {
        return;
    }
    free(game);
}


Game gameCopy(Game game)
{
    if (game == NULL)
    {
        return NULL;
    }

    Game new_game = gameCreate(game->tournament_id, game->first_player, game->second_player,
                               game->winner, game->play_time, game->game_id);
    
    return new_game;
}


GameResult gameRemovePlayer(Game game, int player_id)
{
    if (game == NULL)
    {
        return GAME_NULL_ARGUMENT;
    }

    if (player_id <= 0)
    {
        return GAME_INVALID_ID;
    }

    if (!gameisPlayerInGame(game, player_id))
    {
        return GAME_PLAYER_NOT_EXIST;
    }

    // Locate the player to remove and update values accordingly
    if (player_id == game->first_player)
    {
        game->first_player = DELETED_PLAYER;
        game->winner       = GAME_SECOND_PLAYER;
    }
    else
    {
        game->second_player = DELETED_PLAYER;
        game -> winner      = GAME_FIRST_PLAYER;
    }
    return GAME_SUCCESS;
}

int gameGetIdOfWinner(Game game)
{
    if (game == NULL)
    {
        return -1;
    }

    if (game->winner == GAME_FIRST_PLAYER)
    {
        return game->first_player;
    }

    if (game->winner == GAME_SECOND_PLAYER)
    {
        return game->second_player;
    }

    return INVALID_PLAYER;
}


int gameGetID(Game game)
{
    if (game == NULL)
    {
        return INVALID_GAME_ID;
    }
    return game->game_id;
}

int gameGetPlayTime(Game game)
{
    if (game == NULL)
    {
        return -1;
    }
    return game->play_time;
}


bool gameisPlayerInGame(Game game, int player_id)
{
    if (game == NULL || player_id <= 0)
    {
        return false;
    }

    if (game->first_player == player_id || game->second_player == player_id)
    {
        return true;
    }
    return false;
}

int gameGetTournamentID(Game game)
{
    if (game == NULL)
    {
        return -1;
    }
    return game->tournament_id;
}


int gameGetPlayersOpponent(Game game, int player_id)
{
    if (game == NULL)
    {
        return -1;
    }

    if (!gameisPlayerInGame(game, player_id))
    {
        return INVALID_PLAYER;
    }

    if (game->first_player == player_id)
    {
        return game->second_player;
    }

    return game->first_player;
}