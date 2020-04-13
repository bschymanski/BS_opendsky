#ifndef KEYBOARD_H
#define KEYBOARD_H


const short     keyNone                 = 20;
const short     keyVerb                 = 10;
const short     keyNoun                 = 11;
const short     keyPlus                 = 12;
const short     keyMinus                = 13;
const short     keyNumber0              = 0;
const short     keyNumber1              = 1;
const short     keyNumber2              = 2;
const short     keyNumber3              = 3;
const short     keyNumber4              = 4;
const short     keyNumber5              = 5;
const short     keyNumber6              = 6;
const short     keyNumber7              = 7;
const short     keyNumber8              = 8;
const short     keyNumber9              = 9;
const short     keyClear                = 18;
const short     keyProceed              = 14;
const short     keyRelease              = 16;
const short     keyEnter                = 15;
const short     keyReset                = 17;

extern short temporaryKey;
extern short pressedKey;

extern void keyboardsetup();
extern int readKeyboard();
#endif