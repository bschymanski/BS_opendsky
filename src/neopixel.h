#ifndef NEOPIXEL_H
#define NEOPIXEL_H

// give all the Neopixel Lamps a Name
const short     lampNoun            = 0;
const short     lampProg            = 1;
const short     lampVerb            = 2;
const short     lampCompActy        = 3;
const short     lampTemp            = 4;
const short     lampGimbalLock      = 5;
const short     lampProgCond        = 6;
const short     lampRestart         = 7;
const short     lampTracker         = 8;
const short     lampAlt             = 9;
const short     lampVel             = 10;
const short     lampClk             = 11;
const short     lampPosition        = 12;
const short     lampOprErr          = 13;
const short     lampKeyRelease      = 14;
const short     lampSTBY            = 15;
const short     lampNoAtt           = 16;
const short     lampUplinkActy      = 17;

// now define Colors
const short     green               = 1;
const short     white               = 2;
const short     yellow              = 3;
const short     orange              = 4;
const short     blue                = 5;
const short     red                 = 6;
const short     off                 = 7;

// this is the function that this little library provides
// provide the color and the lamp
extern void setLamp(int color, int lampNumber);
extern void NeoPixelSetup(void);
#endif