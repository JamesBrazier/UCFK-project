/**
 * module for the game logic functions and dealing with the game structure
 * @authors jbr185 & fya33
 */

#include "system.h"
#include "gameLogic.h"

char choices[3] = {'P', 'S', 'R'}; //Paper, Scissors, and Rock

/**
 * compare the symbol to the other, as per the classic rules
 * Paper beats Rock, Rock beats Scissors, Scissors beats Paper
 * @param a & b, the two chars to be compared
 */
uint8_t compare(char a, char b)
{
    switch (a) {
    case 'S':
        switch (b) {  
        case 'S':
            return DRAW;
        case 'P':
            return WIN;
        case 'R':
            return LOSE;
        }
    case 'P':
        switch (b) {
        case 'S':
            return LOSE;
        case 'P':
            return DRAW;
        case 'R':
            return WIN;
        }
    case 'R':
        switch (b) {
        case 'S':
            return WIN;
        case 'P':
            return LOSE;
        case 'R':
            return DRAW;
        }
    }
    return DRAW; //if we don't have a result or invalid symbol, its a draw
}

/**
 * checks if the given game is a valid symbol for the opponent
 * @param game, the game to check
 */
uint8_t opponent_valid(const GameObj* game)
{
    return game->opponent == 'S' || game->opponent == 'R' || game->opponent == 'P';
}

/**
 * resets all the values in the game to defaults
 * @param game, the game to reset
 */
void game_reset(GameObj* game)
{
    game->choice = 0;
    game->opponent = 0;
    game->lives = LIVES;
    game->result = 0;
}
