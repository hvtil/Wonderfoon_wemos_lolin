/*
  Wonderfoon D1Mini 0108
  http://wonderfoon.eu
  WEMOS version
*/

#include "EEPROM.h"
//#include "ESP8266WiFi.h"
# define Start_Byte 0x7E
# define Version_Byte 0xFF
# define Command_Length 0x06
# define End_Byte 0xEF
# define Acknowledge 0x00 //Returns info with command 0x41 [0x01: info, 0x00: no info]
# define ACTIVATED LOW

const bool DEB = true; //Debug
const bool dialDisc = true;  // DialDisk
const int numPadType = 1;

// Variables for Dialdisc Hook and Amplifier

const int hookPin   = 12;     // the in for the telephone hook                 D6 12
const int dialPin   = 14;     //  the in for the telephone dialpulse (yellow)  D5 14
const int pulsePin  = 13;     // the in for the telephone dialbusy (green)      D7 13
const int ampPin    = 15;     // to turn amplifier on / off                   D2 15
const int busyPin   = 16;     // when ready playing                           D0 16

//dialpad config.b
const int numPad[4][3] = {{1, 2, 3}, {4, 5, 6}, {7, 8, 9}, {0, 10, 0}};
const int rowPin[4] = {13, 0, 2, 14};  //  D7 D8 D9 D13
const int colPin[3] = {5, 4, 14 };        //  D3 D4 D5


//const int rowPin[4] = {D7, D8, D9, D13};  //  D7 D8 D9 D13
//const int colPin[3] = {D3, D4, D5};         //  D3 D4 D5

int row[4];   //Array dialpad rows
int col[3];   //Array dialpad cols
int num;      // dialpad number
int lastNum;  //
//dialpad config.e


//boolean isPlaying = false;  // mp3

int countedPulses;          // then number of pulses counted at the end of dialbusy
int pulseCount;             // number of pulses during counting
int audioVolume;            // Audio Volume
int hookState = HIGH;       // the current state of the hook
int dialState = HIGH;       // the current state of the dailbusy
int pulseState = HIGH;      // the current state of the pulse
int lastHookState = HIGH;   // the previous reading from the hook
int lastDialState = HIGH;   // the previous reading from dailbusy
int lastPulseState = HIGH;  // the previous reading from the pulse
bool apmlifierState = false;

int lastDialed[4] = {20, 21, 22, 23};   // last 4 dialed digits for PIN
int folderNumber;                       // current folderNumber
int playMode;                           // current playmode

bool PlayingRandom = false ;             // random playing mode from app or keypad  *

unsigned long lastTime[4];                // time last for digits were dialed to determine pin

// the following variables are unsigned long's because the time, measured in miliseconds,

unsigned long lastHookDebounceTime = 0;   // the last time the hook pin was toggled
unsigned long lastDialDebounceTime = 0;   // the last time the dial pin was
unsigned long lastPulseDebounceTime = 0;  // the last time the pulse pin was
unsigned long hookDebounceDelay = 50;     // the debounce time; increase if the output flickers
unsigned long dialDebounceDelay = 50;     // the debounce time;
unsigned long pulseDebounceDelay = 20;    // the debounce time;
unsigned long waitNextDial = 0;           // wait 1 second for next number  // not in use or depreciated


