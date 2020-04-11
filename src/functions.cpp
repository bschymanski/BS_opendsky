#include <Arduino.h>
#include <functions.h>
#include <main.h>
#include <neopixel.h>
#include <SevenSeg.h>
#include <globals.h>

// 1sec toogle
bool toggle_timer(void *)
{
  if(global_state_1sec==false){
    global_state_1sec=true;
    toggle = true;
  }
  else
  {
    global_state_1sec=false;
    toggle = false;
  }
  return true; // repeat? true
}

// 500msec toggle
bool toggle_timer_500(void *)
{
  if(global_state_500==false){
    global_state_500=true;
    toggle500 = true;
  }
  else
  {
    global_state_500=false;
    toggle500 = false;
  }
  return true; // repeat? true
}

bool toggle_timer_250(void *)
{
  if(global_state_250==false){
    global_state_250=true;
    toggle250 = true;
  }
  else
  {
    global_state_250=false;
    toggle250 = false;
  }
  return true; // repeat? true
}

void keyResetfunction()
{  // for now the Reset Button resets the mode to Idlemode, clears all relevant variables for a clean startover
  mode = modeIdle;
  setLamp(green, lampVerb);
  setLamp(green, lampNoun);
  setLamp(green, lampProg);
  setLamp(off, lampProgCond);
  setLamp(off, lampUplinkActy);
  noun = 0;
  verb = 0;
  prog = 0;
  action = action_none;
  verb_valid = false;
  noun_valid = false;
  verb_error = false;
  executeProgram = false;
  executeAction = false;
  lookupAction = false;
  lookupProgram = false;
  setLamp(off, lampOprErr);
  clearRegister(1);
  clearRegister(2);
  clearRegister(3);
  }

void clearVerbfunction()
{
  verb_valid = false;
  noun_valid = false;
  verb = 0;
  noun = 0;
  verb_0 = -1;
  verb_1 = -1;
  noun_0 = -1;
  noun_1 = -1;
  executeProgram = false;
  executeAction = false;
  lookupAction = false;
  lookupProgram = false;
  verb_error = false;
  noun_error = false;
  printChar(0, 0, ' ');
  printChar(0, 1, ' ');
  printChar(0, 4, ' ');
  printChar(0, 5, ' ');
}

void clearNounfunction()
{
  noun_valid = false;
  noun = 0;
  noun_0 = -1;
  noun_1 = -1;
  executeAction = false;
  printChar(0, 4, ' ');
  printChar(0, 5, ' ');
}

void clearProgfunction()
{
  prog_valid = false;
  prog = 0;
  prog_0 = -1;
  prog_1 = -1;
  executeProgram = false;
  printChar(0, 2, ' ');
  printChar(0, 3, ' ');
}