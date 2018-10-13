// File: Communicate
// Authors: Daniel Page (dwi65) and Caleb Smith (cas202)
// Date: 14 Oct 2018
// Descr: Communications module


#include "system.h"
#include "ir_serial.h"


// Indicators
#define STARTING_INDICATOR 30
#define WINNER_INDICATOR 35


// Sends starting singal when both devices are on "START" screen
void send_starting_signal(void)
{
    ir_serial_transmit(STARTING_INDICATOR);
}


// The loser sends a signal to the other device that they are the winner
void communicate_winner(void)
{
    ir_serial_transmit(WINNER_INDICATOR);
}


