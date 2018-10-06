#include "system.h"
#include "button.h"
#include "display.h"
#include "navswitch.h"
#include "led.h"
#include "pacer.h"


#define PACER_RATE 500
#define SLIDER_RATE 10

static uint8_t pixel_x = 0;
static uint8_t pixel_y = 0;
static int8_t row = 3;
static uint16_t counter_north = 100;
static uint16_t counter_south = 100;


void reset(void)
{
    display_pixel_set(4,row,0);
    display_pixel_set(4,row+1,0);
    display_pixel_set(4,row-1,0);
    display_pixel_set(pixel_x,pixel_y,0);
    display_update();
}


void slider_movement(void)
{
    navswitch_update();
    if (navswitch_down_p(NAVSWITCH_NORTH)) {
        if (row == -3) {

            row = 7;
        }
        if (counter_north == (PACER_RATE / SLIDER_RATE)) {
            row--;
            counter_north = 0;
        } else {
            counter_north++;
        }
    } else if (navswitch_up_p(NAVSWITCH_NORTH)) {
        counter_north = (PACER_RATE / SLIDER_RATE);
    }
    if (navswitch_down_p(NAVSWITCH_SOUTH)) {
        if (row == 8) {
            row = -1;
        }
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


void pixel(void)
{
    pixel_x = 0;
    pixel_y = 3;
    display_pixel_set(pixel_x,pixel_y,1);
    display_update();
}

void pixel_movement(void)
{

}


int main (void)
{
    system_init ();
    display_init();
    pacer_init(PACER_RATE);

    while (1) {
        pacer_wait ();
        reset();
        slider_movement();
        pixel();
    }
}
