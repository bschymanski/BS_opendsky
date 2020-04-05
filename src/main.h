#ifndef MAIN_H
#define MAIN_H

enum Action: int {
    none                        = 0,
    displayIMUAttitude          = 1,
    displayRealTimeClock        = 2,
    displayGPS                  = 3,
    displayRangeWith1202Error   = 4,
    setTime                     = 5,
    setDate                     = 6,
    PlayAudioclip               = 7,
    PlaySelectedAudioclip       = 8,
    displayIMUGyro              = 9
};

enum Mode: int {
    modeIdle                = 0,
    modeInputVerb           = 1,
    modeInputNoun           = 2,
    modeInputProgram        = 3,
    modeLampTest            = 4
};

enum programNumber: int {
    programNone             = 0,
    programJFKAudio         = 62,
    programApollo11Audio    = 69,
    programApollo13Audio    = 70
};

enum verbValues: int
{ // Verbs 0,35,16,21
    verbNone                = 0,
    verbLampTest            = 35,
    verbDisplayDecimal      = 16,
    verbSetComponent        = 21
};

enum nounValues: int 
{ // Nouns 0,17,36,37,43,68,98
    nounNone                = 0,
    nounIMUAttitude         = 17,
    nounIMUgyro             = 18,
    nounClockTime           = 36,
    nounDate                = 37,
    nounLatLongAltitude     = 43,
    nounRangeTgoVelocity    = 68,
    nounSelectAudioclip      = 98
};

// some toggle definitions for blinking stuff
bool toggle = false;
bool toggle500 = false;
bool toggle250 = false;

bool toggled = false;
bool toggled500 = false;
bool toggled250 = false;

bool blink = false;
bool blinkverb = true;
bool blinknoun = true;
bool blinkprog = true;
bool imutoggle = true;
bool printregtoggle = true;
bool uplink_compact_toggle = true;

#endif