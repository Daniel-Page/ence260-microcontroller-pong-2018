#include "system.h"
#include "button.h"
#include "display.h"
#include "navswitch.h"
#include "led.h"
#include "pacer.h"


#define PACER_RATE 500
#define SLIDER_RATE 10


static uint8_t row = 3;
static uint16_t counter_north = 100; 
static uint16_t counter_south = 100;
void slider_movement(void) {
     display_pixel_set(4,row,0);
        display_pixel_set(4,row+1,0);
        display_pixel_set(4,row-1,0);
        display_update();
        navswitch_update();
        if ((navswitch_down_p(NAVSWITCH_NORTH)) && row-1 != 0) {
            if (counter_north == (PACER_RATE / SLIDER_RATE)) {
                row--;
                counter_north = 0;
            } else {
                counter_north++;
            }
        } else if (navswitch_up_p(NAVSWITCH_NORTH)) {
            counter_north = (PACER_RATE / SLIDER_RATE);
        }
        if ((navswitch_down_p(NAVSWITCH_SOUTH)) && row+1 != 6) {
            if (counter_south == (PACER_RATE / SLIDER_RATE)) {
                row++;
                counter_south = 0;
            } else {
                counter_south++;
            }
        } else if (navswitch_up_p(NAVSWITCH_SOUTH)) {
            counter_south = (PACER_RATE / SLIDER_RATE);
        }
        display_pixel_set(4,row,1);
        display_pixel_set(4,row+1,1);
        display_pixel_set(4,row-1,1);
        display_update();
}


int main (void)
{
    system_init ();
    display_init();
    pacer_init(PACER_RATE);

    while (1) {
        pacer_wait ();
        slider_movement();
    }
}
