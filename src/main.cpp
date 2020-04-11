#include <Arduino.h>
// Wirelibrary for RTC and IMU
#include <Wire.h>
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
#include <inputVerb.h>
#include <inputNoun.h>
#include <inputProgram.h>
#include <time_functions.h>

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
  time_setup();
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
            if (verb_valid == true)
            {
              mode = modeInputNoun;
            }
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
    case modeExcuteAction:
    {
      setLamp(white, lampUplinkActy);  // just to indicate we are in Action execution Mode
      printProg(prog);
      printVerb(verb);
      printNoun(noun);
      // we have a valid Action but we have to figure out which one
      if ((verb_valid == true) && (noun_valid == true) && (lookupAction == false))
      {
        // lets check which action to perform, but as always scan for some keys first
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
              if (verb_valid == true)
              {
                mode = modeInputNoun;
              }
              break;
            case keyRelease:
              mode = modeIdle;
              clearRegister(1);
              clearRegister(2);
              clearRegister(3);
              action = action_none;
              setLamp(off, lampUplinkActy);
              break;
            case keyReset:
              keyResetfunction();
              break;  
          }
        }
        pressedKey = temporaryKey;
        short idx = 0;
        bool found = false;
        while ((idx < NUM_PROG_TABLE_ENTRIES) && (found == false))
        { // lets have a look at the Programtable for a valid noun / verb combination
            if ((ProgramTable[idx].VerbNumber == verb) && (ProgramTable[idx].NounNumber == noun))
            {
                lookupAction = true;
                found = true;
                action = ProgramTable[idx].action;
                printRegister(1, action);
                printRegister(3, idx);
            }
            idx++;
        }
        if (found == false)
        { // Why didn't we find the Action if noun and verb are correct?
            setLamp(orange, lampProgCond);
            //clearprogfunction();
        }
      }
      else if ((verb_valid == true) && (noun_valid == true) && (lookupAction == true))
      { // lets execute the action
        printProg(prog);
        printVerb(verb);
        printNoun(noun);
        // As always, lets scan some keys first
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
              if (verb_valid == true)
              {
                mode = modeInputNoun;
              }
              break;
            case keyRelease:
              mode = modeIdle;
              clearRegister(1);
              clearRegister(2);
              clearRegister(3);
              action = action_none;
              lookupAction = false;
              setLamp(off, lampUplinkActy);
              setLamp(off, lampTemp);
              clearVerbfunction();
              break;
            case keyReset:
              keyResetfunction();
              break;  
          }
        }
        pressedKey = temporaryKey;
        // now, finally we can really execute the action!
        switch (action)
        {
          case action_displayRealTimeClock:
          {
            actionReadTime();
            break;
          }
          case action_none:
          {
            clearVerbfunction();
            break;
          }
        }
      }
      break;
    }
    default:
    {
      // there should be no Mode...
      setLamp(orange, lampProgCond);
      break;
    }
  }
  // blink the OPR Error Lamp if there is an input Error
  if ((verb_error == true) || (noun_error == true) || (prog_error == true))
  {
    if (toggle == true)
    {
      setLamp(white, lampOprErr);
    }
    else if (toggle == false)
    {
      setLamp(off, lampOprErr);
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