#include <LedControl.h>
#include <SevenSeg.h>
#include <main.h>

// include main.h for global definitions

//initialize the Sevensegment function with the LedControll Library
LedControl SevenSeg = LedControl(12,10,11,4);

// Setup of the Sevensegment LED
void SevenSegSetup(void)
{
     for (int index = 0; index < 4; index++) {
        SevenSeg.shutdown(index,false);
        SevenSeg.setIntensity(index, 8);
        SevenSeg.clearDisplay(index);
    }
}

void clearRegister(int dregister)
{
    SevenSeg.clearDisplay(dregister);
}

void printRegister(int dregister, long number, bool leadzero, bool blink, bool alarm)
{   // Print the Register 1, 2, or 3, the number, if you want leading zeros if you want to blink it, check if it is an alarm
    // Setdigit: Register 0 - 3, plus sign 0, 1-5 numbers
    //num4 = (fm_station / 10) % 10;
    //num3 = (fm_station / 100) % 10;
    //num2 = (fm_station / 1000) % 10;
    //num1 = (fm_station / 10000) % 10;
    int one = 0;
    int ten = 0;
    int hundred = 0;
    long thousand = 0;
    long tenthousand = 0;
    // first, check if the number is positive or negative and set the plus or minus sign
    if (number < 0)
    {
        number = -number;
        // Set the minus sign 
        SevenSeg.setRow(dregister, 0, B00100100);
    }
    else 
    {
        // Set the plus sign
        SevenSeg.setRow(dregister, 0, B01110100);
    }
    // now seperate the number
    if (number == 0)
    {
        one = int(number);
    }
    else if ((number > 0) && (number < 10))
    {
        one = int(number);
    }
    else if ((number >= 10) && (number < 100))
    {   
        one = number % 10;
        ten = (number - one) / 10;
    }
    else if ((number >= 100) && (number < 1000))
    {
        one = number % 10;
        ten = (number / 10) % 10;
        hundred = (number / 100) % 10;

    }
    else if ((number >= 1000) && (number < 10000))
    {
        one = number % 10;
        ten = (number / 10) % 10;
        hundred = (number / 100) % 10;
        thousand = (number / 1000) % 10;
    }
    else if ((number >= 10000) && (number < 100000))
    {
        one = number % 10;
        ten = (number / 10) % 10;
        hundred = (number / 100) % 10;
        thousand = (number / 1000) % 10;
        tenthousand = (number / 10000) % 10;
    }
    // show the number
    if (blink == false)
    {
        if (number >= 100000)
        {
            SevenSeg.setRow(dregister,0,B00000000);
            SevenSeg.setChar(dregister,1,' ',false);
            SevenSeg.setChar(dregister,2,'1', false);
            SevenSeg.setChar(dregister,3,'3', false);
            SevenSeg.setChar(dregister,4,'0', false);
            SevenSeg.setChar(dregister,5,'5', false);
        }
        else 
        {
            SevenSeg.setDigit(dregister, 5, one, false);
            SevenSeg.setDigit(dregister, 4, ten, false);
            SevenSeg.setDigit(dregister, 3, hundred, false);
            SevenSeg.setDigit(dregister, 2, thousand, false);
            SevenSeg.setDigit(dregister, 1, tenthousand, false);
        }
    }
    if (blink == true)
    {
        SevenSeg.clearDisplay(dregister);
    }
}

void printProg(int prog, bool blink)
{  // Print the Progam PROG
    int one = 0;
    int ten = 0;
    if (blink == false)
    {
        if (prog == 0)
        {
            SevenSeg.setRow(0,2,B00000000);
            SevenSeg.setRow(0,3,B00000000);
        }
        else if ((prog > 0) && (prog < 10))
        {
            SevenSeg.setDigit(0, 2, 0, false);
            SevenSeg.setDigit(0, 3, prog, false);
        }
        else if (prog >= 10)
        {   
            one = prog % 10;
            ten = (prog - one) / 10;
            SevenSeg.setDigit(0, 2, ten, false);
            SevenSeg.setDigit(0, 3, one, false);
        }
    }
    else if (blink == true)
    {
        SevenSeg.setRow(0,2,B00000000);
        SevenSeg.setRow(0,3,B00000000);
    }
}

void printVerb(int verb, bool blink)
{  // Print the verb VERB
    int one = 0;
    int ten = 0;
    if (blink == false)
    {
        if (verb == verbNone)
        {
            SevenSeg.setRow(0,0,B00000000);
            SevenSeg.setRow(0,1,B00000000);
        }
        else if ((verb > 0) && (verb < 10))
        {
            SevenSeg.setDigit(0, 0, 0, false);
            SevenSeg.setDigit(0, 1, verb, false);
        }
        else if (verb >= 10)
        {   
            one = verb % 10;
            ten = (verb - one) / 10;
            SevenSeg.setDigit(0, 0, ten, false);
            SevenSeg.setDigit(0, 1, one, false);
        }
    }
    else if (blink == true)
    {
        SevenSeg.setRow(0,0,B00000000);
        SevenSeg.setRow(0,1,B00000000);
    }
}

void printNoun(int noun, bool blink)
{  // Print the noun NOUN
    int one = 0;
    int ten = 0;
    if (blink == false)
    {
        if (noun == nounNone)
        {
            SevenSeg.setRow(0,4,B00000000);
            SevenSeg.setRow(0,5,B00000000);
        }
        else if ((noun > 0) && (noun < 10))
        {
            SevenSeg.setDigit(0, 4, 0, false);
            SevenSeg.setDigit(0, 5, noun, false);
        }
        else if (noun >= 10)
        {   
            one = noun % 10;
            ten = (noun - one) / 10;
            SevenSeg.setDigit(0, 4, ten, false);
            SevenSeg.setDigit(0, 5, one, false);
        }
    }
    else if (blink == true)
    {
        SevenSeg.setRow(0,4,B00000000);
        SevenSeg.setRow(0,5,B00000000);
    }
}

void printChar(int dregister, int digit, char character,  bool blink, bool alarm, bool dp)
{
    /* 
    * Display a character on a 7-Segment display.
    * There are only a few characters that make sense here :
    * '0','1','2','3','4','5','6','7','8','9','0',
    * 'A','b','c','d','E','F','H','L','P',
    * '.','-','_',' ' 
    * Params:
    * addr	  address of the display
    * digit  the position of the character on the display (0..7)
    * value  the character to be displayed. 
    * dp     sets the decimal point.
    */
    SevenSeg.setChar(dregister, digit, character, dp);
}