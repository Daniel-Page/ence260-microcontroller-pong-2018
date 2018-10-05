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
    uint8_t row = 3;
    uint8_t column = 2;
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
        }
        display_pixel_set(column,row,1);
        display_update();
    }
}
