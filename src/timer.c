#include "defines.h"
#include "raylib.h"

#include "timer.h"

double timesStart[32];
double timesEnd[32];

void startTimer(int timer, double Time)
{
    timesStart[timer] = Time;
}

void endTimer(int timer, double Time)
{
    timesEnd[timer] = Time;
}

double getTimer(int timer)
{
    return timesEnd[timer] - timesStart[timer];
}