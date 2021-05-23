#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "map.h"
#include "mapUtil.h"
#include "tournament.h"
#include "player.h"
#include "string.h"
#include <limits.h>


struct tournament_t {
    int tournament_id;
    Map games;
    int max_games_per_player;
    int winner;
    int longest_game;
    int total_game_time;
    char *location;
    int current_game_id;
    int amount_of_players;
};


//================== INTERNAL FUNCTIONS START ==================//

// static Map createGamesMap()
// {
//     Map map = mapCreate(gameCopyWrapper, intCopy,
//                         gameDestroyWrapper, intFree, intCompare );
//     return map;
// }


// Get the length (amount of digits) of the longest int
// static int maxLengthOfInt()
// {
//     int length = 0;
//     int number = INT_MAX;
//     while (number != 0)
//     {
//         length++;
//         number /= 10;
//     }
//     return length;
// }


// Checks if a char* points to a uppercased
static bool isUpper(const char *character)
{
    if (character == NULL)
    {
        return false;
    }
    
    return *character >= 'A' && *character <= 'Z';
}

// Checks if a char* points to a lowercased
static bool isLower(const char *character)
{
    if (character == NULL)
    {
        return false;
    }
    
    return *character >= 'a' && *character <= 'z';
}

// Check if a char* points to a space
static bool isSpace(const char *character)
{
    if (character == NULL)
    {
        return false;
    }
    
    return *character == ' ';
}

static char* copyLocation (const char *location)
{
    // Location is NULL / invalid by the rules we applied
    if (!tournamentValidateLocation(location))
    {
        return NULL;
    }

    char* new_location = malloc(strlen(location)+1);
    if (new_location == NULL)
    {
        return NULL;
    }

    new_location = strcpy(new_location, location);
    return new_location;
}

//================== INTERNAL FUNCTIONS END ==================//





Tournament tournamentCreate(int tournament_id, int max_games_per_player,
                            const char *tournament_location)
{
    Tournament tournament = malloc(sizeof(*tournament));

    //check if the tournament allocation failed 
    if (tournament == NULL)
    {
        return NULL;
    }
    
    tournament->games = createGamesMap();
    
    //check if thr games map allocation failed
    if(tournament->games == NULL)
    {
        free(tournament);
        return NULL;
    }


    if (!tournamentValidateLocation(tournament_location))
    {
        return NULL;
    }

    tournament->location             = copyLocation(tournament_location);
    if (tournament->location == NULL)
    {
        return NULL;
    }

    tournament->tournament_id        = tournament_id;
    tournament->max_games_per_player = max_games_per_player;
    tournament->winner               = INVALID_PLAYER;
    tournament->longest_game         = 0;
    tournament->total_game_time      = 0;
    tournament->current_game_id      = 0;
    tournament->amount_of_players    = 0;

    return tournament;
} 


void tournamentDestroy(Tournament tournament)
{
    if (tournament == NULL)
    {
        return;
    }
    mapDestroy(tournament->games);
    free(tournament->location);
    free(tournament);
}


Tournament tournamentCopy(Tournament tournament)
{
    if (tournament == NULL)
    {
        return NULL;
    }
    Tournament new_tournament = tournamentCreate(tournament->tournament_id,
                                tournament->max_games_per_player, tournament->location);
    if (new_tournament == NULL)
    {
        return NULL;
    }

    new_tournament->games = mapCopy(tournament->games);
    if (new_tournament->games == NULL)
    {
        free(new_tournament);
        return NULL;
    }
    
    new_tournament->location = copyLocation(tournament->location);
    if (new_tournament->location == NULL)
    {
        mapDestroy(new_tournament->games);
        free(new_tournament);
        return NULL;
    }

    new_tournament->amount_of_players = tournament->amount_of_players;
    new_tournament->current_game_id   = tournament->current_game_id;
    new_tournament->longest_game      = tournament->longest_game;
    new_tournament->total_game_time   = tournament->total_game_time;
    new_tournament->winner            = tournament->winner;


    return new_tournament;
}


