#include <stdio.h>
#include <stdlib.h>

#include "playerInTournament.h"


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

//==============================================================//
//================== INTERNAL FUNCTIONS START ==================//
//==============================================================//

// Checks whether a player played a game with a given ID
static bool isGameExists(PlayerInTournament player_in_tournament, int game_id)
{
    int game_count = playerInTournamentGetTotalGames(player_in_tournament);
    for (int i = 0 ; i < game_count ; i++)
    {
        if((player_in_tournament->game_ids)[i] == game_id)
        {
            return true; // Game exists
        }
    }
    return false;
}

// Verify the input for the playerInTournamentAddGame function
static PlayerInTournamentResult playerInTournamentAddGameInputVerification(PlayerInTournament player_in_tournament, Game game)
{
    // Verify input not NULL
    if (player_in_tournament == NULL || game == NULL)
    {
        return PLAYER_IN_TOURNAMENT_NULL_ARGUMENT;
    }
    
    // Verify the player is in the game
    if (!gameisPlayerInGame(game, player_in_tournament->player_id))
    {
        return PLAYER_IN_TOURNAMENT_PLAYER_NOT_IN_GAME;
    }

    // Verify game ID is unique
    if (isGameExists(player_in_tournament, gameGetID(game)))
    {
        return PLAYER_IN_TOURNAMENT_GAME_ALREADY_EXISTS;
    }

    // Verify game count is valid
    int game_count = playerInTournamentGetTotalGames(player_in_tournament);
    if (game_count >= player_in_tournament->max_games_per_player)
    {
        return PLAYER_IN_TOURNAMENT_EXCEEDED_GAMES;
    }

    // Verify game and player belong to the same tournament
    if (player_in_tournament->tournament_id != gameGetTournamentID(game))
    {
        return PLAYER_IN_TOURNAMENT_CONFLICT_ID;
    }

    return PLAYER_IN_TOURNAMENT_SUCCESS;
}

//============================================================//
//================== INTERNAL FUNCTIONS END ==================//
//============================================================//



PlayerInTournament playerInTournamentCreate(int player_id, int tournament_id, int max_games_per_player)
{
    // Allocate new player_in_tournament
    PlayerInTournament player_in_tournament = malloc(sizeof(*player_in_tournament));
    if (player_in_tournament == NULL)
    {
        return NULL;
    }
    
    // Allocate game_ids array
    player_in_tournament->game_ids = malloc(max_games_per_player*sizeof(int));
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

    // Initializing fields
    player_in_tournament->player_id = player_id;
    player_in_tournament->tournament_id = tournament_id;
    player_in_tournament->max_games_per_player = max_games_per_player;
    player_in_tournament->wins   = 0;
    player_in_tournament->draws  = 0;
    player_in_tournament->losses = 0;
    player_in_tournament->total_game_time = 0;

    return player_in_tournament;
}


void playerInTournamentDestroy(PlayerInTournament player_in_tournament)
{
    if (player_in_tournament == NULL)
    {
        return;
    }
    free(player_in_tournament->game_ids);
    free(player_in_tournament);
}


PlayerInTournament playerInTournamentCopy(PlayerInTournament player_in_tournament)
{
    if (player_in_tournament == NULL)
    {
        return NULL;
    }

    // Create new player_in_tournament
    PlayerInTournament new_player_in_tournament = playerInTournamentCreate(
                    player_in_tournament->player_id, player_in_tournament->tournament_id,
                    player_in_tournament->max_games_per_player);
    
    if (new_player_in_tournament == NULL)
    {
        return NULL;
    }

    // Copy fields
    new_player_in_tournament->draws  = player_in_tournament->draws;
    new_player_in_tournament->wins   = player_in_tournament->wins;
    new_player_in_tournament->losses = player_in_tournament->losses;
    new_player_in_tournament->total_game_time = player_in_tournament->total_game_time;

    // Copy games
    for (int i = 0 ; i < player_in_tournament->max_games_per_player ; i++)
    {
        // break if no more games were played
        if (player_in_tournament->game_ids[i] == INVALID_GAME_ID)
        {
            break;
        }
        new_player_in_tournament->game_ids[i] = player_in_tournament->game_ids[i];
    }

    return new_player_in_tournament;
}


