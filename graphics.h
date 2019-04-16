/**
 * the module for controlling the game graphics, like displaying bitmaps
 * and using tinygl
 * @author jbr185
 */

#ifndef GRAPHICS_H
#define GRAPHICS_H

#include "ledmat.h"
#include "tinygl.h"
#include "display.h"
#include "pio.h"
#include "../fonts/font3x5_1.h"
#include <stdlib.h>

#define DISPLAY_RATE 300 //60 frames per second

#define WIN 0 //the the result indices
#define LOSE 1
#define DRAW 2
#define READY 3 //the index for the ready animation 

/**
 * initalizes all the elements of the graphics
 */
void gfx_init(void);

/**
 * sets tinygl text and sets into scroll mode
 * @param text, the string to display
 */
void gfx_set_text(char* text);

/**
 * sets the tinygl message text to a number and into step mode
 * @param num, the uint8 number to display
 */
void gfx_set_num(uint8_t num);

/**
 * just a pass through for updating tinygl
 */
void gfx_display_text(void);

/**
 * displays the symbol of the given choice
 * @param choice, the index for the system to display
 */
void gfx_display_symbol(uint8_t choice);

/**
 * displays the animation of the given type
 * @param type, the index for the type of animation to play
 */
void gfx_display_animation(uint8_t type);

#endif
