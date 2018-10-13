#ifndef SOUND_H
#define SOUND_H

#include "system.h"
#include "pio.h"

void tweeter_task_init(void);

void tweeter_task(void);

void tweeter_collision(void);

void tweeter_collision_reset(void);

#endif
