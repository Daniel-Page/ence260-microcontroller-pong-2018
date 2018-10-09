#include "system.h"
#include "button.h"
#include "display.h"
#include "navswitch.h"
#include "led.h"
#include "pacer.h"
#include "ir_serial.h"
#include "tinygl.h"
#include "../fonts/font3x5_1.h"
#include "spwm.h"


#define PACER_RATE 1000
#define SLIDER_RATE 10
#define PIXEL_RATE 5
#define MESSAGE_RATE 20
#define MENU 1
#define PLAYING 2
#define GAME_OVER 3
#define STATIONARY 0
#define NW 1
#define NE 2
#define SW 3
#define SE 4
#define DIRECTION_OFFSET 10
#define STARTING_INDICATOR 30
#define WINNER_INDICATOR 35
#define LED_ON 1
#define LED_OFF 0

static int8_t pixel_x = -1;
static int8_t pixel_y = -1;
static int8_t row = 3;
static uint16_t counter_north = (PACER_RATE / SLIDER_RATE);
static uint16_t counter_south = (PACER_RATE / SLIDER_RATE);
static uint16_t counter_pixel = (PACER_RATE / PIXEL_RATE);
static uint8_t movement_state = STATIONARY;
static uint8_t game_state = MENU;


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


void pixel_nw(void)
{
    pixel_x++;
    pixel_y++;
}


void pixel_se(void)
{
    pixel_x--;
    pixel_y--;
}


void pixel_sw(void)
{
    pixel_x++;
    pixel_y--;
}


void pixel_ne(void)
{
    pixel_x--;
    pixel_y++;
}


void pixel_movement(void)
{
    if (counter_pixel == (PACER_RATE / PIXEL_RATE)) {
        // Slider rebound
        if ((pixel_x == 3) && ((pixel_y == 6))) {
             if (movement_state == NE) {
                movement_state = SW;
            } else if (movement_state == SE){
                movement_state = NW;
            }
        } else if (((pixel_x == 3 && pixel_y == row) ||
                (pixel_x == 3 && pixel_y == row+1) ||
                (pixel_x == 3 && pixel_y == row-1))) {
            if (movement_state == NW) {
                movement_state = NE;
            } else if (movement_state == SW) {
                movement_state = SE;
            }
            // Top rebound
        } else if ((pixel_x == 0 && pixel_y == 6) ||
                   (pixel_x == 1 && pixel_y == 6) ||
                   (pixel_x == 2 && pixel_y == 6) ||
                   (pixel_x == 3 && pixel_y == 6)) {
            if (movement_state == NE) {
                movement_state = SE;
            } else if (movement_state == NW) {
                movement_state = SW;
            }
            // Bottom rebound
        } else if ((pixel_x == 0 && pixel_y == 0) ||
                   (pixel_x == 1 && pixel_y == 0) ||
                   (pixel_x == 2 && pixel_y == 0) ||
                   (pixel_x == 3 && pixel_y == 0)) {
            if (movement_state == SE) {
                movement_state = NE;
            } else if (movement_state == SE) {
                movement_state = NW;
            }
        }
        if (movement_state == NW) {
            pixel_nw();
        } else if (movement_state == NE) {
            pixel_ne();
        } else if (movement_state == SW) {
            pixel_sw();
        } else if (movement_state == SE) {
            pixel_se();
        }
        counter_pixel = 0;
    } else {
        counter_pixel++;
    }
    display_pixel_set(pixel_x,pixel_y,1);
}


void send_starting_signal(void)
{
    ir_serial_transmit(STARTING_INDICATOR);
}


void communicate_winner(void) {
    ir_serial_transmit(WINNER_INDICATOR);
}


void pixel_receive_check(void)
{
    uint8_t data = 0;
    if (ir_serial_receive(&data) == IR_SERIAL_OK) {
        if (data == STARTING_INDICATOR) {
            tinygl_clear();
            game_state = PLAYING;
        } else if (data == WINNER_INDICATOR) {
            reset();
            tinygl_text("YOU WIN!");
            game_state = GAME_OVER;
            led_set(LED1,LED_ON);
        } else if (data >= DIRECTION_OFFSET) {
            pixel_x = 0;
            data = data - DIRECTION_OFFSET;
            pixel_y = data;
            movement_state = NW;
        } else {
            pixel_x = 0;
            pixel_y = data;
            movement_state = SW;
        }
    }
}


void pixel_transition_check(void)
{
    if ((pixel_x == -1 && pixel_y == 0) ||
            (pixel_x == -1 && pixel_y == 1) ||
            (pixel_x == -1 && pixel_y == 2) ||
            (pixel_x == -1 && pixel_y == 3) ||
            (pixel_x == -1 && pixel_y == 4) ||
            (pixel_x == -1 && pixel_y == 5) ||
            (pixel_x == -1 && pixel_y == 6)) {
        // (7-) inverts screen orientation
        if (movement_state == NE) {
            // Going up right
            ir_serial_transmit (6-pixel_y);
            movement_state = STATIONARY;
        } else if (movement_state == SE) {
            // (+10) to signify going down right
            ir_serial_transmit (6-pixel_y+DIRECTION_OFFSET);
            movement_state = STATIONARY;
        }
    }
}


void start_playing(void)
{
    navswitch_update();
    if (navswitch_down_p(NAVSWITCH_PUSH) && game_state == 1) {
        tinygl_clear();
        send_starting_signal();
        game_state = PLAYING;
        movement_state = NW;
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
        tinygl_text("YOU LOSE");
        reset();
        communicate_winner();
        game_state = GAME_OVER;
    }
}


void button_reset_check(void) {
    button_update();
    if (button_push_event_p(0)) {
        reset();
        led_set(LED1,LED_OFF);
        movement_state = 0;
        pixel_x = -1;
        pixel_y = -1;
        row = 3;
        tinygl_text("START");
        game_state = MENU;
    }
}


void init_all(void)
{
    system_init ();
    display_init();
    ir_init();
    led_init();
    led_set(LED1,LED_OFF);
    button_init();
    pacer_init(PACER_RATE);
    tinygl_init (PACER_RATE);
    tinygl_font_set (&font3x5_1);
    tinygl_text_speed_set (MESSAGE_RATE);
    tinygl_text_mode_set (TINYGL_TEXT_MODE_SCROLL);
    tinygl_text("START");
}


int main (void)
{
    init_all();
    while (1) {
        pacer_wait ();
        if (game_state == MENU) {
            start_playing();
            tinygl_update ();
        } else if (game_state == PLAYING) {
            reset();
            slider_movement();
            pixel_movement();
            game_over_check();
            display_update();
        } else if (game_state == GAME_OVER) {
            tinygl_update();
        }
        pixel_transition_check();
        pixel_receive_check();
        button_reset_check();
    }
}

// To do: README, sounds, split into modules and comment everything
