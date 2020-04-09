#include <Arduino.h>
// Wirelibrary for RTC and IMU
#include <Wire.h>



bool toggled = false;
bool toggled500 = false;
bool toggled250 = false;
bool toggledverbblink = false;

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
		/*VerbNumber            NounNumber          action                              Description */
		{ verbLampTest,         nounNotUsed,        action_LampTest                },   /* V35E N--   - Bulb test */
		{ verbDisplayDecimal,   nounIMUAttitude,    action_displayIMUAttitude      },   /* V16  N17 E - IMUAttitude */
    { verbInputProg,        nounNotUsed,        action_none                    },   /* V37  Prognumber E*/
    { verbDisplayDecimal,   nounClockTime,      action_displayRealTimeClock    }    /* V16  N36 E - Display Time */
};

#define NUM_PROG_TABLE_ENTRIES  (sizeof(ProgramTable)/sizeof(ProgramStruct))

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
  if (toggle == true)
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
        }
      }
      pressedKey = temporaryKey;
      break;
    }
    case modeInputVerb:
    { // a Verb is maybe going to be entered
      //
      // 
      if ((verb_0 >= 0) && (verb_1 >= 0) && (verb_valid != true) && (verb_error !=true))
      { // we might possibly have a valid Verb! First and second Digits have been entered
        verb_temp = (verb_0*10)+verb_1;
        uint16_t idx = 0;
        bool found = false;
        while ((idx < NUM_PROG_TABLE_ENTRIES) && (found == false))
        { // lets have a look at the Programtable for a valid verb
          if (ProgramTable[idx].VerbNumber == verb_temp)
          {
            old2_verb = old1_verb;
            old1_verb = verb;
            verb = verb_temp;
            verb_valid = true;
            found = true;
            if (verb != old1_verb)
            { // a new verb has been entered, but we had a valid verb before, now the old noun is invalid
              old2_noun = old1_noun;
              old1_noun = noun;
              noun = nounNone;
            }
            //mode = modeIdle;
            verb_0 = -1;
            verb_1 = -1;
            noun_0 = -1;
            noun_1 = -1;
            setLamp(blue, lampVerb);
          }
          idx++;
        }
        if (verb_valid == false)
        {
          setLamp(white, lampOprErr);
          verb_valid = false;
          verb_error = true;
        }
      }
      else if (verb_valid != true)
      { // we do not have a valid verb yet
        setLamp(orange, lampVerb); // for now, lets indicate that we are inputting a verb
        setLamp(off, lampSTBY); // we are no longer in Idle Mode
        temporaryKey = readKeyboard(); // lets save the return of the key
        if (pressedKey != temporaryKey)
        {
          switch(temporaryKey) // lets check what to do with the pressed key
          {
            case keyVerb: // why presse the Verb button again when already in modeInputVerb and we have a valid Verb? hmm maybe we want a new Verb?
              if (verb_valid == true)
              {
                verb_valid = false;
                verb = 0;
                noun = 0;
                verb_0 = -1;
                verb_1 = -1;
              }
              break;
            case keyNoun: // if we are in verbinput mode, why press the noun button, makes sense only if we already have a valid verb.
              if (verb_valid == true)
              {
                mode = modeInputNoun;
              }
              break;
            case keyRelease:  // we changed our mind and don't want to input a verb, back to idle mode
              mode = modeIdle;
              setLamp(green, lampVerb);
              break;
            // Now we are entering the numbers for the verb!
            case keyNumber0:  
            case keyNumber1:
            case keyNumber2:
            case keyNumber3:
            case keyNumber4:
            case keyNumber5:
            case keyNumber6:
            case keyNumber7:
            case keyNumber8:
            case keyNumber9:
              if (verb_valid == false)
              { // we don't have a valid verb yet, but we have entered a number, lets assing it to the proper place (first or second Digit)
                if (verb_0 < 0)
                { // First Verb Digit has been entered
                  verb_0 = temporaryKey;
                  printChar(0, 0, char(verb_0));
                }
                else if ((verb_0 >= 0) && (verb_1 < 0))
                { // Second verb Digit has been entered
                  verb_1 = temporaryKey;
                  printChar(0, 1, char(verb_1));
                }
              }
            case keyPlus:
              break;
            case keyMinus:
              break;
            case keyClear:
              if (verb_error == true)
              {
                verb_0 = -1;
                verb_1 = -1;
                printChar(0, 0, ' ');
                printChar(0, 1, ' ');
                verb_valid = false;
                verb_error = false;
                setLamp(off, lampOprErr);
              }
              verb_0 = -1;
              verb_1 = -1;
              printChar(0, 0, ' ');
              printChar(0, 1, ' ');
              verb_valid = false;
              break;
            case keyEnter:
              break;
            case keyProceed:
              break;
            case keyReset:
              mode = modeIdle;
              setLamp(green, lampVerb);
              setLamp(green, lampNoun);
              setLamp(green, lampProg);
              noun = 0;
              verb = 0;
              prog = 0;
              verb_valid = false;
              break;
            default:
              break;
          }
        }
        pressedKey = temporaryKey;
      }
      else if ((verb_valid == true) && (noun_valid != true))
      {
        if (verb == verbInputProg)
        {
          mode = modeInputProgram;
        }
        else if (verb == verbLampTest)
        {
          // lamptest will follow
        }
        else
        { // Neither a Programm will be entered nor a Lamptest will be done 
          // a Noun will be entered one "ENTR" is pressed
          temporaryKey = readKeyboard();
          if (pressedKey != temporaryKey)
          {
            switch(temporaryKey)
            {
              case keyVerb:  
                break;
              case keyNoun:
                break;
              case keyRelease:
                break;
              case keyNumber0:
              case keyNumber1:
              case keyNumber2:
              case keyNumber3:
              case keyNumber4:
              case keyNumber5:
              case keyNumber6:
              case keyNumber7:
              case keyNumber8:
              case keyNumber9:
              case keyPlus:
                break;
              case keyMinus:
                break;
              case keyClear:
                break;
              case keyEnter:
                mode = modeInputNoun;
                break;
              case keyProceed:
                break;
              case keyReset:
                mode = modeIdle;
                setLamp(green, lampVerb);
                setLamp(green, lampNoun);
                setLamp(green, lampProg);
                noun = 0;
                verb = 0;
                prog = 0;
                verb_valid = false;
                break;
              default:
                break;
            }
          }
          pressedKey = temporaryKey;
        }
      }
      break;
    }
    case modeInputNoun:
    {
      setLamp(orange, lampNoun);
      temporaryKey = readKeyboard();
      if (pressedKey != temporaryKey)
      {
        switch(temporaryKey)
        {
          case keyVerb:
            break;
          case keyNoun:
            break;
          case keyRelease:
            break;
          case keyNumber0:
          case keyNumber1:
          case keyNumber2:
          case keyNumber3:
          case keyNumber4:
          case keyNumber5:
          case keyNumber6:
          case keyNumber7:
          case keyNumber8:
          case keyNumber9:
          case keyPlus:
            break;
          case keyMinus:
            break;
          case keyClear:
            break;
          case keyEnter:
            break;
          case keyProceed:
            break;
          case keyReset:
            mode = modeIdle;
            setLamp(green, lampVerb);
            setLamp(green, lampNoun);
            setLamp(green, lampProg);
            noun = 0;
            verb = 0;
            prog = 0;
            verb_valid = false;
            break;
          default:
            break;
        }
      }
      pressedKey = temporaryKey;
      break;
    }
    case modeInputProgram:
    {
      setLamp(blue, lampProg);
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
      temporaryKey = readKeyboard();
      if (pressedKey != temporaryKey)
      {
        switch(temporaryKey)
        {
          case keyVerb:
            break;
          case keyNoun:
            break;
          case keyRelease:
            break;
          case keyNumber0:
          case keyNumber1:
          case keyNumber2:
          case keyNumber3:
          case keyNumber4:
          case keyNumber5:
          case keyNumber6:
          case keyNumber7:
          case keyNumber8:
          case keyNumber9:
          case keyPlus:
            break;
          case keyMinus:
            break;
          case keyClear:
            break;
          case keyEnter:
            break;
          case keyProceed:
            break;
          case keyReset:
            mode = modeIdle;
            setLamp(green, lampVerb);
            setLamp(green, lampNoun);
            setLamp(green, lampProg);
            noun = 0;
            verb = 0;
            prog = 0;
            verb_valid = false;
            break;
          default:
            break;
        }
      }
      pressedKey = temporaryKey;
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