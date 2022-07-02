#include <TM1637Display.h>
#include "Arduino.h"
#include "SoftwareSerial.h"
#include "DFRobotDFPlayerMini.h"
#define CLK 4
#define DIO 5
#define numberofSeconds(_time_) (_time_ % 60)
#define numberofMinutes(_time_) ((_time_ / 60) % 60)
SoftwareSerial mySoftwareSerial(10, 11); // RX, TX
DFRobotDFPlayerMini myDFPlayer;
void printDetail(uint8_t type, int value);
int m = 1;
static int state = 0;
const uint8_t OFF[] = {0, 0, 0, 0};
const uint8_t SEG_DONE[] = {
  SEG_B | SEG_C | SEG_D | SEG_E | SEG_G,           // d
  SEG_A | SEG_B | SEG_C | SEG_D | SEG_E | SEG_F,   // O
  SEG_C | SEG_E | SEG_G,                           // n
  SEG_A | SEG_D | SEG_E | SEG_F | SEG_G            // E
  };
TM1637Display display(CLK, DIO);
boolean buttom = false;
unsigned long timeLimit25 = 1500000;
unsigned long timeLimit5 = 300000;


void countdown5(){    //倒數五分鐘
  unsigned long timeLimit5 = 300;
  int passtime5 = 0;
  while(passtime5 <= 300){
    unsigned long timeRemaining5 = timeLimit5 - passtime5;
    timeRemaining5 = timeLimit5 - passtime5;
    uint8_t segto;
    segto = 0x80; 
    int seconds = numberofSeconds(timeRemaining5);
    int minutes = numberofMinutes(timeRemaining5);  
    Serial.println(seconds);
    Serial.println(minutes);
    display.setSegments(&segto, 1, 1);
    display.showNumberDecEx(minutes, 0, true, 2, 0);
    display.showNumberDecEx(seconds, 0x80>>1, true, 2, 2);
    passtime5++;
    delay(1000);
  }
  
}
void countdown25(){    //倒數二十五分鐘
  unsigned long timeLimit25 = 10;
  int passtime25 = 0;
  while(passtime25 <= 10){
    unsigned long timeRemaining25 = timeLimit25 - passtime25;
    timeRemaining25 = timeLimit25 - passtime25;
    uint8_t segto;
    segto = 0x80; 
    int seconds = numberofSeconds(timeRemaining25);
    int minutes = numberofMinutes(timeRemaining25);  
    Serial.println(seconds);
    Serial.println(minutes);
    display.setSegments(&segto, 1, 1);
    display.showNumberDecEx(minutes, 0, true, 2, 0);
    display.showNumberDecEx(seconds, 0x80>>1, true, 2, 2);
    passtime25++;
    delay(1000);
  }
}
void setup(){
  mySoftwareSerial.begin(9600);
  Serial.begin(115200);
  display.setBrightness(0x0b);
  // Clear the display
  display.setSegments(OFF);
  pinMode(9, OUTPUT);
  digitalWrite(8, HIGH);
  pinMode(8, INPUT_PULLUP);
  delay(1000);
  Serial.println();
  Serial.println(F("DFRobot DFPlayer Mini Demo"));
  Serial.println(F("Initializing DFPlayer ... (May take 3~5 seconds)"));
  
  if (!myDFPlayer.begin(mySoftwareSerial, false)) {  //Use softwareSerial to communicate with mp3.
    Serial.println(F("Unable to begin:"));
    Serial.println(F("1.Please recheck the connection!"));
    Serial.println(F("2.Please insert the SD card!"));
    while(true){
      delay(0); // Code to compatible with ESP8266 watch dog.
    }
  }
  Serial.println(F("DFPlayer Mini online."));
  
  myDFPlayer.volume(18);  //Set volume value. From 0 to 30
  
}
void loop(){
   switch(state){
    case 0:
      display.setSegments(OFF);
      Serial.println(state);
      delay(1000);
      if(digitalRead(8) != LOW) {
        state++;
      }
      Serial.println(state);
      break;
    case 1:
      countdown25();
      if(digitalRead(8) == LOW) {
        state++;
      }
      Serial.println(state);
      break;
    case 2:
      if(digitalRead(8) != LOW) {
        state++;
        myDFPlayer.pause();
      }
      else {
        display.setSegments(SEG_DONE);
        myDFPlayer.play(1);  //Play the first mp3
        delay(8000);
        Serial.println(state);
      }
      break;
    case 3:
      countdown5();
      if(digitalRead(8) == LOW) {
        state++;
      }
      Serial.println(state);
      break;      
    case 4:
      if(digitalRead(8) != LOW) {
        state++;
        myDFPlayer.pause();
      }
      else {
        display.setSegments(SEG_DONE);
        myDFPlayer.play(1);  //Play the first mp3
        delay(8000);
        Serial.println(state);
      }
      break;
    case 5:
      if(digitalRead(8) == LOW) {
        state = 0;
      }
      Serial.println(state);
      break;
   }
}
