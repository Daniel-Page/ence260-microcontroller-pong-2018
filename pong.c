#include "system.h"
#include "button.h"
#include "display.h"
#include "navswitch.h"
#include "led.h"
#include "pacer.h"
#include "ir_serial.h"
#include "tinygl.h"
#include "../fonts/font3x5_1.h"


#define PACER_RATE 500
#define SLIDER_RATE 10
#define PIXEL_RATE 5
#define MESSAGE_RATE 30


static int8_t pixel_x = -1;
static int8_t pixel_y = -1;
static int8_t row = 3;
static uint16_t counter_north = (PACER_RATE / SLIDER_RATE);
static uint16_t counter_south = (PACER_RATE / SLIDER_RATE);
static uint16_t counter_pixel = (PACER_RATE / PIXEL_RATE);
static uint8_t movement_state = 0;
static uint8_t game_state = 1;


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
    display_update();
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
        // Slider rebound
        if (((pixel_x == 3 && pixel_y == row) ||
                (pixel_x == 3 && pixel_y == row+1) ||
                (pixel_x == 3 && pixel_y == row-1))) {
            if (movement_state == 3) {
                movement_state = 4;
            } else if (movement_state == 5) {
                movement_state = 6;
            }
            // Top rebound
        } else if ((pixel_x == 1 && pixel_y == 6) ||
                   (pixel_x == 2 && pixel_y == 6) ||
                   (pixel_x == 3 && pixel_y == 6)) {
            if (movement_state == 4) {
                movement_state = 6;
            } else if (movement_state == 3) {
                movement_state = 5;
            }
            // Bottom rebound
        } else if ((pixel_x == 1 && pixel_y == 0) ||
                        (pixel_x == 2 && pixel_y == 0) ||
                        (pixel_x == 3 && pixel_y == 0)) {
            if (movement_state == 6) {
                movement_state = 4;
            } else if (movement_state == 6) {
                movement_state = 3;
            }
        }
        if (movement_state == 3) {
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
}


void pixel_receive_check(void)
{
    uint8_t data = 0;
    uint8_t ret = 0;
    ret = ir_serial_receive(&data);
    if (ret == IR_SERIAL_OK) {
        pixel_x = 0;
        if (data >= 10) {
            data = data - 10;
            pixel_y = data;
            movement_state = 3;
        } else {
            pixel_y = data;
            movement_state = 5;
        }
    }
}


void pixel_transition_check(void)
{
    if ((pixel_x == -1 && pixel_y == -1) ||
            (pixel_x == -1 && pixel_y == 0) ||
            (pixel_x == -1 && pixel_y == 1) ||
            (pixel_x == -1 && pixel_y == 2) ||
            (pixel_x == -1 && pixel_y == 3) ||
            (pixel_x == -1 && pixel_y == 4) ||
            (pixel_x == -1 && pixel_y == 5) ||
            (pixel_x == -1 && pixel_y == 6) ||
            (pixel_x == -1 && pixel_y == 7)) {
        // (7-) inverts screen orientation
        if (movement_state == 4) {
            // Going up right
            if (pixel_y == 7) {
                ir_serial_transmit (6-5+10);
            } else {
                ir_serial_transmit (6-pixel_y);
            }
            movement_state = 0;
        } else if (movement_state == 6) {
            // (+10) to signify going down right
            if (pixel_y == -1) {
                ir_serial_transmit (6-1);
            } else {
                ir_serial_transmit (6-pixel_y+10);
                movement_state = 0;
            }
        }
    }
}


void start_playing(void)
{
    navswitch_update();
    if (navswitch_down_p(NAVSWITCH_PUSH) && game_state == 1) {
        movement_state = 3;
        tinygl_clear();
        game_state = 2;
    }
}


void game_over_check(void)
{
    if ((pixel_x == 4 && pixel_y == 0) ||
            (pixel_x == 4 && pixel_y == 1) ||
            (pixel_x == 4 && pixel_y == 2) ||
            (pixel_x == 4 && pixel_y == 3) ||
            (pixel_x == 4 && pixel_y == 4) ||
            (pixel_x == 4 && pixel_y == 5) ||
            (pixel_x == 4 && pixel_y == 6)) {
        tinygl_text("GAME OVER");
        reset();
        game_state = 4;
    }
}


// 1: Menu
// 2: Playing
// 3: No pixel
// 4: Game over
int main (void)
{
    system_init ();
    display_init();
    ir_init();
    led_init();
    led_set (LED1, 0);
    pacer_init(PACER_RATE);
    tinygl_init (PACER_RATE);
    tinygl_font_set (&font3x5_1);
    tinygl_text_speed_set (MESSAGE_RATE);
    tinygl_text_mode_set (TINYGL_TEXT_MODE_SCROLL);
    tinygl_text("PLAY");
    while (1) {
        pacer_wait ();
        if (game_state == 1) {
            start_playing();
            tinygl_update ();
        } else if (game_state == 2) {
            reset();
            slider_movement();
            pixel_movement();
            game_over_check();
            display_update();
        } else if (game_state == 4) {
            tinygl_update();
        }
        pixel_transition_check();
        pixel_receive_check();
    }
}
