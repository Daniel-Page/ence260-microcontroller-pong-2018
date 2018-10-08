#include "system.h"
#include "button.h"
#include "display.h"
#include "navswitch.h"
#include "led.h"
#include "pacer.h"
#include "ir_serial.h"
#include "ir.h"
#include "tinygl.h"
#include "../fonts/font3x5_1.h"


#define PACER_RATE 1000
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
static uint8_t connection_state = 0;
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
}


static uint8_t i = 0;
static uint8_t n = 0;
static uint8_t check[] = {0,1,1,0,1,1,0,1};
void check_connection(void)
{
    if (n != 8) {
        ir_tx_set(check[n],1);
        n++;
    } else {
        n = 0;
    }

    if (ir_rx_get() == check[i]) {
        i++;
    } else {
        i = 0;
    }
    if (i == 8) {
        connection_state = 1;
        i = 0;
        led_set (LED1, 1);
    }
}

void choose_starting_side(void)
{
    if (navswitch_down_p(NAVSWITCH_PUSH) && movement_state == 0) {
        movement_state = 3;
    }
}


void start_playing(void)
{
    navswitch_update();
    if (navswitch_down_p(NAVSWITCH_PUSH) && game_state == 1) {
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
// 3: Pause
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
            // if (connection_state == 1) {
            if (1) {
                reset();
                choose_starting_side();
                slider_movement();
                pixel_movement();
                game_over_check();
                display_update();
            }
        } else if (game_state == 4) {
            tinygl_update ();
        }
        check_connection();
    }
}