ChessResult tournamentAddGame(Tournament tournament, int first_player, int second_player,
                              Winner winner, int play_time, int amount_of_new_players)
{
    if (tournament == NULL)
    {
        return CHESS_NULL_ARGUMENT;
    }
    
    if (first_player <= 0 || second_player <= 0 || first_player == second_player)
    {
        return CHESS_INVALID_ID;
    }

    if (play_time <= 0)
    {
        return CHESS_INVALID_PLAY_TIME;
    }

    if (tournament->winner != INVALID_PLAYER)
    {
        return CHESS_TOURNAMENT_ENDED;
    }

    //assert game is valid


    Game new_game = gameCreate(tournament->tournament_id, first_player, second_player,
               winner, play_time, tournament->current_game_id);
    
    if (new_game == NULL)
    {
        return CHESS_OUT_OF_MEMORY;
    }

    mapPut(tournament->games, &(tournament->current_game_id), new_game);
    gameDestroy(new_game);

    if (play_time > tournament->longest_game)
    {
        tournament->longest_game = play_time;
    }
    (tournament->current_game_id)++;
    tournament->total_game_time   += play_time;
    tournament->amount_of_players += amount_of_new_players;

    return CHESS_SUCCESS;
}


ChessResult tournamentRemovePlayer(Tournament tournament, int player_id, int game_ids[])
{
    // Input verification
    if (tournament == NULL || game_ids == NULL)
    {
        return CHESS_NULL_ARGUMENT;
    }

    if (player_id <= 0)
    {
        return CHESS_INVALID_ID;
    }
    
    if (tournament->winner != INVALID_PLAYER)
    {
        return CHESS_TOURNAMENT_ENDED;
    }


    // Scan games, remove player from games
    for (int i = 0 ; i < tournament->max_games_per_player ; i++)
    {
        // Extra breaking point
        if (game_ids[i] == INVALID_GAME_ID)
        {
            break;
        }
        Game game = mapGet(tournament->games, &game_ids[i]);
        if (game == NULL)
        {
            return CHESS_INVALID_ID; // NEVER GET HERE?????
        }

        gameRemovePlayer(game, player_id);
    }

    return CHESS_SUCCESS;
}


ChessResult tournamentEnd (Tournament tournament, int winner_id)
{
    if (tournament == NULL)
    {
        return CHESS_NULL_ARGUMENT;
    }

    // Checks if there were games played in the tournament
    if (tournament->current_game_id == 0)
    {
        return CHESS_NO_GAMES;
        
    }
    
    tournament->winner = winner_id;
    return CHESS_SUCCESS;
}




int tournamentGetSizePlayers (Tournament tournament)
{
    return tournament->amount_of_players;
}


int tournamentGetSizeGames (Tournament tournament)
{
    return tournament->current_game_id;
}


int tournamentGetWinner(Tournament tournament)
{
    return tournament->winner;
}

char* tournamentGetLocation(Tournament tournament)
{
    return tournament->location;
}

int tournamentGetLongestGameTime(Tournament tournament)
{
    return tournament->longest_game;
}

double tournamentGetAverageGameTime(Tournament tournament)
{
    if(tournament->current_game_id == 0)
    {
        return 0;
    }
    return (double)tournament->total_game_time / tournament->current_game_id;
}


bool tournamentValidateLocation(const char *location)
{
    if (location == NULL)
    {
        return false;
    }
    
    // Verify first letter is uppercased
    if (!isUpper(location))
    {
        return false;
    }

    // Verify the rest of the letters are lowercased/spaces
    location++;
    while (*location)
    {
        // If current char is not a lowercased one and not a space, the char is invalid
        if (!isLower(location) && !isSpace(location))
        {
            return false;
        }
        location++;
    }
    return true;
}


int tournamentGetMaxGamesPerPlayer(Tournament tournament)
{
    if (tournament == NULL)
    {
        return -1;
    }
    return tournament->max_games_per_player;
}


Game tournamentGetGame(Tournament tournament, int game_id)
{
    if (tournament == NULL)
    {
        return NULL;
    }

    return mapGet(tournament->games, &game_id);
}


bool tournamentPrintStatsToFile(Tournament tournament, FILE *output_file)
{
    double average_game_time = tournamentGetAverageGameTime(tournament);
    int fprintf_output = 0;
    fprintf_output = fprintf(output_file,"%d\n%d\n%.2f\n",
                            tournament->winner,
                            tournament->longest_game,
                            average_game_time);

    if (fprintf_output < 0)
    {
        return false;
    }

    fprintf_output = fprintf(output_file, "%s\n%d\n%d\n"  ,
                            tournament->location,
                            tournament->current_game_id,
                            tournament->amount_of_players);

    if (fprintf_output < 0)
    {
        return false;
    }

    return true;
}