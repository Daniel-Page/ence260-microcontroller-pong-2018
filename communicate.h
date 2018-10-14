#ifndef COMMUNICATE_H
#define COMMUNICATE_H


#include "system.h"
#include "ir_serial.h"
#include "led.h"
#include "tinygl.h"


void send_starting_signal(void);

void communicate_winner(void);

void receive_check(int8_t* pixel_x,int8_t* pixel_y,uint8_t* movement_state,int8_t* row, uint8_t* game_state);


#endif

