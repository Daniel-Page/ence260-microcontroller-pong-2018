// File: communicate.h
// Authors: Daniel Page (dwi65) and Caleb Smith (cas202)
// Date: 15 Oct 2018
// Descr: Communicate header file


#ifndef COMMUNICATE_H
#define COMMUNICATE_H


#include "system.h"
#include "ir_serial.h"
#include "led.h"
#include "tinygl.h"


// Sends starting singal when both devices are on "START" screen
void send_starting_signal(void);


// The loser sends a signal to the other device that they are the winner
void communicate_winner(void);


// Checks for IR signals and carries out actions
void receive_check(int8_t* pixel_x,int8_t* pixel_y,uint8_t* movement_state,int8_t* row, uint8_t* game_state);


// Encodes and sends the position via IR to the other device
void send_position(int8_t pixel_y, uint8_t* movement_state);


#endif

