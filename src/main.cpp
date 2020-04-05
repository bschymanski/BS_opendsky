#include <Arduino.h>
// Wirelibrary for RTC and IMU
#include <Wire.h>

// Neopixel functions
#include <neopixel.h>
// SevenSegment functions
#include <SevenSeg.h>

void setup() {
  // Initialize the Neopixels
  NeoPixelSetup();
  setLamp(green, lampVerb);
  setLamp(green, lampNoun);
  setLamp(green, lampProg);
  setLamp(white, lampNoAtt);
  // initialize the Sevensegments
  SevenSegSetup();
}

void loop() {
  // put your main code here, to run repeatedly:
  
}