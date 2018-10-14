// File: pong.c
// Authors: Daniel Page (dwi65) and Caleb Smith (cas202)
// Date: 15 Oct 2018
// Descr: The main file for controlling the states of the game


#include "system.h"
#include "button.h"
#include "display.h"
#include "navswitch.h"
#include "led.h"
#include "pacer.h"
#include "ir_serial.h"
#include "tinygl.h"
#include "../fonts/font3x5_1.h"
#include "sound.h"
#include "slider.h"
#include "pixel.h"
#include "communicate.h"


// Rates
#define PACER_RATE 1000
#define MESSAGE_RATE 20
// Game States
#define MENU 1
#define PLAYING 2
#define GAME_OVER 3
#define STATIONARY 0
// Directions
#define DNW 1
#define DNE 2
#define DSW 3
#define DSE 4
#define DIRECTION_OFFSET 10
// Output States
#define LED_ON 1
#define LED_OFF 0
// Indicators
#define STARTING_INDICATOR 30
#define WINNER_INDICATOR 35


// Game variables
static int8_t row = 3;
static int8_t pixel_x = -1;
static int8_t pixel_y = -1;
static uint8_t movement_state = STATIONARY;
static uint8_t game_state = MENU;


// Initiates game on both devices
void start_playing(void)
{
    navswitch_update();
    if (navswitch_down_p(NAVSWITCH_PUSH) && game_state == 1) {
        tinygl_clear();
        send_starting_signal();
        game_state = PLAYING;
        movement_state = DNW;
    }
}


// Resets the game
void button_reset_check(void)
{
    button_update();
    if (button_push_event_p(0)) {
        reset_pixel(&pixel_x,&pixel_y);
        reset_slider(row);
        led_set(LED1,LED_OFF);
        movement_state = 0;
        pixel_x = -1;
        pixel_y = -1;
        row = 3;
        tinygl_text("START");
        game_state = MENU;
    }
}


// Initialises all of the I/O devices and text
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
    tweeter_task_init();
}


// Controls the game with a series of states
int main (void)
{
    init_all();
    while (1) {
        pacer_wait ();
        if (game_state == MENU) {
            start_playing();
            tinygl_update ();
        } else if (game_state == PLAYING) {
            reset_pixel(&pixel_x,&pixel_y);
            reset_slider(row);
            row = slider_movement(row);
            pixel_movement(&pixel_x,&pixel_y,&movement_state,row);
            game_over_check(pixel_x,pixel_y,row,&game_state);
            display_update();
        } else if (game_state == GAME_OVER) {
            tinygl_update();
        }
        pixel_transition_check(pixel_x,pixel_y,&movement_state);
        receive_check(&pixel_x,&pixel_y,&movement_state,&row,&game_state);
        button_reset_check();
        tweeter_collision_reset();
    }
}
