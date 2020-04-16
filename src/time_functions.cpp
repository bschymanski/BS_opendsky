#include <Arduino.h>
#include <time_functions.h>
#include <inputVerb.h>
#include <functions.h>
#include <main.h>
#include <neopixel.h>
#include <SevenSeg.h>
#include <keyboard.h>
#include <globals.h>
#include <gps_functions.h>
#include <Timezone.h>

#include "RTClib.h"
RTC_DS1307 realTimeClock;

unsigned long previousMillis = 0;
int oldSecond = 0;

// V21N36 actionSetTime() and Date toggle Variables
bool actionSetTime_init = false;
bool actionSetTime_hour = false;
bool actionSetTime_minute = false;
bool actionSetTime_second = false;
bool actionSetTime_year = false;
bool actionSetTime_month = false;
bool actionSetTime_day = false;
bool actionSetTime_hour_done = false;
bool actionSetTime_minute_done = false;
bool actionSetTime_second_done = false;
bool actionSetTime_year_done = false;
bool actionSetTime_month_done = false;
bool actionSetTime_day_done = false;
short nowYear_temp = 0;
short nowMonth_temp = 0;
short nowDay_temp = 0;
short nowHour_temp = 0;
short nowMinute_temp = 0;
short nowSecond_temp = 0;
bool toggle_R1 = false;
bool toggle_R2 = false;
bool toggle_R3 = false;

//https://github.com/JChristensen/Timezone?utm_source=platformio&utm_medium=piohome
/*
Define a TimeChangeRule as follows:
TimeChangeRule myRule = {abbrev, week, dow, month, hour, offset};

Where:
abbrev is a character string abbreviation for the time zone; it must be no longer than five characters.
week is the week of the month that the rule starts.
dow is the day of the week that the rule starts.
hour is the hour in local time that the rule starts (0-23).
offset is the UTC offset in minutes for the time zone being defined.
For convenience, the following symbolic names can be used:
week: First, Second, Third, Fourth, Last
dow: Sun, Mon, Tue, Wed, Thu, Fri, Sat
month: Jan, Feb, Mar, Apr, May, Jun, Jul, Aug, Sep, Oct, Nov, Dec
*/
TimeChangeRule myDST = {"EDT", Last, Sun, Mar, 2, +120};    //Daylight time = UTC + 2 hours
TimeChangeRule mySTD = {"EST", Last, Sun, Oct, 2, +60};     //Standard time = UTC + 1 hours

Timezone myTZ(myDST, mySTD);


TimeChangeRule* tcr;               //pointer to the time change rule, use to get TZ abbrev
time_t now_time_t, utc_time_t, local_time_t;


void time_setup()
{
    // Analog Pins for the keyboard
    realTimeClock.begin();
}

void actionReadTime()
{
    // read time from real-time clock (RTC)
    // the Timezone Library uses the time_t structure, where the RTC Lib used here uses DateTime structure. They need to be converted....
    // https://arduino.stackexchange.com/questions/17639/the-difference-between-time-t-and-datetime/17650#17650
    DateTime now = realTimeClock.now();
    DateTime now_time_t = realTimeClock.now();
    time_t utc_time_t = now_time_t.unixtime();
    time_t local_time_t = myTZ.toLocal(utc_time_t, &tcr);
    // 10th and hundreds of seconds
    if( oldSecond < now.second() )
    {
        oldSecond = now.second();
        previousMillis = millis();
    }
    int hundreds = ( ( millis()-previousMillis )/10 )%100;
    int tenth = hundreds - (hundreds % 10);
    printVerb(verb);
    printNoun(noun);
    printRegister(1, hour(local_time_t));
    printRegister(2,(now.minute()));
    printRegister(3,((now.second() * 100) + tenth));
}

