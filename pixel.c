// File: Pong
// Authors: Daniel Page (dwi65) and Caleb Smith (cas202)
// Date: 10 Oct 2018
// Descr: The main file for a game of pong


#include "pixel.h"
#include "system.h"
#include "sound.h"
#include "display.h"

#define DNW 1
#define DNE 2
#define DSW 3
#define DSE 4
#define PACER_RATE 1000
#define PIXEL_RATE 5


static uint16_t counter_pixel = (PACER_RATE / PIXEL_RATE);


// North-West movement
void pixel_nw(int8_t* pixel_x,int8_t* pixel_y)
{
    (*pixel_x)++;
    (*pixel_y)++;
}

// South-East movement
void pixel_se(int8_t* pixel_x,int8_t* pixel_y)
{
    (*pixel_x)--;
    (*pixel_y)--;
}

// South-West movement
void pixel_sw(int8_t* pixel_x,int8_t* pixel_y)
{
    (*pixel_x)++;
    (*pixel_y)--;
}

// North-East movement
void pixel_ne(int8_t* pixel_x,int8_t* pixel_y)
{
    (*pixel_x)--;
    (*pixel_y)++;
}


// Contols the pixel movement in free space and at the boundaries of the game
void pixel_movement(int8_t* pixel_x,int8_t* pixel_y,int8_t* movement_state,int8_t row)
{
    if (counter_pixel == (PACER_RATE / PIXEL_RATE)) {
        // Slider rebound
        if ((*pixel_x == 3) && ((*pixel_y == 6) ||
                               (*pixel_y == 0)) && ((*pixel_y == row) ||
                                       (*pixel_y == row+1) || (*pixel_y == row-1))) {
            if (*movement_state == DNW) {
                *movement_state = DSE;
                tweeter_collision();
            } else if (*movement_state == DSW) {
                *movement_state = DNE;
                tweeter_collision();
            }
        } else if (((*pixel_x == 3 && *pixel_y == row) ||
                    (*pixel_x == 3 && *pixel_y == row+1) ||
                    (*pixel_x == 3 && *pixel_y == row-1))) {
            if (*movement_state == DNW) {
                *movement_state = DNE;
                tweeter_collision();
            } else if (*movement_state == DSW) {
                *movement_state = DSE;
                tweeter_collision();
            }
            // Top rebound
        } else if ((*pixel_x == 0 && *pixel_y == 6) ||
                   (*pixel_x == 1 && *pixel_y == 6) ||
                   (*pixel_x == 2 && *pixel_y == 6) ||
                   (*pixel_x == 3 && *pixel_y == 6)) {
            if (*movement_state == DNE) {
                *movement_state = DSE;
                tweeter_collision();
            } else if (*movement_state == DNW) {
                *movement_state = DSW;
                tweeter_collision();
            }
            // Bottom rebound
        } else if ((*pixel_x == 0 && *pixel_y == 0) ||
                   (*pixel_x == 1 && *pixel_y == 0) ||
                   (*pixel_x == 2 && *pixel_y == 0) ||
                   (*pixel_x == 3 && *pixel_y == 0)) {
            if (*movement_state == DSE) {
                *movement_state = DNE;
                tweeter_collision();
            } else if (*movement_state == DSW) {
                *movement_state = DNW;
                tweeter_collision();
            }
        }
        // Moves pixel by changing coordinates
        if (*movement_state == DNW) {
            pixel_nw(pixel_x,pixel_y);
        } else if (*movement_state == DNE) {
            pixel_ne(pixel_x,pixel_y);
        } else if (*movement_state == DSW) {
            pixel_sw(pixel_x,pixel_y);
        } else if (*movement_state == DSE) {
            pixel_se(pixel_x,pixel_y);
        }
        counter_pixel = 0;
    } else {
        counter_pixel++;
    }
    display_pixel_set(*pixel_x,*pixel_y,1);
}
