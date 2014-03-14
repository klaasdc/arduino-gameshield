/*
  An example that uses all the in/outputs of the Arduino Game Shield
  
  
  Written by Klaas De Craemer, klaasdc@gmail.com
 */


/* -------- Display related -------- */
#include <Adafruit_GFX.h>
#include <Adafruit_PCD8544.h>
// pin 8 - Serial clock out (SCLK)
// pin 7 - Serial data out (DIN)
// pin 6 - Data/Command select (D/C)
// pin 9, LCD reset (RST) (optional, can be connected to Arduino reset to save a pin)
Adafruit_PCD8544 display = Adafruit_PCD8544(6, 7, 8, 9);

/* -------- Music related -------- */
#define  speakerPin  10
#include "pitches.h"

// notes in the melody:
int melody[] = {
  NOTE_C4, NOTE_G3,NOTE_G3, NOTE_A3, NOTE_G3,0, NOTE_B3, NOTE_C4};
// note durations: 4 = quarter note, 8 = eighth note, etc.:
int noteDurations[] = {
  4, 8, 8, 4,4,4,4,4 };

/* -------- Button related ------- */
int buttonStates = 0;
#define  BUTTON0  0b00000001
#define  BUTTON1  0b00000010
#define  BUTTON2  0b00000100
#define  BUTTON3  0b00001000
#define  BUTTON4  0b00010000
#define  BUTTON5  0b00100000
#define  BUTTON6  0b01000000
#define  BUTTON7  0b10000000

#define  addressPinA    4
#define  addressPinB    3
#define  addressPinC    2
#define  buttonInputPin    5
#define  nbOfButtons  8

/* ---------------------------------- */
void setup(){
  /* Initialise button inputs */
  pinMode(addressPinA, OUTPUT);
  pinMode(addressPinB, OUTPUT);
  pinMode(addressPinC, OUTPUT);
  pinMode(buttonInputPin, INPUT);

  /* Initialise display */
  display.begin();
  display.setContrast(60);
  delay(2000);
  display.clearDisplay();// clears the screen and buffer

  /* Show some text */
  display.setTextSize(1);
  display.setTextColor(BLACK);
  display.setCursor(0,0);
  display.println("Hello, world!");
  display.display();

  playSong();

  display.clearDisplay();// clears the screen and buffer
  display.setCursor(0,0);
  display.println("Press a button");
  display.display();
}

void loop(){
  readButtons();
  display.clearDisplay();
  display.setCursor(0,0);
  display.println("Buttons\n pressed:");

  if (buttonStates > 0){
    if (buttonStates & BUTTON0){
      display.println("0");
    } 
    if (buttonStates & BUTTON1){
      display.println("1");
    } 
    if (buttonStates & BUTTON2){
      display.println("2");
    } 
    if (buttonStates & BUTTON3){
      display.println("3");
    } 
    if (buttonStates & BUTTON4){
      display.println("4");
    } 
    if (buttonStates & BUTTON5){
      display.println("5");
    } 
    if (buttonStates & BUTTON6){
      display.println("6");
    } 
    if (buttonStates & BUTTON7){
      display.println("7");
    }
    //Serial.println(" pressed");
  }
  display.display();
}

void playSong(){
  // iterate over the notes of the melody:
  for (int thisNote = 0; thisNote < 8; thisNote++) {

    // to calculate the note duration, take one second 
    // divided by the note type.
    //e.g. quarter note = 1000 / 4, eighth note = 1000/8, etc.
    int noteDuration = 1000/noteDurations[thisNote];
    tone(speakerPin, melody[thisNote],noteDuration);

    // to distinguish the notes, set a minimum time between them.
    // the note's duration + 30% seems to work well:
    int pauseBetweenNotes = noteDuration * 1.30;
    delay(pauseBetweenNotes);
    // stop the tone playing:
    noTone(speakerPin);
  }
}

void readButtons(){
  buttonStates = 0;
  int curAddress;
  for (curAddress=0; curAddress<nbOfButtons; curAddress++){
    //Turn on pins depending on current Address
    if (curAddress & 1){
      digitalWrite(addressPinA, HIGH);
    } 
    else {
      digitalWrite(addressPinA,LOW);
    }
    if (curAddress & 2){
      digitalWrite(addressPinB, HIGH);
    } 
    else {
      digitalWrite(addressPinB,LOW);
    }
    if (curAddress & 4){
      digitalWrite(addressPinC, HIGH);
    } 
    else {
      digitalWrite(addressPinC,LOW);
    }

    //Read currently selected button input
    int button = digitalRead(buttonInputPin);
    if (button == LOW){
      buttonStates = buttonStates | (1 << curAddress);
    }
  }
}





