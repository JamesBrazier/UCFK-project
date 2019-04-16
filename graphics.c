/**
 * the module for controlling the game graphics, like displaying bitmaps
 * and using tinygl
 * @author jbr185
 */

#include "graphics.h"
#include "ledmat.h"
#include "tinygl.h"
#include "display.h"
#include "pio.h"
#include "uint8toa.h"
#include "../fonts/font3x5_1.h"
#include <stdlib.h>

#define ANIMATION_RATE 50 //the rate in which the animation frame changes
#define ANIMATION_FRAMES 4 //each animation must have four frames

static uint8_t symbols[3][5] = { //the bitmaps for the symbols
    {0b0110001, 0b1001111, 0b1000001, 0b1110001, 0b1001110}, //paper
    {0b0000011, 0b1110011, 0b0001100, 0b1110011, 0b0000011}, //scissors
    {0b0001000, 0b0010100, 0b0100100, 0b0100010, 0b0011100} //rock
};
static uint8_t animations[][4][5] = { //the animation bitmap frames
    {//animation for win
    {0b0010100, 0b0010100, 0b0000000, 0b0111110, 0b0000000},
    {0b0010100, 0b0010100, 0b0000000, 0b0100010, 0b0011100},
    {0b0000000, 0b0010100, 0b0000000, 0b0100010, 0b0011100},
    {0b0010100, 0b0010100, 0b0000000, 0b0100010, 0b0011100}}, 
    {//animation for a lose
    {0b0010100, 0b0010100, 0b0000000, 0b0111110, 0b0000000},
    {0b0010100, 0b0010100, 0b0000000, 0b0011100, 0b0100010},
    {0b0000000, 0b0010100, 0b0000000, 0b0011100, 0b0100010},
    {0b0010100, 0b0010100, 0b0000000, 0b0011100, 0b0100010}}, 
    {//animation for a draw
    {0b0010100, 0b0010100, 0b0000000, 0b0111110, 0b0000000},
    {0b0010100, 0b0010100, 0b0000000, 0b0111110, 0b0000000},
    {0b0000000, 0b0010100, 0b0000000, 0b0111110, 0b0000000},
    {0b0010100, 0b0010100, 0b0000000, 0b0111110, 0b0000000}}, 
    {//animation for ready and sending
    {0b0000000, 0b0000000, 0b0111000, 0b0111111, 0b0111111},
    {0b0000000, 0b0111000, 0b0111111, 0b0111111, 0b0000000},
    {0b0011100, 0b0111100, 0b0011111, 0b0001111, 0b0000000},
    {0b0000000, 0b0111000, 0b0111111, 0b0111111, 0b0000000}}
};

/**
 * initalizes all the elements of the graphics, like tinygl and its settings
 */
void gfx_init(void)
{
    tinygl_init(1000);
    tinygl_font_set(&font3x5_1);
    tinygl_text_speed_set(100);
    tinygl_text_dir_set(TINYGL_TEXT_DIR_ROTATE);
}

/**
 * sets tinygl text and sets into scroll mode
 * @param text, the string to display
 */
void gfx_set_text(char* text)
{
    tinygl_text_mode_set(TINYGL_TEXT_MODE_SCROLL);
    tinygl_clear();
    tinygl_text(text);
}

/**
 * sets the tinygl message text to a number and into step mode
 * @param num, the uint8 number to display
 */
void gfx_set_num(uint8_t num)
{
    char* numStr = "0";
    
    uint8toa(num, numStr, 0); //uses this to full the string with the char for the int
    tinygl_text_mode_set(TINYGL_TEXT_MODE_STEP); //step mode as we have a 1 char message
    tinygl_clear();
    tinygl_text(numStr);
}
           
/**
 * just a pass through for updating tinygl
 */
void gfx_display_text(void) 
{
    tinygl_update();
}

/**
 * displays the one column of the bit pattern, and removes the last
 * @param bitPattern, the bit pattern list to display
 * @param currentColumn, the column to display
 */
static void display_column(uint8_t bitPattern[])
{
    //the pins for the LEDmat rows
    static const pio_t rows[] = {LEDMAT_ROW1_PIO, LEDMAT_ROW2_PIO, LEDMAT_ROW3_PIO, 
        LEDMAT_ROW4_PIO, LEDMAT_ROW5_PIO, LEDMAT_ROW6_PIO, LEDMAT_ROW7_PIO};
    //the pins for the LEDmat columns
    static const pio_t cols[] = {LEDMAT_COL1_PIO, LEDMAT_COL2_PIO, LEDMAT_COL3_PIO, 
        LEDMAT_COL4_PIO, LEDMAT_COL5_PIO};
    static uint8_t previousColumn = 0; //for the display column function
    static uint8_t currentColumn = 0;
    
    uint8_t rowPattern = bitPattern[currentColumn];
    pio_output_high(cols[previousColumn]);//turn off prev col
    
    for (uint8_t currentRow = 0; currentRow < LEDMAT_ROWS_NUM; currentRow++) {
        if ((rowPattern >> currentRow) & 1) {
            pio_output_low(rows[currentRow]);
        } else {
            pio_output_high(rows[currentRow]);
        }
    }
    
    pio_output_low(cols[currentColumn]);//turn on this col
    previousColumn = currentColumn;
    currentColumn = (currentColumn + 1) % 5; //move to the next column
}

/**
 * displays the symbol of the given choice
 * @param choice, the index for the system to display
 */
void gfx_display_symbol(uint8_t choice)
{
    display_column(symbols[choice]);
}

/**
 * displays the animation of the given type
 * @param type, the index for the type of animation to play
 */
void gfx_display_animation(uint8_t type)
{
    static uint8_t currentFrame = 0; //for the display animation
    static uint8_t frameCounter = 0;
    
    display_column(animations[type][currentFrame]);
    //display the current column of the frame
    frameCounter++;
    if (frameCounter >= ANIMATION_RATE) { //move to next frame
        currentFrame = (currentFrame + 1) % ANIMATION_FRAMES;
        frameCounter = 0;
    }
}


