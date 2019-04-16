/**
 * the module for simple access to the board input and output
 * like the IR communication and the button and navswitch
 * @authors fya33 & jbr185
 */

#include "basicIO.h"
#include "button.h"
#include "navswitch.h"
#include "ir_uart.h"
#include "led.h"

static uint8_t rsCounter; /*counter for the amount of signal resends, needs
                                to be global as two functions use it*/

void basicIO_init()
{
    navswitch_init();
    led_init();
    led_set(LED1, 0); //ensure led is off at start
    button_init();
    ir_uart_init();
}

/**
 * update the buttons
 */
void basicIO_button_update(void)
{
    navswitch_update();
    button_update();
}

/**
 * turns on the led
 */
void led_on(void)
{
    led_set(LED1, 1);
}

/**
 * turns off the led
 */
void led_off(void)
{
    led_set(LED1, 0);
}

/**
 * reverses the stored state
 */
void led_toggle(void)
{
    static uint8_t ledState = 1; //for led toggle
    
    led_set(LED1, ledState);
    ledState = !ledState;
}

/**
 * sends the given signal over IR if ready to send
 * @param signal, the bit squence (char) to send
 */
void ir_send(char signal)
{
    if (ir_uart_write_ready_p()) { //send if we are ready to
        ir_uart_putc(signal);
    }
}

/**
 * resends the signal a certain number of times based on resend count
 * @param signal, the bit squence (char) to send
 */
void ir_resend(char signal)
{
    if (rsCounter < RESEND_COUNT) {
        ir_send(signal);  //resend the connect symbol up to resend count
        rsCounter++;
    }
}

/**
 * resents the internal counter (restarts resending)
 */
void ir_resend_reset(void)
{
    rsCounter = 0;
}

/**
 * returns the character received in the IR buffer
 */
char ir_get_message(void)
{
    return ir_uart_getc();
}

/**
 * returns 1 if a signal has been received, 0 otherwise
 */
uint8_t ir_received(void)
{
    return ir_uart_read_ready_p();
}

/**
 * returns 1 if the button has been pushed, 0 otherwise
 */
uint8_t button_pushed(void)
{
    return button_push_event_p(0);
}

//the following are just simple passthrough functions to reduce text walls
/**
 * returns 1 if the navswitch has been pushed down, 0 otherwise
 */
uint8_t navswitch_pushed(void)
{
    return navswitch_push_event_p(NAVSWITCH_PUSH);
}

/**
 * returns 1 if the navswitch has been pushed north, 0 otherwise
 */
uint8_t navswitch_north(void)
{
    return navswitch_push_event_p(NAVSWITCH_NORTH);
}

/**
 * returns 1 if the navswitch has been pushed south, 0 otherwise
 */
uint8_t navswitch_south(void)
{
    return navswitch_push_event_p(NAVSWITCH_SOUTH);
}

/**
 * returns 1 if the navswitch has been pushed east, 0 otherwise
 */
uint8_t navswitch_east(void)
{
    return navswitch_push_event_p(NAVSWITCH_EAST);
}

/**
 * returns 1 if the navswitch has been pushed west, 0 otherwise
 */
uint8_t navswitch_west(void)
{
    return navswitch_push_event_p(NAVSWITCH_WEST);
}

/**
 * returns 1 if the navswitch has been pushed any direction, 0 otherwise
 */
uint8_t navswitch_any(void)
{
    return navswitch_pushed() || navswitch_north() || navswitch_south() 
                                || navswitch_east() || navswitch_west();
}

