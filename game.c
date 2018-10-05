#include "system.h"
#include "button.h"
#include "display.h"
#include "navswitch.h"
#include "led.h"
#include "pacer.h"

#define PACER_RATE 500

int main (void)
{
    system_init ();
    display_init();
    led_init();
    led_set(LED1, 0);
    uint8_t row = 3;
    uint8_t column = 2;
    uint8_t led_state = 0;
    pacer_init(PACER_RATE);

    while (1) {
        pacer_wait ();
        display_pixel_set(column,row,0);
        display_update();
        navswitch_update();
        if ((navswitch_push_event_p (NAVSWITCH_NORTH)) && row != 0) {
            row--;
        } else if ((navswitch_push_event_p (NAVSWITCH_SOUTH)) && row != 6) {
            row++;
        } else if ((navswitch_push_event_p (NAVSWITCH_EAST)) && column != 4) {
            column++;
        } else if ((navswitch_push_event_p (NAVSWITCH_WEST)) && column != 0) {
            column--;
        } else if ((navswitch_push_event_p (NAVSWITCH_PUSH))) {
            if (led_state == 1) {
                led_state = 0;
            } else {
                led_state = 1;
            }
            led_set (LED1, led_state);
        }
        display_pixel_set(column,row,1);
        display_update();
    }
}
