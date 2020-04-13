#include <globals.h>
#include <main.h>
#include <time_functions.h>



/* This structure contains all of the programs in the system. The noun number is
optional. The program function is called when the verb and noun are entered. if
the program should be called repeatedly, then you will need to setup a thread
and callback function inside the program function. The set data function is
used to give numerial data to the function. The Get Disp Data Function Should
return display data for the 7 segment display driver. Run State is used internally
by the Program module to track the state of all of the current programs.*/
ProgramStruct ProgramTable[] =
{
	/*VerbNumber            NounNumber          action                        ProgramNumber                     Description */
	{ verbLampTest,         nounNotUsed,        action_LampTest,              programNotUsed                },  /* V35E N--     - Bulb test */
	{ verbDisplayDecimal,   nounIMUAttitude,    action_displayIMUAttitude,    programNotUsed                },  /* V16E  N17 E  - IMUAttitude */
    { verbDisplayDecimal,   nounClockTime,      action_displayRealTimeClock,  programNotUsed                },  /* V16E  N36 E  - Display Time */
    { verbDisplayDecimal,   nounClockTime,      action_displayRealTimeClock,  programNotUsed                },  /* V16E  N36 E  - Display Time */
    { verbDisplayDecimal,   nounClockTime,      action_displayRealTimeClock,  programNotUsed                },  /* V16E  N36 E  - Display Time */
    { verbDisplayDecimal,   nounClockTime,      action_displayRealTimeClock,  programNotUsed                },  /* V16E  N36 E  - Display Time */
    { verbDisplayDecimal,   nounClockTime,      action_displayRealTimeClock,  programNotUsed                },  /* V16E  N36 E  - Display Time */
    { verbDisplayDecimal,   nounClockTime,      action_displayRealTimeClock,  programNotUsed                },  /* V16E  N36 E  - Display Time */
    { verbInputProg,        nounNotUsed,        action_none,                  programDispTimeDate           },  /* V37  20E     - Display Date / Month / Time : progDispTimeDate()*/ 
    { verbInputProg,        nounNotUsed,        action_none,                  programSetDateMan             },  /* V37  21E*/
    { verbInputProg,        nounNotUsed,        action_none,                  programSetTimeGPS             },  /* V37  22E*/
    { verbInputProg,        nounNotUsed,        action_none,                  programSetDateGPS             },  /* V37  23E*/
    { verbInputProg,        nounNotUsed,        action_none,                  programSetDebugEEPROM         },  /* V37  24E*/
    { verbInputProg,        nounNotUsed,        action_none,                  programSetColormodeEEPROM     },  /* V37  24E*/
    { verbInputProg,        nounNotUsed,        action_none,                  programJFKAudio               },  /* V37  62E*/
    { verbInputProg,        nounNotUsed,        action_none,                  programApollo11Audio          },  /* V37  69E*/
    { verbInputProg,        nounNotUsed,        action_none,                  programApollo13Audio          },  /* V37  70E*/
    { verbDisplayDecimal,   nounIMUgyro,        action_displayIMUGyro,        programNotUsed                }   /* V16E  N18 E - Display IMUGyro */
};

// this function is in globals because it refers to the Programtable
short runAction(short action)
{
    switch (action)
    {
        case action_displayRealTimeClock:
        {
        actionReadTime();
        break;
        }
    }
    return(0);
}

short runProgram(short prog)
{
    switch (prog)
    {
        case programDispTimeDate:
        {
            progDispTimeDate();
            break;
        }
    }
    return(0);
}

// initialisation of Mode-, Verb, Noun, Program and Action Modes
// current Mode-, Verb, Noun, Program and Action Values
short mode    = modeIdle;     // eq 0
short verb    = verbNone;     // eq 0
short noun    = verbNone;     // eq 0
short prog    = programNone;  // eq 0
short action  = action_none;  // eq 0

bool actionrunning = false;
bool programrunning = false;

// old Mode-, Verb, Noun, Program and Action Values
short old1_mode    = modeIdle;     // eq 0
short old1_verb    = verbNone;     // eq 0
short old1_noun    = verbNone;     // eq 0
short old1_prog    = programNone;  // eq 0
short old1_action  = action_none;  // eq 0

// before old Mode-, Verb, Noun, Program and Action values
short old2_mode    = modeIdle;     // eq 0
short old2_verb    = verbNone;     // eq 0
short old2_noun    = verbNone;     // eq 0
short old2_prog    = programNone;  // eq 0
short old2_action  = action_none;  // eq 0

bool verb_valid = false;
bool noun_valid = false;
bool prog_valid = false;

short verb_0 = -1;
short verb_1 = -1;

short noun_0 = -1;
short noun_1 = -1;

short prog_0 = -1;
short prog_1 = -1;

short verb_temp = -1;
short noun_temp = -1;
short prog_temp = -1;

bool verb_error = false;
bool noun_error = false;
bool prog_error = false;

short NUM_PROG_TABLE_ENTRIES = (sizeof(ProgramTable)/sizeof(ProgramStruct));

bool blink = false;
bool toggledprogblink = false;
bool blinkprog = false;
bool toggled = false;
bool toggled500 = false;
bool toggled250 = false;
bool toggledverbblink = false;

bool blinkverb = true;
bool blinknoun = true;
bool imutoggle = true;
bool printregtoggle = true;
bool uplink_compact_toggle = true;

bool global_state_1sec = true;
bool global_state_500 = true;
bool global_state_250 = true;
bool toggle500 = true;
bool toggle250 = true;
bool toggle = true;
bool printed = true;

bool executeAction = false;
bool lookupAction = false;
bool executeProgram = false;
bool lookupProgram = false;