// File: Pong
// Authors: Daniel Page (dwi65) and Caleb Smith (cas202)
// Date: 10 Oct 2018
// Descr: The main file for a game of pong

#include "system.h"
#include "tweeter.h"
#include "sound.h"
#include "pio.h"

// Rate
#define PACER_RATE 1000
// Tweeter initialisation
#define PIEZO_PIO PIO_DEFINE (PORT_D, 6)
#define TWEETER_TASK_RATE 20000
#define MIDI_NOTE_C4 60
#define TWEETER_RESET_RATE 10


// Tweeter initialisation
static tweeter_scale_t scale_table[] = TWEETER_SCALE_TABLE(TWEETER_TASK_RATE);
static tweeter_t tweeter;
static tweeter_obj_t tweeter_info;
static uint8_t collision_reset_counter = 0;


// Initialises the tweeter
void tweeter_task_init(void)
{
    tweeter = tweeter_init(&tweeter_info,TWEETER_TASK_RATE,scale_table);
    pio_config_set(PIEZO_PIO, PIO_OUTPUT_LOW);
}


// Updates the tweeter sound
void tweeter_task(void)
{
    pio_output_set(PIEZO_PIO,tweeter_update(tweeter));
}


// Makes a sound when there is a collision
void tweeter_collision(void)
{
    tweeter_note_play(tweeter,75,127);
    tweeter_task();
}


// Turns off the collision beep
void tweeter_collision_reset(void)
{
    if (collision_reset_counter == (PACER_RATE / TWEETER_RESET_RATE)) {
        tweeter_note_play(tweeter,0,127);
        tweeter_task();
        collision_reset_counter = 0;
    } else {
        collision_reset_counter++;
    }
}