void progDispTimeDate()
{  // read time from real-time clock (RTC)
  // the Timezone Library uses the time_t structure, where the RTC Lib used here uses DateTime structure. They need to be converted....
  // https://arduino.stackexchange.com/questions/17639/the-difference-between-time-t-and-datetime/17650#17650
  DateTime now = realTimeClock.now();
  DateTime now_time_t = realTimeClock.now();
  time_t utc_time_t = now_time_t.unixtime();
  time_t local_time_t = myTZ.toLocal(utc_time_t, &tcr);
  // 10th and hundreds of seconds
  if( oldSecond < now.second() )
  {
      oldSecond = now.second();
      previousMillis = millis();
  }
  int hundreds = ( ( millis()-previousMillis )/10 )%100;
  int tenth = hundreds - (hundreds % 10);
  short day = now.day();
  printVerb(day);
  short month = now.month();
  printNoun(month);
  printRegister(1,(hour(local_time_t)));
  printRegister(2,(now.minute()));
  printRegister(3,((now.second() * 100) + tenth));
}

void actionSetGPSTime()
{ // Read the Time from the GPS, press PROceed button, to accept the time  /V21 N38
      actionReadGPSTime(true); // read GPS Time an let the Register Blink
      temporaryKey = readKeyboard();
      //
      if (pressedKey != temporaryKey)
      {
        switch(temporaryKey)
        {
          case keyProceed:
            {
              DateTime now = realTimeClock.now();
              int nowYear = now.year();
              int nowMonth = now.month();
              int nowDay = now.day();
              int nowHour = gps_hour;
              int nowMinute = gps_minute;
              int nowSecond = gps_second;
              realTimeClock.adjust(DateTime(nowYear, nowMonth, nowDay, nowHour, nowMinute, nowSecond));
              // we set the Time, now we should display it via Action
              mode = modeIdle;
              setLamp(off, lampClk);
              clearVerbfunction();
              clearNounfunction();
              verb = 16;
              noun = 36;
              verb_valid = true;
              noun_valid = true;
              action = action_displayRealTimeClock;
              executeAction = true;
              break;
            }
          case keyVerb:
            clearVerbfunction();
            clearNounfunction();
            mode = modeInputVerb;
            break;
          case keyNoun:
            if (verb_valid == true)
            {
              if (executeAction == true)
              { // we have a running action. The Verb key hase been pressed, so a new action is going to be
                // entered, the current action is therefore invalid
                clearNounfunction();
                mode = modeInputNoun;
              }
              clearNounfunction();
              mode = modeInputNoun;
            }
            break;
          case keyRelease:
            mode = modeIdle;
            clearVerbfunction();
            clearNounfunction();
            setLamp(off, lampKeyRelease);
            break;
          case keyReset:
            keyResetfunction();
            break;  
        }
      }
      pressedKey = temporaryKey;
}

