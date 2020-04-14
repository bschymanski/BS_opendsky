#ifndef TIMEFUNCTIONS_H
#define TIMEFUNCTIONS_H

extern unsigned long previousMillis;
extern int oldSecond;

// V21N36 actionSetTime() and Date toggle Variables
extern bool actionSetTime_init;
extern bool actionSetTime_hour;
extern bool actionSetTime_minute;
extern bool actionSetTime_second;
extern bool actionSetTime_year;
extern bool actionSetTime_month;
extern bool actionSetTime_day;
extern bool actionSetTime_hour_done;
extern bool actionSetTime_minute_done;
extern bool actionSetTime_second_done;
extern bool actionSetTime_year_done;
extern bool actionSetTime_month_done;
extern bool actionSetTime_day_done;
extern short nowYear_temp;
extern short nowMonth_temp;
extern short nowDay_temp;
extern short nowHour_temp;
extern short nowMinute_temp;
extern short nowSecond_temp;
extern bool toggle_R1;
extern bool toggle_R2;
extern bool toggle_R3;

extern void actionReadTime();
extern void actionSetTime();
extern void actionSetGPSTime();
extern void progDispTimeDate();
extern void time_setup();
#endif