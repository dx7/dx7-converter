#include "led.h"
#include "timer.h"
#include "host.h"
#include <stdint.h>
#include <avr/pgmspace.h>
#include "unimap.h"

/*
 * LED functions
 */
void set_led_on_layer_change(uint32_t layer_state)
{
    DDRD |= (1<<4);

    /* Led for Layer */
    if ((1<<1 & layer_state) != 0) {
        PORTD |= (1<<4);
    } else {
        PORTD &= ~(1<<4);
    }
}

void led_set_on_PD6(bool on)
{
    DDRD |= (1<<6);

    if (on) {
        PORTD |= (1<<6);
    } else {
        PORTD &= ~(1<<6);
    }
}


/*
 * HOOKS
 */

/*
 * Flash LED on every keypress
 * */
bool my_led_status = 0;
uint16_t my_led_timer;

void hook_matrix_change(keyevent_t event)
{
    // only flash LED for key press events, not key release events.
    if (IS_PRESSED(event)) {
        my_led_status = 1;
        led_set_on_PD6(my_led_status);
        my_led_timer = timer_read();
    }
}

void hook_keyboard_loop(void)
{
    if (my_led_status) {
        // check if we've reached 80 milliseconds yet...
        if (timer_elapsed(my_led_timer) > 80) {
            my_led_status = 0;
            led_set_on_PD6(my_led_status);
        }
    }
}


/*
 * Turn on LED when Layer 1 is active
 */
void hook_layer_change(uint32_t layer_state)
{
    set_led_on_layer_change(layer_state);
}
