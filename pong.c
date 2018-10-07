#include "system.h"
#include "button.h"
#include "display.h"
#include "navswitch.h"
#include "led.h"
#include "pacer.h"
#include "ir_serial.h"


#define PACER_RATE 1000
#define SLIDER_RATE 10
#define PIXEL_RATE 5


static int8_t pixel_x = -1;
static int8_t pixel_y = -1;
static int8_t row = 3;
static uint16_t counter_north = (PACER_RATE / SLIDER_RATE);
static uint16_t counter_south = (PACER_RATE / SLIDER_RATE);
static uint16_t counter_pixel = (PACER_RATE / PIXEL_RATE);
static uint8_t movement_state = 0;
static uint8_t connection_state = 0;


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
}


void pixel_left(void)
{
    pixel_x++;
}


void pixel_right(void)
{
    pixel_x--;
}


void pixel_up_left(void)
{
    pixel_x++;
    pixel_y++;
}


void pixel_down_right(void)
{
    pixel_x--;
    pixel_y--;
}


void pixel_down_left(void)
{
    pixel_x++;
    pixel_y--;
}


void pixel_up_right(void)
{
    pixel_x--;
    pixel_y++;
}


// left: 1
// right: 2
// left up: 3
// right up: 4
// left down: 5
// right down: 6
void pixel_movement(void)
{
    if (counter_pixel == (PACER_RATE / PIXEL_RATE)) {
        if (((pixel_x == 3 && pixel_y == row) ||
                (pixel_x == 3 && pixel_y == row+1) ||
                (pixel_x == 3 && pixel_y == row-1))) {
            if (movement_state == 3) {
                movement_state = 4;
            } else if (movement_state == 5) {
                movement_state = 6;
            } else if (movement_state == 3) {
                movement_state = 6;
            }
        } else if ((movement_state == 4) && ((pixel_x == 1 && pixel_y == 6) ||
                                             (pixel_x == 2 && pixel_y == 6) ||
                                             (pixel_x == 3 && pixel_y == 6))) {
            movement_state = 6;
        } else if ((movement_state == 6) && ((pixel_x == 1 && pixel_y == 0) ||
                                             (pixel_x == 2 && pixel_y == 0) ||
                                             (pixel_x == 3 && pixel_y == 0))) {
            movement_state = 4;
        } else if ((pixel_x == 0 && pixel_y == 1) ||
                   (pixel_x == 0 && pixel_y == 2) ||
                   (pixel_x == 0 && pixel_y == 3) ||
                   (pixel_x == 0 && pixel_y == 4) ||
                   (pixel_x == 0 && pixel_y == 5)) {
            if (movement_state == 6) {
                movement_state = 5;
            } else if (movement_state == 4) {
                movement_state = 3;
            } else if (movement_state == 1) {
                movement_state = 2;
            }
        } else if ((movement_state == 6) && (pixel_x == 0 && pixel_y == 0)) {
            movement_state = 3;
        } else if ((movement_state == 4) && (pixel_x == 0 && pixel_y == 6)) {
            movement_state = 5;
        }
        if (movement_state == 1) {
            pixel_left();
        } else if (movement_state == 2) {
            pixel_right();
        } else if (movement_state == 3) {
            pixel_up_left();
        } else if (movement_state == 4) {
            pixel_up_right();
        } else if (movement_state == 5) {
            pixel_down_left();
        } else if (movement_state == 6) {
            pixel_down_right();
        }
        counter_pixel = 0;
    } else {
        counter_pixel++;
    }
    display_pixel_set(pixel_x,pixel_y,1);
    display_update();
}


void game_over_check(void) {
        if ((pixel_x == 4 && pixel_y == 0) ||
                   (pixel_x == 4 && pixel_y == 1) ||
                   (pixel_x == 4 && pixel_y == 2) ||
                   (pixel_x == 4 && pixel_y == 3) ||
                   (pixel_x == 4 && pixel_y == 4) ||
                   (pixel_x == 4 && pixel_y == 5) ||
                   (pixel_x == 4 && pixel_y == 6)) {
                       // game over instructions
                   }
    
}


void check_connection(void)
{
    
    
    
    
}


// Push the navswtich direcly down to start game on your side
void choose_starting_side(void) {
    if (navswitch_down_p(NAVSWITCH_PUSH) && movement_state == 0) {
        movement_state = 3;
    }
}


int main (void)
{
    system_init ();
    display_init();
    ir_serial_init ();
    pacer_init(PACER_RATE);
    while (1) {
        pacer_wait ();
        
        choose_starting_side();
        // connection_state == 1
        if (1) {
            reset();
            slider_movement();
            pixel_movement();
            display_update();
        }
        check_connection();
    }
}
