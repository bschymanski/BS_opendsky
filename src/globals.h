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
// ProgramStruct ProgramTable[];
#endif