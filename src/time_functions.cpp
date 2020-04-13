#include <Arduino.h>
#include <time_functions.h>
#include <inputVerb.h>
#include <functions.h>
#include <main.h>
#include <neopixel.h>
#include <SevenSeg.h>
#include <keyboard.h>
#include <globals.h>

#include "RTClib.h"
RTC_DS1307 realTimeClock;

unsigned long previousMillis = 0;
int oldSecond = 0;

void time_setup()
{
    // Analog Pins for the keyboard
    realTimeClock.begin();
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
    printRegister(1,(now.hour()));
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
    short day = now.day();
    printVerb(day);
    short month = now.month();
    printNoun(month);
    printRegister(1,(now.hour()));
    printRegister(2,(now.minute()));
    printRegister(3,((now.second() * 100) + tenth));
}