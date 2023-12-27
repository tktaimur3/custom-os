#include "timer.h"

uint64 ticks = 0;

void timer_callback()
{
    ticks++;
}

// each tick is 100 ms
void sleep_ticks(uint64 sleep_ticks)
{
    volatile uint64 target = ticks+sleep_ticks;

    while (ticks < target);
}