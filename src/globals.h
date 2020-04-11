#ifndef GLOBALS_H
#define GLOBALS_H


typedef struct PROGRAM_STRUCT
{
	short VerbNumber;
	short NounNumber;
    short action;
	short ProgramNumber;
}ProgramStruct;

extern ProgramStruct ProgramTable[];

extern short NUM_PROG_TABLE_ENTRIES;

extern bool blink;
extern bool toggledprogblink;
extern bool blinkprog;

extern bool toggled;
extern bool toggled500;
extern bool toggled250;
extern bool toggledverbblink;

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