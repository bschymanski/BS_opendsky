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

// Loop Modes
const short     modeIdle                    = 0;
const short     modeInputVerb               = 1;
const short     modeInputNoun               = 2;
const short     modeInputProgram            = 3;
const short     modeInputNumber             = 4;
const short     modeLampTest                = 5;

// possible Programs
const short     programNone                 = 0;
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
const short     nounLatLongAltitude         = 43;
const short     nounRangeTgoVelocity        = 68;
const short     nounSelectAudioclip         = 98;
const short     nounNotUsed         		= -1;

extern bool toggle;


extern bool global_state_1sec;
extern bool global_state_500;
extern bool global_state_250;


extern bool toggle500;
extern bool toggle250;




extern short verb_0;
extern short verb_1;

extern short noun_0;
extern short noun_1;

extern short prog_0;
extern short prog_1;

extern short mode;
extern short verb;
extern short noun;
extern short prog;
extern short action;

// old Mode-, Verb, Noun, Program and Action Values
extern short old1_mode;
extern short old1_verb;
extern short old1_noun;
extern short old1_prog;
extern short old1_action;

// before old Mode-, Verb, Noun, Program and Action values
extern short old2_mode;
extern short old2_verb;
extern short old2_noun;
extern short old2_prog;
extern short old2_action;

extern bool verb_valid;
extern bool noun_valid;
extern bool prog_valid;

extern short temporaryKey;
extern short pressedKey;

extern short verb_temp;
extern short noun_temp;
extern short prog_temp;

extern bool verb_error;
extern bool noun_error;
extern bool prog_error;

#endif