void setup() {
  delay(1500);
  EEPROM.begin(256);                      // using 0-20 max
  Serial.begin(9600);                             // start serial for debug and mp3
//  EEPROM_clear();
//  AfdelingNaam =  EEPROM_getName();       // read values from eprom before starting Wonderfoon
//  folderNumber = EEPROM_getFolder();
//  audioVolume = EEPROM_getVolume();
//  playMode = EEPROM_getPlayMode();
//  Serial.println("AfdelingNaam = " + AfdelingNaam + " folderNumber = " + folderNumber + " audioVolume = " + audioVolume + " playMode = " + playMode);
  EEPROM_init(0);                          // initialize to check if this is the first time the Wonderfoon is started addess 100 = 77


  folderNumber = EEPROM_getFolder();
  audioVolume = EEPROM_getVolume();
  playMode = EEPROM_getPlayMode();

  //WiFi.forceSleepBegin();
  //WiFi.mode(WIFI_OFF);     // detfault WIFI Off

  //Serial.begin(9600);                             // start serial for debug and mp3

  Serial.println("Booting");                      // 2 debuglines that will always be displayed in logging.
  //Serial.println(" folderNumber = " + folderNumber + " audioVolume = " + audioVolume + " playMode = " + playMode);

  pinMode(hookPin, INPUT_PULLUP);                 //Set pins to input and add pullup resitor
  pinMode(dialPin, INPUT_PULLUP);
  pinMode(pulsePin, INPUT_PULLUP);
  pinMode(ampPin, OUTPUT);                        //Set Aplifier pin to output


  // Setup for Numpad.b
  for (int thisPin = 0; thisPin < (sizeof(rowPin) / sizeof(rowPin[0])); thisPin++) {  // Set pins to input and add pullup resitor
    pinMode(rowPin[thisPin], INPUT_PULLUP);
  }
  //
  for (int thisPin = 0; thisPin < (sizeof(colPin) / sizeof(colPin[0])) ; thisPin++) { // Set pins to input and add pullup resitor
    pinMode(colPin[thisPin], INPUT_PULLUP);
  }

  // Setup for Numpad.b

  mp3Wake();
  amplifier(1);                                           // switch amplifier on to read boot feedback
  setMP3Volume(audioVolume);                              // set volume to current volume
  delay(1000);// set volume to read value ; 00 - 30
  playTrackInFolder(10, 4);                               // Wonderfoon has started
  delay(2000);

  //put mp3 in sleep mode to save battery
  debug("Setup ready");
  debug("Start");
  debug("");
  playVolume();                                           // play vulume status
  playWillekeurig(playMode);                              // play random status
  playFolder(folderNumber);                               // play folder number status
  //switchWebServer(false,false);                                 //switch webserver to default setting, this will also play wifi status
  mp3Sleep();                                             // set mp3 to battry save mode
  amplifier(0);                                           //Aplifier off to battry save mode
  // switchWebServer(true);                               //test always on uncomment to start with wifi on.

  Serial.println("");
  Serial.println("Ready....");
}

void loop() {

  waitForHook();                                          // wait for someone to pick up the hook
}

void waitForHook() {
  // read the state hook switch into a local variable:
  int hookReading = digitalRead(hookPin);


  // check to see if you just pressed/released the hook
  // (i.e. the input went from HIGH to LOW),  and you've waited
  // long enough since the last press to ignore any noise:

  // If the hook changed, due to noise or pressing:
  if (hookReading != lastHookState) {
    // reset the debouncing timer
    lastHookDebounceTime = millis();
  }

  if ((millis() - lastHookDebounceTime) > hookDebounceDelay) {
    // whatever the reading is at, it's been there for longer
    // than the debounce delay, so take it as the actual current state:

    // if the hook state has changed:
    if (hookReading != hookState) {             // (one time action)

      hookState = hookReading;                  // put the value read to the current hookstate.
      if (hookState == LOW) {                   // check state is LOW
        debug("The hook is picked up");         // only when hookstate changes and goes to low
        mp3Wake();                              // Wake up MP3 since hook is picked up and set player to SD
        amplifier(1);                           // switch on the amplifier
        playTrackInFolder(99, 4);               // Play dailtone
        setMP3Volume(audioVolume);
      }

      else {                                    // if not low but high (still one time action)
        debug("The hook is down on the phone");
        countedPulses = 0;                     //reset counting parameters
        pulseCount = 0;
        MP3stop();                             // Stop MP3Player
        mp3Sleep();                            // Put MP3 is sleep mode since hook is down
        amplifier(0);                          // amplfier of  CHECK THIS
       
      }
      //digitalWrite(ledPin, ledState);
      debug("");
    }
  }
  
  if (hookState == LOW) {                       // if Hook is LOW waitforDial or checkNumPad dependig on the version

    if (dialDisc)
    {

      waitForDial();  // every loop we check if hookstate is low and wait for dial.
    }
    else {
      checkNumPad();
    }
    checkPlaying();
  }

  if (hookState == HIGH) {

  }
  lastHookState = hookReading;    //store current status in lastHookState for bounceTime check

}
