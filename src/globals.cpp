#include <globals.h>
#include <main.h>



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
    { verbInputProg,        nounNotUsed,        action_none,                  programSetTimeMan             },  /* V37  20E     - Set Time Manually*/ 
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


short NUM_PROG_TABLE_ENTRIES = (sizeof(ProgramTable)/sizeof(ProgramStruct));