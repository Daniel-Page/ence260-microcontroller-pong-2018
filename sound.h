// File: sound.h
// Authors: Daniel Page (dwi65) and Caleb Smith (cas202)
// Date: 15 Oct 2018
// Descr: Sound header file


#ifndef SOUND_H
#define SOUND_H


#include "system.h"
#include "pio.h"


// Initialises the tweeter
void tweeter_task_init(void);


// Updates the tweeter sound
void tweeter_task(void);


// Makes a sound when there is a collision
void tweeter_collision(void);


// Turns off the collision beep
void tweeter_collision_reset(void);


#endif
