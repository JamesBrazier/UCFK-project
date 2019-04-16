/**
 * the module for playing sound of the game
 * @author jbr185
 */

#include "sound.h"
#include "mmelody.h"
#include "pio.h"
#include "tweeter.h"
#include <stdlib.h>

#define PIEZO_PIO PIO_DEFINE(PORT_D, 6)
/*the piezo, use either end of the extention board, with the black wire
closest to the matrix for best result*/

static tweeter_t tweeter; //module objects
static mmelody_t melody;

/**
 * the list containing the tracks
 */
char* tunes[3] = {
    "C,A///", //win
    "C,F,C///", //lose
    "C,C///", //draw
};

/**
 * initialize the tweeter, piezo and melody modules
 */
void sound_init(void)
{
    tweeter_scale_t scaleTable[] = TWEETER_SCALE_TABLE(TWEETER_RATE);
    mmelody_obj_t melodyInfo; //objects for storing data for the modules
    tweeter_obj_t tweeterInfo;
    
    tweeter = tweeter_init(&tweeterInfo, TWEETER_RATE, scaleTable);
    pio_config_set(PIEZO_PIO, PIO_OUTPUT_LOW);
    melody = mmelody_init(&melodyInfo, MELODY_RATE, 
                    (mmelody_callback_t) tweeter_note_play, tweeter);
    mmelody_speed_set(melody, MELODY_BPM_RATE);
}

/**
 * update the tweeter and piezo, and the move the melody occationally
 * based on the melody counter variable
 */
void sound_update(void)
{
    static uint8_t melodyCounter = 0; //the counter to delay the melody update
    
    melodyCounter++;
    pio_output_set(PIEZO_PIO, tweeter_update(tweeter));
    if (melodyCounter >= TWEETER_RATE / MELODY_RATE) { //delay the change of the melody
        melodyCounter = 0;
        mmelody_update(melody);
    }
}

/**
 * changes the track being played
 * @param tune, the string for melody to play, string of notes
 */
void sound_set_track(char* tune)
{
    mmelody_play(melody, tune);
}

/**
 * plays a blank track to effectively clear whats being played
 */
void sound_clear_trck(void)
{
    mmelody_play(melody, 0);
}