PlayerInTournamentResult playerInTournamentAddGame(PlayerInTournament player_in_tournament, Game game)
{
    // Input verification through helper function
    PlayerInTournamentResult verification_result = playerInTournamentAddGameInputVerification(player_in_tournament, game);
    if (verification_result != PLAYER_IN_TOURNAMENT_SUCCESS)
    {
        return verification_result;
    }

    // Update statistics (play time, game count & wins / losses / draws)
    int game_count = playerInTournamentGetTotalGames(player_in_tournament);
    (player_in_tournament->game_ids)[game_count] = gameGetID(game);
    player_in_tournament->total_game_time += gameGetPlayTime(game);

    int winner_id = gameGetIdOfWinner(game);
    if (winner_id == player_in_tournament->player_id)
    {
        (player_in_tournament->wins)++;
    }
    else
    {
        if (winner_id == INVALID_PLAYER)
        {
            (player_in_tournament->draws)++;
        }
        else
        {
            (player_in_tournament->losses)++;
        }
    }

    return PLAYER_IN_TOURNAMENT_SUCCESS;
}


int playerInTournamentGetWins (PlayerInTournament player_in_tournament)
{
    if (player_in_tournament == NULL)
    {
        return 0;
    }
    return player_in_tournament->wins;
}

int playerInTournamentGetDraws (PlayerInTournament player_in_tournament)
{
    if (player_in_tournament == NULL)
    {
        return 0;
    }

    return player_in_tournament->draws;
}

int playerInTournamentGetLosses (PlayerInTournament player_in_tournament)
{
    if (player_in_tournament == NULL)
    {
        return 0;
    }

    return player_in_tournament->losses;
}

int playerInTournamentGetTotalTime (PlayerInTournament player_in_tournament)
{
    if (player_in_tournament == NULL)
    {
        return PLAYER_IN_TOURNAMENT_INVALID_INPUT;
    }

    return player_in_tournament->total_game_time;
}

int playerInTournamentGetTotalGames (PlayerInTournament player_in_tournament)
{
    if (player_in_tournament == NULL)
    {
        return PLAYER_IN_TOURNAMENT_INVALID_INPUT;
    }

    return player_in_tournament->wins + player_in_tournament->draws + player_in_tournament->losses;
}

int *playerInTournamentGetGameIds(PlayerInTournament player_in_tournament)
{
    if (player_in_tournament == NULL)
    {
        return NULL;
    }

    return player_in_tournament->game_ids;
}


bool playerInTournamentCanPlayMore(PlayerInTournament player_in_tournament)
{
    if(player_in_tournament == NULL)
    {
        return false;
    }
    return playerInTournamentGetTotalGames(player_in_tournament) < player_in_tournament->max_games_per_player;
}


int playerInTournamentGetTournamentID(PlayerInTournament player_in_tournament)
{
    if (player_in_tournament == NULL)
    {
        return PLAYER_IN_TOURNAMENT_INVALID_INPUT;
    }

    return player_in_tournament->tournament_id;
}


bool playerInTournamentUpdateDrawToWin(PlayerInTournament player_in_tournament)
{
    if (player_in_tournament == NULL || player_in_tournament->draws <= 0)
    {
        return false;
    }
    player_in_tournament->draws -= 1;
    player_in_tournament->wins  += 1;
    return true;
}

bool playerInTournamentUpdateLossToWin(PlayerInTournament player_in_tournament)
{
    if (player_in_tournament == NULL || player_in_tournament->losses <= 0)
    {
        return false;
    }
    player_in_tournament->losses -= 1;
    player_in_tournament->wins  += 1;
    return true;
}


PlayerInTournamentResult playerInTournamentRemoveLastGame(PlayerInTournament player_in_tournament, Game game)
{
    // Verify input not NULL
    if (player_in_tournament == NULL || game == NULL)
    {
        return PLAYER_IN_TOURNAMENT_NULL_ARGUMENT;
    }
    
    // Verify the player is in the game
    if (!gameisPlayerInGame(game, player_in_tournament->player_id))
    {
        return PLAYER_IN_TOURNAMENT_PLAYER_NOT_IN_GAME;
    }

    // Verify game and player belong to the same tournament
    if (player_in_tournament->tournament_id != gameGetTournamentID(game))
    {
        return PLAYER_IN_TOURNAMENT_CONFLICT_ID;
    }
    
    int winner = gameGetIdOfWinner(game);
    int game_id = playerInTournamentGetTotalGames(player_in_tournament);
    
    // Given game is not the last game
    if (gameGetID(game) != player_in_tournament->game_ids[game_id-1])
    {
        return PLAYER_IN_TOURNAMENT_CONFLICT_ID;
    }
    
    player_in_tournament->game_ids[game_id-1] = INVALID_GAME_ID;
    if (player_in_tournament->player_id == winner)
    {
        (player_in_tournament->wins)--;
        return PLAYER_IN_TOURNAMENT_SUCCESS;
    }

    if (winner == INVALID_PLAYER)
    {
        (player_in_tournament->draws)--;
        return PLAYER_IN_TOURNAMENT_SUCCESS;
    }
    (player_in_tournament->losses)--;
    return PLAYER_IN_TOURNAMENT_SUCCESS;
}