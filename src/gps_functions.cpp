#include <Arduino.h>
#include <gps_functions.h>
#include <globals.h>
#include <functions.h>
#include <neopixel.h>
#include <SevenSeg.h>
#include <TinyGPS++.h>
#include <time_functions.h>
TinyGPSPlus gps;


bool gpsread = false;
bool GPS_READ_STARTED = false;
bool gpsfix = false;

void GPS_setup()
{
    pinMode(7, OUTPUT);
    digitalWrite(7, LOW);
    Serial.begin(9600);
}



void readGPS()
{ // locacl function for reading the GPS Device
  if ((toggle == true) && (gpsreadtoggle == true))
  {
    setLamp(white, lampClk);
    digitalWrite(7,HIGH);
    delay(20);
    //Serial.begin(9600);
    while(Serial.available())
    {
      setLamp(white, lampAlt);
      if (gps.encode(Serial.read()))
      {
        //setLamp(orange, lampPosition);
        setLamp(orange, lampVel);
      }
    }
    digitalWrite(7,LOW);
    delay(20);
    gpsreadtoggle = false;
  }
  else if ((toggle == false) && (gpsreadtoggle == false))
  {
    setLamp(yellow, lampClk);
    digitalWrite(7,HIGH);
    delay(20);
    //Serial.begin(9600);
    while(Serial.available())
    {
      setLamp(white, lampAlt);
      if (gps.encode(Serial.read()))
      {
        //setLamp(orange, lampPosition);
        setLamp(orange, lampVel);
      }
    }
    digitalWrite(7,LOW);
    delay(20);
    gpsreadtoggle = true;
  }
}

void actionReadGPS()
{ // Read GPS Lat / Long / 
  if (toggle == true && gpsread == true)
  {
    gpsread = false;
    readGPS();
    setLamp(off, lampAlt);
    setLamp(off, lampVel);
  }
  else if ((toggle == false) && (gpsread == false))
  {
    gpsread = true;
    readGPS();
    setLamp(off, lampAlt);
    setLamp(off, lampVel);
    //if (gps.location.lat() != 0)
  }
  
  if (gps.location.isValid() == 1 )
  {
    gpsfix = true;
    if (gps.location.age() <= 20000)
    {
      setLamp(off, lampPosition);
      printRegister(1,gps.location.lat()*100);
      printRegister(2,gps.location.lng()*100);
      //printRegister(3,gps.satellites.value());
      printRegister(3, gps.location.age()/1000);
    }
    else if (gps.location.age() > 20000)
    {
      setLamp(white, lampPosition);
      clearRegister(1);
      clearRegister(2);
      //clearRegister(3);
      printRegister(3, gps.location.age()/1000);
    }
    else if (gps.location.age() > 100000)
    {
      gpsfix = false;
      setLamp(white, lampPosition);
      clearRegister(1);
      clearRegister(2);
      clearRegister(3);  
    }
  } 
  else if (gps.location.isValid() != 1)
  {
    gpsfix = false;
    setLamp(white, lampPosition);
    clearRegister(1);
    clearRegister(2);
    clearRegister(3);
  }
}

void actionReadGPSTime(bool blinkgpstime)
{ // Read GPS Time V16N38
  if (toggle == true && gpsread == true)
  {
    gpsread = false;
    readGPS();
    setLamp(off, lampAlt);
    setLamp(off, lampVel);
  }
  else if ((toggle == false) && (gpsread == false))
  {
    gpsread = true;
    readGPS();
    setLamp(off, lampAlt);
    setLamp(off, lampVel);
    //if (gps.location.lat() != 0)
  }
  
  if (gps.location.isValid() == 1 )
  {
    gpsfix = true;
    if (gps.location.age() <= 20000)
    {
      setLamp(off, lampPosition);
      gps_hour = gps.time.hour();
      gps_minute = gps.time.minute();
      gps_second = gps.time.second();
      if (blinkgpstime == false)
      {
        printRegister(1,gps_hour);
        printRegister(2,gps_minute);
        printRegister(3,(gps_second*100) + (gps.location.age()/10));
      }
      else if (blinkgpstime == true)
      {
        if (toggle == true && blinkgpstimetoggled == true)
        {
          printRegister(1,gps_hour);
          printRegister(2,gps_minute);
          printRegister(3,(gps_second*100) + (gps.location.age()/10));
          blinkgpstimetoggled = false;
        }
        else if (toggle == false && blinkgpstimetoggled == false)
        {
          clearRegister(1);
          clearRegister(2);
          clearRegister(3);
          blinkgpstimetoggled = true;
        }
      }
      
    }
    else if (gps.location.age() > 20000)
    {
      setLamp(yellow, lampPosition);
      gps_second = gps_second + (gps.location.age()/1000);
      clearRegister(1);
      clearRegister(2);
      //clearRegister(3);
      printRegister(3, (gps.time.second()*100) + (gps.location.age()/10));
    }
    else if (gps.location.age() > 100000)
    {
      gpsfix = false;
      setLamp(white, lampPosition);
      clearRegister(1);
      clearRegister(2);
      clearRegister(3);  
    }
  } 
  else if (gps.location.isValid() != 1)
  {
    gpsfix = false;
    setLamp(white, lampPosition);
    clearRegister(1);
    clearRegister(2);
    clearRegister(3);
  }
}