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

#include "RTClib.h"
RTC_DS1307 realTimeClock;

unsigned long previousMillis = 0;
int oldSecond = 0;

// Germany is UTC+1
short timezone = 1;
short summertime = 1;

void time_setup()
{
    // Analog Pins for the keyboard
    realTimeClock.begin();
}

short calclocaltime(short hour)
{
  if ((hour + timezone + summertime - 24) >= 0)
  {
    hour = hour +timezone + summertime -24;
  }
  else if ((hour + timezone + summertime - 24) < 0) 
  {
    hour = hour + timezone + summertime;
  }
  return hour;
}

void actionReadTime()
{
    // read time from real-time clock (RTC)
    DateTime now = realTimeClock.now();
    // 10th and hundreds of seconds
    if( oldSecond < now.second() )
    {
        oldSecond = now.second();
        previousMillis = millis();
    }
    int hundreds = ( ( millis()-previousMillis )/10 )%100;
    int tenth = hundreds - (hundreds % 10);
    //valueForDisplay[register1Position] = (now.hour());
    //valueForDisplay[register2Position] = (now.minute());
    //valueForDisplay[register3Position] = ((now.second() * 100) + tenth);
    //setDigits();
    printVerb(verb);
    printNoun(noun);
    short utchour = now.hour();
    short locaclhour = calclocaltime(utchour);
    printRegister(1,(locaclhour));
    printRegister(2,(now.minute()));
    printRegister(3,((now.second() * 100) + tenth));
}

void progDispTimeDate()
{
    
    // read time from real-time clock (RTC)
    DateTime now = realTimeClock.now();
    // 10th and hundreds of seconds
    if( oldSecond < now.second() )
    {
        oldSecond = now.second();
        previousMillis = millis();
    }
    int hundreds = ( ( millis()-previousMillis )/10 )%100;
    int tenth = hundreds - (hundreds % 10);
    //valueForDisplay[register1Position] = (now.hour());
    //valueForDisplay[register2Position] = (now.minute());
    //valueForDisplay[register3Position] = ((now.second() * 100) + tenth);
    //setDigits();
    short utchour = now.hour();
    short locaclhour = calclocaltime(utchour);
    short day = now.day();
    if (locaclhour < utchour)
    {
      day = day+1;
    }
    printVerb(day);
    short month = now.month();
    printNoun(month);
    printRegister(1,(locaclhour));
    printRegister(2,(now.minute()));
    printRegister(3,((now.second() * 100) + tenth));
}

void actionSetGPSTime()
{ // Read the Time from the GPS, press PROceed button, to accept the time  /V26 N36
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