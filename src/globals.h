#ifndef GLOBALS_H
#define GLOBALS_H

extern bool debug;
extern bool colormode;

typedef struct PROGRAM_STRUCT
{
	short VerbNumber;
	short NounNumber;
    short action;
	short ProgramNumber;
}ProgramStruct;

extern short runAction(short action);
extern short runProgram(short prog);

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

extern bool actionrunning;
extern bool programrunning;

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

extern ProgramStruct ProgramTable[];

extern short NUM_PROG_TABLE_ENTRIES;

extern bool blink;
extern bool toggledprogblink;
extern bool blinkprog;
extern bool blinkgpstime;
extern bool blinkgpstimetoggled;
extern bool toggled;
extern bool toggled500;
extern bool toggled250;
extern bool toggledverbblink;
extern bool gpsreadtoggle;
extern short gps_hour;
extern short gps_minute;
extern short gps_second;

extern bool blinkverb;
extern bool blinknoun;
extern bool imutoggle;
extern bool printregtoggle;
extern bool uplink_compact_toggle;

extern bool global_state_1sec;
extern bool global_state_500;
extern bool global_state_250;
extern bool toggle500;
extern bool toggle250;
extern bool toggle;
extern bool printed;
extern bool executeAction;
extern bool lookupAction;
extern bool executeProgram;
extern bool lookupProgram;

#endif