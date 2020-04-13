#ifndef TIMEFUNCTIONS_H
#define TIMEFUNCTIONS_H

extern unsigned long previousMillis;
extern int oldSecond;

extern short timezone;
extern short summertime;

extern void actionReadTime();
extern void actionSetGPSTime();
extern void progDispTimeDate();
extern void time_setup();
#endif