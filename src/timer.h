#ifndef RAYTRIS_TIMER_H
#define RAYTRIS_TIMER_H

#include "raylib.h"

#define TIMER_UPDATE 0

#define STARTTIMER(timer) startTimer(timer, GetTime())
#define ENDTIMER(timer) endTimer(timer, GetTime())

void startTimer(int timer, double Time);
void endTimer(int timer, double Time);

double getTimer(int timer);

#endif // RAYTRIS_TIMER_H