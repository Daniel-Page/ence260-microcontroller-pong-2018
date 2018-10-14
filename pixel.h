// File: pixel.h
// Authors: Daniel Page (dwi65) and Caleb Smith (cas202)
// Date: 15 Oct 2018
// Descr: Pixel header file

#ifndef PIXEL_H
#define PIXEL_H


#include "system.h"
#include "sound.h"
#include "display.h"
#include "communicate.h"
#include "tinygl.h"
#include "slider.h"


// Contols the pixel movement in free space and at the boundaries of the game
void pixel_movement(int8_t* pixel_x,int8_t* pixel_y,uint8_t* movement_state,int8_t row);


// Resets moving pixel
void reset_pixel(int8_t* pixel_x,int8_t* pixel_y);


// Sends signal to other device after processing
void pixel_transition_check(int8_t pixel_x,int8_t pixel_y,uint8_t* movement_state);


// Checks to see if the pixel has entered the restricted zone
void game_over_check(int8_t pixel_x,int8_t pixel_y, int8_t row, uint8_t* game_state);


#endif
