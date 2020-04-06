#include <Arduino.h>
// Wirelibrary for RTC and IMU
#include <Wire.h>



bool toggled = false;
bool toggled500 = false;
bool toggled250 = false;

bool blink = false;
bool blinkverb = true;
bool blinknoun = true;
bool blinkprog = true;
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

#include <main.h>

bool global_state_1sec = true;
bool global_state_500 = true;
bool global_state_250 = true;
bool toggle500 = true;
bool toggle250 = true;
bool toggle = true;
bool printed = true;


/* This structure contains all of the programs in the system. The noun number is
   optional. The program function is called when the verb and noun are entered. if
   the program should be called repeatedly, then you will need to setup a thread
   and callback function inside the program function. The set data function is
   used to give numerial data to the function. The Get Disp Data Function Should
   return display data for the 7 segment display driver. Run State is used internally
   by the Program module to track the state of all of the current programs.*/
ProgramStruct ProgramTable[] =
{
		/*verb                  noun                Action                              Description */
		{ verbLampTest,         nounNone,           action_LampTest                },   /* V35E N--   - Bulb test */
		{ verbDisplayDecimal,   nounIMUAttitude,    action_displayIMUAttitude      },   /* V16  N17 E - IMUAttitude */
    { verbDisplayDecimal,   nounClockTime,      action_displayRealTimeClock    }    /* V16  N36 E - Display Time */
};

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


short temporaryKey  = keyNone;
short pressedKey    = keyNone;

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
    }
  }
  pressedKey = temporaryKey;

  // Serial Print the mode, noun, etc
  if ((toggle250 == true) && (printed == true))
  {
    Serial.write(12);//ASCII for a Form feed
    Serial.print("mode : ");Serial.print(mode);Serial.print(" verb : ");Serial.print(verb);Serial.print(" noun : ");Serial.print(noun);Serial.print(" prog : ");Serial.print(prog);Serial.print(" action : ");Serial.println(action);
    Serial.print("temporaryKey : ");Serial.print(temporaryKey);
    printed = false;
  }
  else if ((toggle250 == false) && (printed == false))
  {
    Serial.write(12);//ASCII for a Form feed
    Serial.print("mode : ");Serial.print(mode);Serial.print(" verb : ");Serial.print(verb);Serial.print(" noun : ");Serial.print(noun);Serial.print(" prog : ");Serial.print(prog);Serial.print(" action : ");Serial.println(action);
    Serial.print("temporaryKey : ");Serial.print(temporaryKey);
    printed = true;
  }
 
}