#ifndef PIXEL_H
#define PIXEL_H

#include "system.h"
#include "sound.h"
#include "display.h"

void pixel_movement(int8_t* pixel_x,int8_t* pixel_y,uint8_t* movement_state,int8_t row);

void reset_pixel(int8_t* pixel_x,int8_t* pixel_y);

#endif
