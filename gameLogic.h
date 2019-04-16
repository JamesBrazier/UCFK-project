/**
 * module for the game logic functions and dealing with the game structure
 * @authors jbr185 & fya33
 */

#ifndef GAMELOGIC_H
#define GAMELOGIC_H

#include "system.h"

#define WIN 0 //the enumerated results
#define LOSE 1
#define DRAW 2

#define LIVES 3 //game constants
#define SYMBOL_COUNT 3
char choices[3];

/**
 * the structure holding the game values listed below
 */
typedef struct gameinfo {
    uint8_t choice; //your choice as an index for the symbol list
    char opponent; //the char
    uint8_t lives;
    uint8_t result; //the round result 0 for win, 1 for lose, 2 for draw
} GameObj;

/**
 * compare the symbol to the other, as per the classic rules
 * Paper beats Rock, Rock beats Scissors, Scissors beats Paper
 * @param a & b, the two chars to be compared
 */
uint8_t compare(char a, char b);

/**
 * checks if the given game is a valid symbol for the opponent
 * @param game, the game to check
 */
uint8_t opponent_valid(const GameObj* game);

/**
 * resets all the values in the game to defaults
 * @param game, the game to reset
 */
void game_reset(GameObj* game);

#endif
