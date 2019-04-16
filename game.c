/**
 * The main game file, which contains the main function and tasks
 * 
 * the game tasks are all borken up into stages/phases which are
 * controlled by a static variable, this seemed like the easiest way
 * to have the game run through distinct phases with the task scheduler
 * api function running the main loop
 * @authors jbr185 & fya33
 */

#include "graphics.h"
#include "basicIO.h"
#include "gameLogic.h"
#include "sound.h"
#include "system.h"
#include "task.h"
#include <stdlib.h>

#define WELCOME 0 //game stages/phases
#define CONNECT 1
#define HEALTH 2
#define GAME 3
#define SEND 4
#define RESULT 5
#define END 6

#define VICTORY 0 //used for end phase to make clear what is a victory/loss
#define DEFEAT 1

static uint8_t gameState; //stores the gameState for all tasks
static GameObj game; //this stores all the attributes of a game

/**
 * the welcome phase, displays the game title and resents the game struct
 * to default values
 */
void welcome_phase(void) {
    gameState = WELCOME;
    led_off(); //make sure the led if off in this phase
    gfx_set_text("PAPER SCISSORS ROCK");
    game_reset(&game); //reset the game's attributes to default
}

/**
 * waits for another board to send an 'c' (connect) signal to start the game
 */
void connect_phase(void) {
    gameState = CONNECT;
    gfx_set_text("CONNECTING...");
    ir_resend_reset(); /*resets the resend counter
    after every sending phase, we reset this counter*/
}

/**
 * displays the players current lives
 */
void health_phase(void) {
    gameState = HEALTH;
    led_off();
    gfx_set_num(game.lives); //displays current lives value
}

/**
 * displays the three game symbols and allows the user to cycle through them
 */
void game_phase(void) {
    gameState = GAME;
    led_off();
}

/**
 * sends the selected choice to the other board, and displays a little 
 * waiting animation
 */
void send_phase(void) {
    gameState = SEND;
    ir_resend_reset();
}

/**
 * displays the result of the round with a little animation, and plays
 * a short sound based on the result 
 */
void result_phase(void) {
    gameState = RESULT;
    sound_set_track(tunes[game.result]); //play the sound based on the round result
    led_off();
}

/**
 * displays the result of the whole game and exits to the welcome screen
 * @param result, the result of the game, 0 for lose, 1 for win
 */
void end_phase(uint8_t gameResult) {
    gameState = END;
    led_off();
    if (gameResult == VICTORY) {
        gfx_set_text("YOU WIN! :)");
    } else {
        gfx_set_text("YOU LOSE :(");
    }
    ir_resend_reset();
}

/**
 * the task for dealing with all the button inputs in the stages of the
 * game and changing the state based on that input
 */
void input_task(__unused__ void *data) {
    basicIO_button_update(); //update the buttons

    switch (gameState) {
        case WELCOME:
            if (navswitch_pushed()) {
                connect_phase(); //find a partner board
            }
            break;
        case CONNECT:
            if (button_pushed()) {
                welcome_phase(); //cancel connect attempt
            }
            break;
        case HEALTH:
            if (button_pushed()) {
                ir_resend_reset(); //restart resending the connect signal
            }
            if (navswitch_any()) {
                game_phase(); //return to choices
            }
            break;
        case GAME:
            if (button_pushed()) {
                ir_resend_reset(); //resend reconnect
            }
            if (navswitch_east() || navswitch_west()) {
                health_phase(); //view remaining lives
            }
            if (navswitch_north()) {
                game.choice = (game.choice + 1) % SYMBOL_COUNT; //advance choice
            }
            if (navswitch_south()) {
                if (game.choice == 0) { //modulo was causing issues with over lapping
                    game.choice = 2;    //to 255, so set it manually to smooth experiance
                } else {
                    game.choice -= 1;
                }
            } else if (navswitch_pushed()) {
                send_phase(); //send current choice
            }
            break;
        case SEND:
            if (navswitch_any()) {
                game_phase(); //cancel current choice
            }
            break;
        case RESULT:
            if (button_pushed()) {
                ir_resend_reset(); //restart resending choice signal
            }
            if (navswitch_any()) {
                game_phase(); //back to next round
            }
            break;
        case END:
            if (button_pushed() || navswitch_any()) {
                welcome_phase(); //back to main menu
            }
            break;
        default:
            break;
    }
}

