/**
 * the module for simple access to the board input and output
 * like the IR communication and the button and navswitch
 * @authors fya33 & jbr185
 */

#ifndef BASICIO_H
#define BASICIO_H

#include "button.h"
#include "navswitch.h"
#include "ir_uart.h"
#include "led.h"

#define IO_RATE 20
#define IR_SEND_RATE 1 /*send rarely as sending the IR signal is time consuming 
and seems to elapse the task rate*/
#define LED_FLASH_RATE 5
#define RESEND_COUNT 1 //the amount of times to resend the signal

void basicIO_init(void);

/**
 * update both the button and navswitch
 */
void basicIO_button_update(void);

/**
 * turns on the led
 */
void led_on(void);

/**
 * turns off the led
 */
void led_off(void);

/**
 * reverses the stored state
 */
void led_toggle(void);

/**
 * sends the given signal over IR if ready to send
 * @param signal, the bit squence (char) to send
 */
void ir_send(char signal);

/**
 * resends the signal a certain number of times based on resend count
 * @param signal, the bit squence (char) to send
 */
void ir_resend(char signal);

/**
 * resents the internal counter (restarts resending)
 */
void ir_resend_reset(void);

/**
 * returns the character received in the IR buffer
 */
char ir_get_message(void);

/**
 * returns 1 if a signal has been received, 0 otherwise
 */
uint8_t ir_received(void);

/**
 * returns 1 if the button has been pushed, 0 otherwise
 */
uint8_t button_pushed(void);

/**
 * returns 1 if the navswitch has been pushed down, 0 otherwise
 */
uint8_t navswitch_pushed(void);

/**
 * returns 1 if the navswitch has been pushed north, 0 otherwise
 */
uint8_t navswitch_north(void);

/**
 * returns 1 if the navswitch has been pushed south, 0 otherwise
 */
uint8_t navswitch_south(void);

/**
 * returns 1 if the navswitch has been pushed east, 0 otherwise
 */
uint8_t navswitch_east(void);

/**
 * returns 1 if the navswitch has been pushed west, 0 otherwise
 */
uint8_t navswitch_west(void);

/**
 * returns 1 if the navswitch has been pushed any direction, 0 otherwise
 */
uint8_t navswitch_any(void);

#endif
