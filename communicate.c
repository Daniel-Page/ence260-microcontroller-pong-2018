// File: Communicate
// Authors: Daniel Page (dwi65) and Caleb Smith (cas202)
// Date: 14 Oct 2018
// Descr: Communications module


#include "system.h"
#include "ir_serial.h"
#include "led.h"
#include "tinygl.h"
#include "pixel.h"
#include "slider.h"


// Indicators
#define STARTING_INDICATOR 30
#define WINNER_INDICATOR 35
// LED state
#define LED_ON 1
#define LED_OFF 0
// Game states
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
void receive_check(int8_t* pixel_x,int8_t* pixel_y,uint8_t* movement_state,int8_t* row, uint8_t* game_state)
{
    uint8_t data = 0;
    if (ir_serial_receive(&data) == IR_SERIAL_OK) {
        if (data == STARTING_INDICATOR) {
            tinygl_clear();
            led_set(LED1,LED_OFF);
            *game_state = PLAYING;
        } else if (data == WINNER_INDICATOR) {
            reset_pixel(pixel_x,pixel_y);
            reset_slider(*row);
            tinygl_text("YOU WIN!");
            *game_state = GAME_OVER;
            led_set(LED1,LED_ON);
            *row = 3;
        } else if (data >= DIRECTION_OFFSET) {
            *pixel_x = 0;
            data = data - DIRECTION_OFFSET;
            *pixel_y = data;
            *movement_state = DNW;
        } else {
            *pixel_x = 0;
            *pixel_y = data;
            *movement_state = DSW;
        }
    }
}
