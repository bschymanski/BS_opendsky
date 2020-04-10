#include <Arduino.h>
#include <inputVerb.h>
#include <functions.h>
#include <main.h>
#include <neopixel.h>
#include <SevenSeg.h>
#include <keyboard.h>
#include <globals.h>

void inputVerb()
{
if ((verb_0 >= 0) && (verb_1 >= 0) && (verb_valid != true) && (verb_error !=true))
      { // we might possibly have a valid Verb! First and second Digits have been entered
        verb_temp = (verb_0*10)+verb_1;
        uint16_t idx = 0;
        bool found = false;
        printRegister(1,verb);
        printRegister(2, noun_temp);
        printRegister(3, NUM_PROG_TABLE_ENTRIES);
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
        { // we don't have a valid verb, sorry. Turn on the error lamp and clear verb_0 - noun_1 variables
          setLamp(white, lampOprErr);
          verb_error = true;
          //clearVerbfunction();
        }
      }
      else if ((verb_valid != true) && (verb_error !=true))
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
                clearVerbfunction();
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
                clearVerbfunction();
                verb_error = false;
                setLamp(off, lampOprErr);
              }
              clearVerbfunction();
              break;
            case keyEnter:
              break;
            case keyProceed:
              break;
            case keyReset:
              keyResetfunction();
              break;
            default:
              break;
          }
        }
        pressedKey = temporaryKey;
      }
      else if ((verb_valid != true) && (verb_error ==true))
      { // we do not have a valid verb yet
        setLamp(red, lampVerb); // for now, lets indicate that we are inputting a verb
        setLamp(off, lampSTBY); // we are no longer in Idle Mode
        temporaryKey = readKeyboard(); // lets save the return of the key
        if (pressedKey != temporaryKey)
        {
          switch(temporaryKey) // lets check what to do with the pressed key
          {
            case keyVerb: // why presse the Verb button again when already in modeInputVerb and we have a valid Verb? hmm maybe we want a new Verb?
              if (verb_valid == true)
              {
                clearVerbfunction();
              }
              break;
            case keyNoun: // if we are in verbinput mode, why press the noun button, makes sense only if we already have a valid verb.
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
              break;
            case keyPlus:
              break;
            case keyMinus:
              break;
            case keyClear:
              if (verb_error == true)
              {
                clearVerbfunction();
                verb_error = false;
                setLamp(off, lampOprErr);
              }
              clearVerbfunction();
              break;
            case keyEnter:
              break;
            case keyProceed:
              break;
            case keyReset:
              keyResetfunction();
              break;
            default:
              break;
          }
        }
        pressedKey = temporaryKey;
      }
      else if ((verb_valid == true) && (noun_valid != true))
      { // we have a valid verb and now lets see what we comes next: Lamptest, Program Input, Verb input?
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
          // a Noun will be entered once "ENTR" is pressed
          temporaryKey = readKeyboard();
          if (pressedKey != temporaryKey)
          {
            switch(temporaryKey)
            {
              case keyVerb:  
                break;
              case keyNoun:
                mode = modeInputNoun;
                setLamp(green, lampVerb);
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
                break;
              case keyPlus:
                break;
              case keyMinus:
                break;
              case keyClear:
                if (verb_error == true)
                {
                  clearVerbfunction();
                  verb_error = false;
                  setLamp(off, lampOprErr);
                }
                clearVerbfunction();
                break;
              case keyEnter:
                mode = modeIdle;
                setLamp(green, lampVerb);
                break;
              case keyProceed:
                break;
              case keyReset:
                keyResetfunction();
                break;
              default:
                break;
            }
          }
          pressedKey = temporaryKey;
        }
      }
}