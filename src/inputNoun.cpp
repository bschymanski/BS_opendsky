#include <Arduino.h>
#include <inputNoun.h>
#include <functions.h>
#include <main.h>
#include <neopixel.h>
#include <SevenSeg.h>
#include <keyboard.h>
#include <globals.h>

void inputNoun()
{
    if ((noun_0 >= 0) && (noun_1 >= 0) && (verb_valid == true) && (noun_valid != true) && (noun_error !=true))
    { // we might possibly have a valid noun, lets find out
        noun_temp = (noun_0*10)+noun_1;
        short idx = 0;
        bool found = false;
        printRegister(1,verb);
        printRegister(2, noun_temp);
        printRegister(3, NUM_PROG_TABLE_ENTRIES);
        //printRegister(3, idx);
        while ((idx < NUM_PROG_TABLE_ENTRIES) && (found == false))
        { // lets have a look at the Programtable for a valid noun / verb combination
            if ((ProgramTable[idx].VerbNumber == verb) && (ProgramTable[idx].NounNumber == noun_temp))
            {
                old2_noun = old1_noun;
                old1_noun = noun;
                noun = noun_temp;
                noun_valid = true;
                found = true;
                action = ProgramTable[idx].action;
                printRegister(1,verb);
                printRegister(2, noun);
                printRegister(3, idx);
                //if (noun != old1_noun)
                //{ // a new noun has been entered, but we had a valid noun before, now the old noun is invalid
                //  old2_noun = old1_noun;
                //  old1_noun = noun;
                //  noun = nounNone;
                //}
                noun_0 = -1;
                noun_1 = -1;
                setLamp(blue, lampNoun);
            }
            idx++;
            setLamp(red, lampNoun);
        }
        if (noun_valid == false)
        { // we don't have a valid noun, sorry. Turn on the error lamp and clear noun_0 - noun_1 variables
            setLamp(white, lampOprErr);
            noun_error = true;
            //clearNounfunction();
        }
    }
    else if ((noun_valid != true) && (verb_valid == true) && (noun_error !=true))
    { // we have a valid verb, are not in error Noun mode and don't have a valid noun yet, lets try and enter one
        setLamp(yellow, lampNoun);
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
                    if (noun_valid == false)
                    { // we don't have a valid verb yet, but we have entered a number, lets assing it to the proper place (first or second Digit)
                        if (noun_0 < 0)
                        { // First Verb Digit has been entered
                        noun_0 = temporaryKey;
                        printChar(0, 4, char(noun_0));
                        }
                        else if ((noun_0 >= 0) && (noun_1 < 0))
                        { // Second verb Digit has been entered
                        noun_1 = temporaryKey;
                        printChar(0, 5, char(noun_1));
                        }
                    }
                    break;
                case keyPlus:
                    break;
                case keyMinus:
                    break;
                case keyClear:
                    if (noun_error == true)
                    {
                        clearNounfunction();
                        noun_error = false;
                        setLamp(off, lampOprErr);
                    }
                    clearNounfunction();
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
    else if ((noun_valid != true) && (verb_valid == true) && (noun_error ==true))
    { 
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
                    break;
                case keyPlus:
                    break;
                case keyMinus:
                    break;
                case keyClear:
                    if (noun_error == true)
                    {
                        clearNounfunction();
                        noun_error = false;
                        setLamp(off, lampOprErr);
                    }
                    clearNounfunction();
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
    else if ((noun_valid == true) && (verb_valid == true) && (noun_error ==false))
    {  // we have a valid noun!
        setLamp(blue, lampNoun);
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
                    break;
                case keyPlus:
                    break;
                case keyMinus:
                    break;
                case keyClear:
                    if (noun_error == true)
                    {
                        clearNounfunction();
                        noun_error = false;
                        setLamp(off, lampOprErr);
                    }
                    clearNounfunction();
                    break;
                case keyEnter:
                    mode = modeIdle;
                    executeAction = true;
                    setLamp(green, lampNoun);
                    clearRegister(1);
                    clearRegister(2);
                    clearRegister(3);
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
        //mode = modeIdle;
    }
}