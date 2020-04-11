#include <Arduino.h>
#include <inputProgram.h>
#include <functions.h>
#include <main.h>
#include <neopixel.h>
#include <SevenSeg.h>
#include <keyboard.h>
#include <globals.h>

void inputProgram()
{
    if ((prog_0 >= 0) && (prog_1 >= 0) && (verb_valid == true) && (prog_valid != true) && (prog_error !=true))
    { // we might possibly have a valid noun, lets find out
        prog_temp = (prog_0*10)+prog_1;
        short idx = 0;
        bool found = false;
        printRegister(1, verb);
        printRegister(2, prog_temp);
        printRegister(3, NUM_PROG_TABLE_ENTRIES);
        //printRegister(3, idx);
        while ((idx < NUM_PROG_TABLE_ENTRIES) && (found == false))
        { // lets have a look at the Programtable for a valid noun / verb combination
            if ((ProgramTable[idx].VerbNumber == verb) && (ProgramTable[idx].ProgramNumber == prog_temp))
            {
                old2_prog = old1_prog;
                old1_prog = prog;
                prog = prog_temp;
                prog_valid = true;
                found = true;
                printRegister(1, verb);
                printRegister(2, prog);
                printRegister(3, idx);
                //if (noun != old1_noun)
                //{ // a new noun has been entered, but we had a valid noun before, now the old noun is invalid
                //  old2_noun = old1_noun;
                //  old1_noun = noun;
                //  noun = nounNone;
                //}
                prog_0 = -1;
                prog_1 = -1;
                setLamp(blue, lampProg);
            }
            idx++;
        }
        if (prog_valid == false)
        { // we don't have a valid prog, sorry. Turn on the error lamp and clear prog_0 - prog_1 variables
            setLamp(white, lampOprErr);
            prog_error = true;
            //clearprogfunction();
        }
    }
    else if ((prog_valid != true) && (verb_valid == true) && (prog_error !=true))
    { // we have a valid verb, are not in error prog mode and don't have a valid prog yet, lets try and enter one
        setLamp(yellow, lampProg);
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
                    if (prog_valid == false)
                    { // we don't have a valid verb yet, but we have entered a number, lets assing it to the proper place (first or second Digit)
                        if (prog_0 < 0)
                        { // First Verb Digit has been entered
                        prog_0 = temporaryKey;
                        printChar(0, 4, char(prog_0));
                        }
                        else if ((prog_0 >= 0) && (prog_1 < 0))
                        { // Second verb Digit has been entered
                        prog_1 = temporaryKey;
                        printChar(0, 5, char(prog_1));
                        }
                    }
                    break;
                case keyPlus:
                    break;
                case keyMinus:
                    break;
                case keyClear:
                    if (prog_error == true)
                    {
                        clearProgfunction();
                        prog_error = false;
                        setLamp(off, lampOprErr);
                    }
                    clearProgfunction();
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
    else if ((prog_valid != true) && (verb_valid == true) && (prog_error ==true))
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
                    if (prog_error == true)
                    {
                        clearProgfunction();
                        prog_error = false;
                        setLamp(off, lampOprErr);
                    }
                    clearProgfunction();
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
    else if ((prog_valid == true) && (verb_valid == true) && (prog_error ==false))
    {  // we have a valid Program!
        setLamp(blue, lampProg);
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
                        clearProgfunction();
                        prog_error = false;
                        setLamp(off, lampOprErr);
                    }
                    clearProgfunction();
                    break;
                case keyEnter:
                    mode = modeIdle;
                    setLamp(green, lampProg);
                    setLamp(green, lampVerb);
                    old2_verb = old1_verb;
                    old1_verb = verb;
                    verb = verbNone;
                    verb_valid = false;
                    noun_valid = false;
                    printVerb(verb);
                    printNoun(noun);
                    printProg(prog);
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