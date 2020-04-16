#include <audio_functions.h>
#include <Arduino.h>
#include <SoftwareSerial.h>
#include <DFRobotDFPlayerMini.h>
#include <SevenSeg.h>
#include <neopixel.h>
#include <globals.h>
#include <main.h>
#include <keyboard.h>
#include <functions.h>

SoftwareSerial mySoftwareSerial(5, 4); // RX, TX
DFRobotDFPlayerMini myDFPlayer;

int clipcount = 0;
int clipnum = 1;

short audiotempkey = 0;
short audiopressedkey = 0;

void audio_setup()
{ // DFPlayerMini initialize
    Serial.begin(9600);
    mySoftwareSerial.begin(9600);
    Serial.println();
    Serial.println(F("DFRobot DFPlayer Mini Demo"));
    Serial.println(F("Initializing DFPlayer ... (May take 3~5 seconds)"));
    if (!myDFPlayer.begin(mySoftwareSerial))
    {  //Use softwareSerial to communicate with mp3.
        Serial.println(F("Unable to begin:"));
        Serial.println(F("1.Please recheck the connection!"));
        Serial.println(F("2.Please insert the SD card!"));
    while(true);
    }
    Serial.println(F("DFPlayer Mini online."));
    myDFPlayer.volume(20);  //Set volume value. From 0 to 30
    clipcount = myDFPlayer.readFileCounts();
    Serial.println(clipcount); //read all file counts in SD card
}

void playTrack(uint8_t track)
{
  myDFPlayer.stop();
  delay(200);
  myDFPlayer.play(track);
  delay(200);
  int file = myDFPlayer.readCurrentFileNumber();
  while (file != track) {
    myDFPlayer.play(track);
    delay(200);
    file = myDFPlayer.readCurrentFileNumber();
  }
}

void actionPlaySelectedAudioclip(int clipnum)
{   // V16 N98 play the selected Audio Clip
    //printVerb(verb);
    //printNoun(noun);
    //playTrack(clipnum);
    setLamp(orange, lampTemp);
    myDFPlayer.play(clipnum);
    setLamp(off, lampTemp);
    //clearVerbfunction();
    //clearNounfunction();
}
// V21 N98 read & enter & play the selected Audio Clip
void actionSelectAudioclip(short transmittedkey)
{   // V21 N98 read & enter & play the selected Audio Clip
    // first print initial clipnum = 1
    audiotempkey = transmittedkey;
    if (audiotempkey != audiopressedkey)
    {
      switch (audiotempkey)
      {
        case keyPlus:
        {
          clipnum++;
          if (clipnum > clipcount)
          {
            clipnum = 1;
          }
          break;
        }
        case keyMinus:
        {
          clipnum--;
          if (clipnum <= 0)
          {
            clipnum = clipcount;
          }
          break;
        }
        case keyEnter:
        {
          actionPlaySelectedAudioclip(clipnum);
          break; 
        }
        case keyProceed:
        {
          actionPlaySelectedAudioclip(clipnum);
          break; 
        }
      }
    }
    audiopressedkey = audiotempkey;
    if (toggle500 == true)
    {
      printRegister(1,clipnum);
      printRegister(2,audiopressedkey);
      printRegister(3,audiotempkey);
    }
    else if (toggle500 == false)
    {
      clearRegister(1);
    }
    //printRegister(2,audiotempkey);
    //printRegister(3,audiopressedkey);
    
}