/**
 * the task for sending data, done rarely as it is time consuming and 
 * causes screen flickering issues
 */
void ir_send_task(__unused__ void *data) {
    switch (gameState) {
        case CONNECT:
            ir_send('c');
            break; //send the connect symbol
        case GAME: //no break as same action as health stage
        case HEALTH:
            ir_resend('c');
            break; //resend to ensure parter board has connected
        case SEND:
            ir_send(choices[game.choice]);
            break; //send the choice
        case RESULT:
            ir_resend(choices[game.choice]); //resend to ensure both boards continue
            if (game.lives == 0) { //if you died, tell the other player
                ir_send('D');
                end_phase(DEFEAT);
            }
            break;
        case END:
            if (game.lives == 0) { //if dead, resend that so other board knows
                ir_resend('D');
            }
            break;
        default:
            break;
    }
}

/**
 * task for receiving signals and interpreting them through the game stages
 */
void ir_recieved_task(__unused__ void *data) {
    if (ir_received()) { //if we have a message
        switch (gameState) {
            case CONNECT:
                if (ir_get_message() == 'c'/*onnect*/) {
                    game_phase(); //start game
                }
                break;
            case SEND:
                game.opponent = ir_get_message();
                if (opponent_valid(&game)) { //if the opponent signal is valid
                    game.result = compare(choices[game.choice], game.opponent);
                    if (game.result == LOSE) {
                        game.lives--;
                    }
                    result_phase(); //show result
                }
                break;
            case RESULT:
                if (ir_get_message() == 'D') { //if the other player has died, we win right?
                    end_phase(VICTORY);
                }
                break;
            default:
                break;
        }
    }
}

/**
 * task for displaying the various screens to the user at the stages
 */
void display_task(__unused__ void *data) {
    switch (gameState) {
        case GAME:
            gfx_display_symbol(game.choice);
            break; //gfx has the same order so pass index
        case SEND:
            gfx_display_animation(READY);
            break; //display the ready animation
        case RESULT:
            gfx_display_animation(game.result);
            break; //display your result animation
        default:
            gfx_display_text();
            break; //for all these stages, use tinygl to display
    }
}

/**
 * the task for the led controller, flashing at certain stages
 */
void led_task(__unused__ void *data) {
    switch (gameState) {
        case CONNECT:
        case SEND:
            led_toggle();
            break; //only blink the led when we are sending data
        default:
            break;
    }
}

/**
 * the sound controller for playing sound at certain stages, just result for now
 */
void sound_task(__unused__ void *data) {
    switch (gameState) {
        case RESULT:
            sound_update();
            break; //i only have sounds for results, i cant do the music good
        default:
            break;
    }
}

int main(void) {
    system_init();
    basicIO_init();
    gfx_init();
    sound_init();

    task_t tasks[] = {
            {.func = sound_task, .period = TASK_RATE / TWEETER_RATE, .data = 0},
            {.func = input_task, .period = TASK_RATE / IO_RATE, .data = 0},
            {.func = ir_send_task, .period = TASK_RATE / IR_SEND_RATE, .data = 0},
            {.func = ir_recieved_task, .period = TASK_RATE / IO_RATE, .data = 0},
            {.func = display_task, .period = TASK_RATE / DISPLAY_RATE, .data = 0},
            {.func = led_task, .period = TASK_RATE / LED_FLASH_RATE, .data = 0}
    };

    welcome_phase(); //init welcome stage
    task_schedule(tasks, ARRAY_SIZE(tasks)); //start
}
