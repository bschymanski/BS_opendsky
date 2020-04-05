#ifndef SEVENSEG_H
#define SEVENSEG_H


extern void SevenSegSetup(void);
extern void printRegister(int dregister, long number = 0, bool leadzero = true, bool blink = false, bool alarm = false);
extern void printProg(int prog, bool blink = false);
extern void printVerb(int verb, bool blink = false);
extern void printNoun(int noun, bool blink = false);
extern void printChar(int dregister, int digit, char character,  bool blink = false, bool alarm = false, bool dp = false);
extern void clearRegister(int dregister);
#endif