#include <Arduino.h>
// Wirelibrary for RTC and IMU
#include <Wire.h>



bool toggled = false;
bool toggled500 = false;
bool toggled250 = false;
bool toggledverbblink = false;



bool blinkverb = true;
bool blinknoun = true;
bool imutoggle = true;
bool printregtoggle = true;
bool uplink_compact_toggle = true;


#include <timer.h>
auto timer = timer_create_default();
#include <functions.h>
#include <keyboard.h>
// Neopixel functions
#include <neopixel.h>
// SevenSegment functions
#include <SevenSeg.h>
#include <globals.h>
#include <main.h>



bool global_state_1sec = true;
bool global_state_500 = true;
bool global_state_250 = true;
bool toggle500 = true;
bool toggle250 = true;
bool toggle = true;
bool printed = true;




#include <inputVerb.h>
#include <inputNoun.h>
#include <inputProgram.h>

// some toggle definitions for blinking stuff

// initialisation of Mode-, Verb, Noun, Program and Action Modes
// current Mode-, Verb, Noun, Program and Action Values
short mode    = modeIdle;     // eq 0
short verb    = verbNone;     // eq 0
short noun    = verbNone;     // eq 0
short prog    = programNone;  // eq 0
short action  = action_none;  // eq 0

// old Mode-, Verb, Noun, Program and Action Values
short old1_mode    = modeIdle;     // eq 0
short old1_verb    = verbNone;     // eq 0
short old1_noun    = verbNone;     // eq 0
short old1_prog    = programNone;  // eq 0
short old1_action  = action_none;  // eq 0

// before old Mode-, Verb, Noun, Program and Action values
short old2_mode    = modeIdle;     // eq 0
short old2_verb    = verbNone;     // eq 0
short old2_noun    = verbNone;     // eq 0
short old2_prog    = programNone;  // eq 0
short old2_action  = action_none;  // eq 0

bool verb_valid = false;
bool noun_valid = false;
bool prog_valid = false;

short verb_0 = -1;
short verb_1 = -1;

short noun_0 = -1;
short noun_1 = -1;

short prog_0 = -1;
short prog_1 = -1;

short temporaryKey  = keyNone;
short pressedKey    = keyNone;

short verb_temp = -1;
short noun_temp = -1;
short prog_temp = -1;

bool verb_error = false;
bool noun_error = false;
bool prog_error = false;

void serialprintmainstates()
{
    Serial.write(12);//ASCII for a Form feed
    Serial.print("mode : ");Serial.print(mode);Serial.print(" verb : ");Serial.print(verb);Serial.print(" noun : ");Serial.print(noun);Serial.print(" prog : ");Serial.print(prog);Serial.print(" action : ");Serial.println(action);
    Serial.print("temporaryKey : ");Serial.print(temporaryKey);
}

void setup()
{
  // initialize keyboard Pins
  keyboardsetup();
  // Initialize the Neopixels
  NeoPixelSetup();
  setLamp(green, lampVerb);
  setLamp(green, lampNoun);
  setLamp(green, lampProg);
  setLamp(white, lampNoAtt);
  // initialize the Sevensegments
  SevenSegSetup();
  Serial.begin(9600);
  // Toggle 
  timer.every(1000, toggle_timer);
  timer.every(50, toggle_timer_500);
  timer.every(250, toggle_timer_250);
}

void loop()
{
  timer.tick();
  if (toggle == true)  // this little Blinking routine is an indication that we are still passing the Main loop and are not stuck somewhere
  {
      if ((toggle250 == true) && (toggled250 == false))
      {
          setLamp(white, lampClk);
          toggled250 = true;
      }
      else if ((toggle250 == false) && (toggled250 == true))
      {
          setLamp(off, lampClk);
      }
  }
  else
  {
      //setLamp(off, lampClk);
      if ((toggle250 == true) && (toggled250 == true))
      {
          setLamp(white, lampClk);
          toggled250 = false;
      }
      else if ((toggle250 == false) && (toggled250 == false))
      {
          setLamp(off, lampClk);
      }
  }
  
  switch (mode)
  {
    case modeIdle:
    {
      // Idlemode, dsky just waits for a proper key
      setLamp(white, lampSTBY);
      printProg(prog);
      printVerb(verb);
      printNoun(noun);
      // Read the key and determine which mode to be in
      temporaryKey = readKeyboard();
      //
      if (pressedKey != temporaryKey)
      {
        switch(temporaryKey)
        {
          case keyVerb:
            mode = modeInputVerb;
            break;
          case keyNoun:
            mode = modeInputNoun;
            break;
          case keyRelease:
            mode = modeIdle;
            break;
          case keyReset:
            keyResetfunction();
            break;  
        }
      }
      pressedKey = temporaryKey;
      break;
    }
    case modeInputVerb:
    { // a Verb is maybe going to be entered
      //
      // 
      inputVerb();
      break;
    }
    case modeInputNoun:
    { // A Noun is going to be entered (maybe)
      inputNoun();
      break;
    }
    case modeInputProgram:
    {
      if ((toggle == true) && (toggledverbblink == true))
      {
        toggledverbblink = false;
        printVerb(verb, blink=false);
      }
      else if ((toggle == false) && (toggledverbblink == false))
      {
        toggledverbblink = true;
        printVerb(verb, blink=true);
      }
      inputProgram();
      break;
    }
    case modeInputNumber:
    {
      setLamp(white, lampTemp);
      break;
    }
    case modeLampTest:
    {
      break;
    }
    default:
    {
      // there should be no Mode...
      setLamp(orange, lampProg);
      break;
    }
  }


  // Serial Print the mode, noun, etc
  if ((toggle250 == true) && (printed == true))
  {
    serialprintmainstates();
    printed = false;
  }
  else if ((toggle250 == false) && (printed == false))
  {
    serialprintmainstates();
    printed = true;
  }
 
}