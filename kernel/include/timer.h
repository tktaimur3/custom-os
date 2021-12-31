#include "typedefs.h"

#ifndef TIMER_H
#define TIMER_H

#define FREQ   100     // Timer interrupt frequency, how many times a second 'ticks' is incremented

void timer_callback();
void sleep_ticks(uint64 sleep_ticks);

#endif