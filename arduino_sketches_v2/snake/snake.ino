/*  Simple snake game for the Arduino Game Shield

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
#define MAX_X  84
#define MAX_Y  48

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

#define UP    2
#define DOWN  0
#define LEFT  3
#define RIGHT 1

/* ------ Game related ------ */
int curLength;
int xs[127];//Max snake length is array length
int ys[127];
int dir;
int score;

int ax = 0;//Apple x
int ay = 0;//Apple y

void setup(){
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
  //Snake start position and direction
  curLength = 5;
  xs[0]=20;  xs[1]=21;  xs[2]=22;  xs[3]=23;  xs[4]=24;
  ys[0]=10;  ys[1]=10;  ys[2]=10;  ys[3]=10;  ys[4]=10;
  dir = 0;
  score = 0;
  buttonStates = 0;
  //Generate random apple position
  randomSeed(analogRead(0));//Use unconnected analog pin to get random seed value
  ax = random(48);
  ay = random(48);
  //Show title screen
  showIntro();
}

void showIntro(){
  display.clearDisplay();
  display.setTextSize(2);
  display.setTextColor(BLACK);
  display.println("Snake!");
  display.setTextSize(1);
  display.println("Press a key");
  display.display();
  
  buttonStates = 0;
  while (buttonStates == 0){
    readButtons();
  }
  buttonStates = 0;
  delay(400);//Allow for button release, to avoid immediate death
}

void loop(){
  //Check buttons and set snake movement direction
  readButtons();
  setDirection();
  
  //Check self-collision with snake
  int i=curLength-1;
  while (i>=2){
    if (collide(xs[0], xs[i], ys[0], ys[i], 1, 1, 1, 1)){
      die();
    }
    i = i-1;
  }
  //Check collision of snake head with apple
  if (collide(xs[0], ax, ys[0], ay, 1, 1, 1, 1)){
    score = score +1;
    curLength = curLength +1;
    //Add snake segment with temporary position of new segments
    xs[curLength-1] = 255;
    ys[curLength-1] = 255;
    //Generate new apple position
    ax = random(48);
    ay = random(48);
  }
  //Check if outside playing field
  if ((xs[0]<0) || (xs[0]>MAX_X) || (ys[0]<0) || (ys[0]>MAX_Y)){
    die();
  }
  //Shift snake position array by one
  i = curLength-1;
  while (i>=1){
    xs[i] = xs[i-1];
    ys[i] = ys[i-1];
    i = i-1;
  }
  //Determine new position of head of snake
  if (dir == RIGHT){
    xs[0] = xs[0] + 1;
  } else if (dir == LEFT){
    xs[0] = xs[0] - 1;
  } else if (dir == UP){
    ys[0] = ys[0] - 1;
  } else {//DOWN
    ys[0] = ys[0] + 1;
  }
  //Draw snake
  display.clearDisplay();
  for (int i=0; i<curLength; i++){
    display.drawPixel(xs[i], ys[i], BLACK);
  }
  //Draw apple as pixel
  display.drawPixel(ax, ay, BLACK);
  display.display();
  
  delay(75);
}

void die(){
  display.setTextSize(1);
  display.setTextColor(BLACK);
  display.setCursor(12,20);
  display.println("Game over!");
  display.println("Score:");
  display.println(score);
  display.display();
  
  while (true){
  }
}

/* Collision detection function */
boolean collide(int x1, int x2, int y1, int y2, int w1, int w2, int h1, int h2){
  if ((x1+w1>x2) && (x1<x2+w2) && (y1+h1>y2) && (y1<y2+h2)){
    return true;
  } else {
    return false;
  }
}

/* Set direction from current button state */
void setDirection(){
  if (buttonStates > 0){
    if (buttonStates & BUTTON1){
      dir = DOWN;
    } else if (buttonStates & BUTTON2){
      dir = RIGHT;
    } else if (buttonStates & BUTTON3){
      dir = UP;
    } else if (buttonStates & BUTTON0){
      dir = LEFT;
    } 
    /*if (buttonStates & BUTTON4){
     Serial.print("4 ");
     } 
     if (buttonStates & BUTTON5){
     Serial.print("5 ");
     } 
     if (buttonStates & BUTTON6){
     Serial.print("6 ");
     } 
     if (buttonStates & BUTTON7){
     Serial.print("7");
     }*/
    //Serial.println(" pressed");
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



