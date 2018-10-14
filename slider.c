// File: Slider
// Authors: Daniel Page (dwi65) and Caleb Smith (cas202)
// Date: 14 Oct 2018
// Descr: Slider module


#include "slider.h"
#include "system.h"
#include "navswitch.h"
#include "display.h"


#define PACER_RATE 1000
#define SLIDER_RATE 10


static uint16_t counter_north = (PACER_RATE / SLIDER_RATE);
static uint16_t counter_south = (PACER_RATE / SLIDER_RATE);


void reset_slider(int8_t row)
{
    display_pixel_set(4,row,0);
    display_pixel_set(4,row+1,0);
    display_pixel_set(4,row-1,0);
    display_update();
}


// Controls slider movement and sets display
int8_t slider_movement(int8_t row)
{
    navswitch_update();
    if (navswitch_down_p(NAVSWITCH_NORTH)) {
        if (row == -2) {
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
    return row;
}
