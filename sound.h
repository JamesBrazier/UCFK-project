/**
 * the module for playing sound of the game
 * @author jbr185
 */

#ifndef SOUND_H
#define SOUND_H

#include "mmelody.h"
#include "pio.h"
#include "tweeter.h"

#define TWEETER_RATE 15000 //update rates for the given modules
#define MELODY_RATE 250
#define MELODY_BPM_RATE 200

#define WIN 0 //the indices for results to be used with the tracks array
#define LOSE 1
#define DRAW 2

/**
 * the list containing the tracks
 */
char* tunes[3];

/**
 * initialize the tweeter, piezo and melody modules
 */
void sound_init(void);

/**
 * update the tweeter and piezo, and the move the melody occationally
 * based on the melody counter variable
 */
void sound_update(void);

/**
 * changes the track being played
 * @param tune, the string for melody to play, string of notes
 */
void sound_set_track(char* tune);

/**
 * plays a blank track to effectively clear whats being played
 */
void sound_clear_track(void);

#endif
