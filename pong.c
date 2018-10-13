// File: Pong
// Authors: Daniel Page (dwi65) and Caleb Smith (cas202)
// Date: 10 Oct 2018
// Descr: The main file for a game of pong


#include "system.h"
#include "button.h"
#include "display.h"
#include "navswitch.h"
#include "led.h"
#include "pacer.h"
#include "ir_serial.h"
#include "tinygl.h"
#include "../fonts/font3x5_1.h"
#include "pio.h"
#include "sound.h"
#include "slider.h"
#include "pixel.h"


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
// Indicators
#define STARTING_INDICATOR 30
#define WINNER_INDICATOR 35
// Output States
#define LED_ON 1
#define LED_OFF 0


// Game variables
static int8_t row = 3;
static int8_t pixel_x = -1;
static int8_t pixel_y = -1;
static uint8_t movement_state = STATIONARY;
static uint8_t game_state = MENU;


// Resets slider and pixel
void reset(void)
{
    display_pixel_set(4,row,0);
    display_pixel_set(4,row+1,0);
    display_pixel_set(4,row-1,0);
    display_pixel_set(pixel_x,pixel_y,0);
    display_update();
}


// Sends starting singal when both devices are on "START" screen
void send_starting_signal(void)
{
    ir_serial_transmit(STARTING_INDICATOR);
}


// The loser sends a signal to the other device that they are the winner
void communicate_winner(void)
{
    ir_serial_transmit(WINNER_INDICATOR);
}


// Checks for IR signals and carries out actions
void receive_check(void)
{
    uint8_t data = 0;
    if (ir_serial_receive(&data) == IR_SERIAL_OK) {
        if (data == STARTING_INDICATOR) {
            tinygl_clear();
            led_set(LED1,LED_OFF);
            game_state = PLAYING;
        } else if (data == WINNER_INDICATOR) {
            reset();
            tinygl_text("YOU WIN!");
            game_state = GAME_OVER;
            led_set(LED1,LED_ON);
            row = 3;
        } else if (data >= DIRECTION_OFFSET) {
            pixel_x = 0;
            data = data - DIRECTION_OFFSET;
            pixel_y = data;
            movement_state = DNW;
        } else {
            pixel_x = 0;
            pixel_y = data;
            movement_state = DSW;
        }
    }
}


// Sends signal to other device after processing
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
        if (movement_state == DNE) {
            // Going up right
            ir_serial_transmit (6-pixel_y);
            movement_state = STATIONARY;
        } else if (movement_state == DSE) {
            // (+10) to signify going down right
            ir_serial_transmit (6-pixel_y+DIRECTION_OFFSET);
            movement_state = STATIONARY;
        }
    }
}


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


// Checks to see if the pixel has entered the restricted zone
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


// Resets the game
void button_reset_check(void)
{
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
            reset();
            row = slider_movement(row);
            pixel_movement(&pixel_x,&pixel_y,&movement_state,row);
            game_over_check();
            display_update();
        } else if (game_state == GAME_OVER) {
            tinygl_update();
        }
        pixel_transition_check();
        receive_check();
        button_reset_check();
        tweeter_collision_reset();
    }
}
