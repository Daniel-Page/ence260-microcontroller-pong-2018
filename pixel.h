#ifndef PIXEL_H
#define PIXEL_H


#include "system.h"
#include "sound.h"
#include "display.h"
#include "communicate.h"
#include "tinygl.h"
#include "slider.h"


void pixel_movement(int8_t* pixel_x,int8_t* pixel_y,uint8_t* movement_state,int8_t row);


void reset_pixel(int8_t* pixel_x,int8_t* pixel_y);


void pixel_transition_check(int8_t pixel_x,int8_t pixel_y,uint8_t* movement_state);


void game_over_check(int8_t pixel_x,int8_t pixel_y, int8_t row, uint8_t* game_state);


#endif
