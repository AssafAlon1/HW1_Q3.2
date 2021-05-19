#include <stdio.h>
#include "game.h"
#include "player.h"

struct game_t {
    int tournament_id;
    int game_id;
    int first_player;
    int second_player;
    int play_time;
    Winner winner;
};


Game gameCreate(int tournament_id, int first_player, int second_player, Winner winner, int play_time, int game_id)
{
    Game game = malloc(sizeof(game));
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
}


void gameDestroy(Game game)
{
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
    return game;
}


ChessResult gameRemovePlayer(Game game, int player_id)
{
    if (game == NULL)
    {
        return CHESS_NULL_ARGUMENT;
    }

    if (!gameisPlayerInGame(game, player_id))
    {
        return CHESS_PLAYER_NOT_EXIST;
    }

    // Locate the player to remove and update values accordingly
    int *player_to_remove = NULL;
    int *other_player = NULL;
    if (game->first_player == player_id)
    {
        player_to_remove = &(game->first_player);
        other_player     = &(game->second_player);
    }
    else
    {
        player_to_remove = &(game->second_player);
        other_player     = &(game->first_player);
    }

    *player_to_remove == INVALID_PLAYER;
    if (game->winner == player_id)
    {
        game->winner = *other_player;
    }

    return CHESS_SUCCESS;
}


Winner gameGetWinner(Game game)
{
    return game->winner;
}


int gameGetPlayTime(Game game)
{
    return game->play_time;
}


bool gameisPlayerInGame(Game game, int player_id)
{
    if (game == NULL)
    {
        return false;
    }

    if (game->first_player == player_id || game->second_player == player_id)
    {
        return true;
    }
    return false;
}