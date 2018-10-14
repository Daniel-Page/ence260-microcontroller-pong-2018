// File: slider.h
// Authors: Daniel Page (dwi65) and Caleb Smith (cas202)
// Date: 15 Oct 2018
// Descr: Slider header file


#ifndef SLIDER_H
#define SLIDER_H


#include "system.h"
#include "navswitch.h"
#include "display.h"


// Resets the slider pixels
void reset_slider(int8_t row);


// Controls slider movement and sets display
int8_t slider_movement(int8_t row);


#endif
