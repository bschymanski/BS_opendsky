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

// ProgramStruct ProgramTable[];
#endif