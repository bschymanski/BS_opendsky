#ifndef MAIN_H
#define MAIN_H

// actions to perform when Verb / Noun is correct
const short     action_none                         = 0;
const short     action_displayIMUAttitude           = 1;
const short     action_displayRealTimeClock         = 2;
const short     action_displayGPS                   = 3;
const short     action_displayRangeWith1202Error    = 4;
const short     action_setTime                      = 5;
const short     action_setDate                      = 6;
const short     action_PlayAudioclip                = 7;
const short     action_PlaySelectedAudioclip        = 8;
const short     action_displayIMUGyro               = 9;
const short     action_LampTest                     = 10;
const short     action_displayGPSTime               = 11;
const short     action_setGPSTime                   = 12;

// Loop Modes
const short     modeIdle                    = 0;
const short     modeInputVerb               = 1;
const short     modeInputNoun               = 2;
const short     modeInputProgram            = 3;
const short     modeInputNumber             = 4;
const short     modeLampTest                = 5;
const short     modeExcuteAction            = 6;
const short     modeExcuteProgram           = 7;
const short     modeIncDecNumber            = 8; // increase or decrease a number

// possible Programs
const short     programNone                 = 0;
const short     programDispTimeDate         = 20;
const short     programSetDateMan           = 21;
const short     programSetTimeGPS           = 22;
const short     programSetDateGPS           = 23;
const short     programSetDebugEEPROM       = 24;
const short     programSetColormodeEEPROM   = 25;
const short     programJFKAudio             = 62;
const short     programApollo11Audio        = 69;
const short     programApollo13Audio        = 70;
const short     programNotUsed				= -1;
// valid Verbs

const short     verbNone                    = 0;
const short     verbLampTest                = 35;
const short     verbInputProg               = 37;
const short     verbDisplayDecimal          = 16;
const short     verbInputNumber             = 21;

// valid Nouns
const short     nounNone                    = 0;
const short     nounIMUAttitude             = 17;
const short     nounIMUgyro                 = 18;
const short     nounClockTime               = 36;
const short     nounDate                    = 37;
const short     nounGPSTime                 = 38;
const short     nounLatLongAltitude         = 43;
const short     nounRangeTgoVelocity        = 68;
const short     nounSelectAudioclip         = 98;
const short     nounNotUsed         		= -1;



#endif