void actionSetTime()
{   // read & display time from hardware real-time clock (RTC) /V21 N36
    
    /* 
    global variables to be used:
    bool actionSetTime_init = false;
    bool actionSetTime_hour = false;
    bool actionSetTime_minute = false;
    bool actionSetTime_second = false;
    bool actionSetTime_year = false;
    bool actionSetTime_month = false;
    bool actionSetTime_day = false;
    bool actionSetTime_hour_done = false;
    bool actionSetTime_minute_done = false;
    bool actionSetTime_second_done = false;
    bool actionSetTime_year_done = false;
    bool actionSetTime_month_done = false;
    bool actionSetTime_day_done = false;
    short nowHour_temp = 0;
    short nowMinute_temp = 0;
    short nowSecond_temp = 0;
    bool toggle_R1 = false;
    bool toggle_R2 = false;
    bool toggle_R3 = false;
    */
    // first initialize temporary hour, minute and Second variables
    if (actionSetTime_init == false)
    { // read the current realtime clock
      DateTime now = realTimeClock.now();
      nowYear_temp = now.year();
      nowMonth_temp = now.month();
      nowDay_temp = now.day();
      nowHour_temp = now.hour();
      nowMinute_temp = now.minute();
      nowSecond_temp = now.second();
      actionSetTime_init = true;
    }
    else if (actionSetTime_init == true)
    { // we want to set the hour, minute and second
      if ((actionSetTime_hour == false) && (actionSetTime_minute == false) && (actionSetTime_second == false))
      { // neither hour, minute or second have been set
        actionSetTime_hour = true;
        IncDecNumber = nowHour_temp;
        IncDecNumber_done = false;
      }
      //else if ((actionSetTime_hour == false) && (actionSetTime_minute == true) && (actionSetTime_second == false))
      //{ // neither hour, minute or second have been set
      //  IncDecNumber = nowMinute_temp;
      //  IncDecNumber_done = false;
      //}
    }
    
    
    if ((actionSetTime_hour == true) && (actionSetTime_minute == false) && (actionSetTime_second == false) && (IncDecNumber_done != true))
    {
      nowHour_temp = IncDecNumber;
      setLamp(yellow, lampClk);
    }
    else if ((actionSetTime_hour == true) && (actionSetTime_minute == false) && (actionSetTime_second == false) && (IncDecNumber_done == true))
    {
      actionSetTime_hour_done = true;
      actionSetTime_hour = false;
      actionSetTime_minute = true;
      actionSetTime_second = false;
      nowHour_temp = IncDecNumber;
      IncDecNumber_done = false;
      IncDecNumber = nowMinute_temp;
      setLamp(white, lampClk);
    }
    else if ((actionSetTime_hour == false) && (actionSetTime_minute == true) && (actionSetTime_second == false) && (IncDecNumber_done != true))
    {
      nowMinute_temp = IncDecNumber;
      setLamp(blue, lampClk);
    }
    else if ((actionSetTime_hour == false) && (actionSetTime_minute == true) && (actionSetTime_second == false) && (IncDecNumber_done == true))
    {
      actionSetTime_hour_done = true;
      actionSetTime_hour = false;
      actionSetTime_minute = false;
      actionSetTime_second = true;
      nowMinute_temp = IncDecNumber;
      IncDecNumber_done = false;
      IncDecNumber = nowSecond_temp;
      setLamp(green, lampClk);
    }
    else if ((actionSetTime_hour == false) && (actionSetTime_minute == false) && (actionSetTime_second == true) && (IncDecNumber_done != true))
    {
      nowSecond_temp = IncDecNumber;
      setLamp(red, lampClk);
    }
    else if ((actionSetTime_hour == false) && (actionSetTime_minute == false) && (actionSetTime_second == true) && (IncDecNumber_done == true))
    {
      actionSetTime_hour_done = true;
      actionSetTime_hour = true;
      actionSetTime_minute = true;
      actionSetTime_second = true;
      nowSecond_temp = IncDecNumber;
      IncDecNumber_done = false;
      IncDecNumber = 0;
      setLamp(green, lampClk);
    }


    if (toggle500 == true)
    {
      printRegister(1,nowHour_temp);
      printRegister(2,nowMinute_temp);
      printRegister(3,(nowSecond_temp * 100)); // emulate milliseconds
    }
    else if (toggle500 == false)
    {
      printRegister(1,nowHour_temp,1,actionSetTime_hour);
      printRegister(2,nowMinute_temp,1,actionSetTime_minute);
      printRegister(3,(nowSecond_temp * 100),1,actionSetTime_second); // emulate milliseconds
    }
    if (actionSetTime_second_done == true)
    {
      realTimeClock.adjust(DateTime(nowYear_temp, nowMonth_temp, nowDay_temp, nowHour_temp, nowMinute_temp, nowSecond_temp));
      actionSetTime_init = false;
      actionSetTime_hour = false;
      actionSetTime_minute = false;
      actionSetTime_second = false;
      actionSetTime_year = false;
      actionSetTime_month = false;
      actionSetTime_day = false;
      actionSetTime_hour_done = false;
      actionSetTime_minute_done = false;
      actionSetTime_second_done = false;
      actionSetTime_year_done = false;
      actionSetTime_month_done = false;
      actionSetTime_day_done = false;
      nowHour_temp = 0;
      nowMinute_temp = 0;
      nowSecond_temp = 0;
      toggle_R1 = false;
      toggle_R2 = false;
      toggle_R3 = false;
      mode = modeIdle;
      setLamp(off, lampClk);
      clearVerbfunction();
      clearNounfunction();
      verb = 16;
      noun = 36;
      verb_valid = true;
      noun_valid = true;
      action = action_displayRealTimeClock;
      executeAction = true;
    }
}