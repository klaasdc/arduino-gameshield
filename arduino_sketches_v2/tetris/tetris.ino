/*  Tetris game for the Arduino Game Shield

    Written by Klaas De Craemer
*/

/* -------- Display related -------- */
#include <Adafruit_GFX.h>
#include <Adafruit_PCD8544.h>
// pin 8 - Serial clock out (SCLK)
// pin 7 - Serial data out (DIN)
// pin 6 - Data/Command select (D/C)
// pin 9, LCD reset (RST)
Adafruit_PCD8544 display = Adafruit_PCD8544(6, 7, 8, 9);

//Size of playing field
#define MAX_X  83
#define MAX_Y  47

#define FIELD_WIDTH	12
#define FIELD_HEIGHT	12

// /* -------- Sound related ------- */
// #include "pitches.h"
// #define  speakerPin  10

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
#define  nbOfButtons  4

#define UP    1
#define DOWN  2
#define LEFT  3
#define RIGHT 4

/* ------ Game related ------ */
int score = 0;

void setup(){
  Serial.begin(115200);
  
  //Initialise button ports
  pinMode(addressPinA, OUTPUT);
  pinMode(addressPinB, OUTPUT);
  pinMode(addressPinC, OUTPUT);
  pinMode(buttonInputPin, INPUT);
  //Initialise screen
  display.begin();
  display.setContrast(60);
  display.clearDisplay();
  display.display();
  
  buttonStates = 0;
  //Show title screen
  showIntro();
}

/* Display title screen */
void showIntro(){
  display.clearDisplay();
  display.setTextSize(2);
  display.setTextColor(BLACK);
  display.setCursor(8,0);
  display.println("Tetris");
  display.setTextSize(1);
  display.setCursor(8,30);
  display.println("Press a key");
  display.display();
  
  buttonStates = 0;
  while (buttonStates == 0){
    readButtons();
  }
  buttonStates = 0;
  delay(400);//Allow for button release, to avoid immediate death
}

//To keep the game speed constant, we keep track of the last time the snake was moved
unsigned long prevUpdateTime;

/* Main game loop */
void loop(){
  runTetris();
  showIntro();
}

/* Ending, show score */
void die(){
  display.setTextSize(1);
  display.setTextColor(BLACK);
  display.setCursor(12,20);
  display.println("Game over!");
  display.println("Score:");
  display.println(score);
  display.display();
  
  while (true){
    //Do nothing
  }
}


/* Read buttons */
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

#define  BTN_NONE  0
#define  BTN_UP    1
#define  BTN_DOWN  2
#define  BTN_LEFT  3
#define  BTN_RIGHT 4
#define  BTN_START 5
#define  BTN_OTHER 6

int curControl = BTN_NONE;
int prevControl = BTN_NONE;
void readInput(){
  readButtons();
  if (!buttonStates){
    curControl = BTN_NONE;
  } else if (buttonStates & BUTTON1){
    curControl = BTN_DOWN;
  } else if (buttonStates & BUTTON2){
   curControl = BTN_RIGHT;
  } else if (buttonStates & BUTTON3){
    curControl = BTN_UP;
  } else if (buttonStates & BUTTON0){
    curControl = BTN_LEFT;
  } else if (buttonStates & BUTTON4){
    curControl = BTN_START;
  } else {
    curControl = BTN_OTHER;
  }
  if (curControl == prevControl)
    curControl = BTN_NONE;
  else
    prevControl = curControl;
}

void drawField(){
  //Divider between playing field and score panel
  display.drawLine(4*FIELD_WIDTH, 0, 4*FIELD_WIDTH, FIELD_HEIGHT*4, BLACK);
  //Score
  display.setTextSize(1);
  display.setCursor(4*FIELD_WIDTH+5,20);
  display.println(score);
}

void setFieldPixel(int x, int y, int enabled){
  if (enabled == 1){
    //display.drawPixel(x, y, BLACK);
    display.drawRect(x*4, y*4, 4, 4, BLACK);
  }
}
