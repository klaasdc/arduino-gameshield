/*
  Basic space invaders for the Arduino Game Shield
  
  Klaas De Craemer 2013
 */

/* -------- Display related -------- */
#include <Adafruit_GFX.h>
#include <Adafruit_PCD8544.h>
// pin 8 - Serial clock out (SCLK)
// pin 7 - Serial data out (DIN)
// pin 6 - Data/Command select (D/C)
// pin 9, LCD reset (RST)
Adafruit_PCD8544 display = Adafruit_PCD8544(6, 7, 8, 9);

static const unsigned char PROGMEM alien_bmp[] =
{ 
  B00011000, B00111100,
  B01111110, B11011011,
  B11111111, B00100100,
  B01011010, B10100101
};

static const unsigned char PROGMEM player_bmp[] =
{ 
  B00000000, B00000000,
  B00011000, B00011000,
  B01111110, B11111111,
  B11111111, B10111101
};

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

#define UP    0
#define DOWN  1
#define LEFT  2
#define RIGHT 3

int gameSpeed = 190;

void setup()   {
  Serial.begin(9600);

  //Initialise button ports
  pinMode(addressPinA, OUTPUT);
  pinMode(addressPinB, OUTPUT);
  pinMode(addressPinC, OUTPUT);
  pinMode(buttonInputPin, INPUT);

  display.begin();
  // init done

  // you can change the contrast around to adapt the display
  // for the best viewing!
  display.setContrast(60);

  initGame();
}

//To keep the game speed constant, we keep track of the last time the snake was moved
unsigned long prevUpdateTime;

void loop() {
  moveAliens();
  moveMissiles();
  movePlayer();

  display.clearDisplay();
  drawAliens();
  drawMissiles();
  drawPlayer();
  display.display();

  checkAlienMissileImpact();
  checkPlayerMissileImpact();
  checkLevelClear();

  //Check buttons and set snake movement direction while we are waiting to draw the next move
  unsigned long curTime;
  do{
    readButtons();
    processButtons();
    curTime = millis();
  } 
  while ((curTime - prevUpdateTime) < gameSpeed);//Once enough time  has passed, proceed. The lower this number, the faster the game is
  prevUpdateTime = curTime;
}

int score = 0;

struct Missile {
  int speed;
  int x;
  int y;
  boolean enabled;
};
Missile alienMissile;
Missile playerMissile;

struct Player{
  int x;
  int y;
  int lives;
};
Player player;

#define NB_ALIENS  6
#define SPACE_X  10
struct Alien {
  int x;
  int y;
  int lives;
};
//int aliensX = 0;
//int aliensY = 0;
int aliensSpeedX = 2;
int aliensSpeedY = 2;
Alien aliens[NB_ALIENS];

void checkAlienMissileImpact(){
  if (alienMissile.y > player.y){//Check height first
    if ((alienMissile.x > player.x-2) && (alienMissile.x < (player.x+7))){//Check horizontal alignment
      die();
    }
  }
}

void checkPlayerMissileImpact(){
  for (int i=0; i<NB_ALIENS; i++){
    if (aliens[i].lives>0){
      if ((playerMissile.y < (aliens[i].y+8)) && (playerMissile.y > (aliens[i].y))){
        if (playerMissile.x < (aliens[i].x+8) && playerMissile.x > (aliens[i].x)){
          aliens[i].lives = aliens[i].lives -1;
          score = score+1;
        }
      }
    }
  }
}

void checkLevelClear(){
  boolean alienLivesLeft = false;
  for (int i=0; i<NB_ALIENS; i++){
    if (aliens[i].lives>0){
      alienLivesLeft = true;
      break;
    }
  }
  if (!alienLivesLeft){
    levelCleared();
  }
}

void levelCleared(){
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(BLACK);
  display.setCursor(0,20);
  display.println("Level clear!");
  display.println("Score:");
  display.println(score);
  display.display();
  delay(1500);
  
  initGame();
  if (gameSpeed >= 30){
    gameSpeed = gameSpeed - 20;
  }
  prevUpdateTime = millis();
}

void initGame(){
  //Init alien positions
  for (int i=0; i<NB_ALIENS; i++){
    aliens[i].lives = 1;
    aliens[i].x = aliensSpeedX + SPACE_X*i;
    aliens[i].y = 0;
  }
  //Init missiles
  alienMissile.speed = 4;
  alienMissile.x = 0;
  alienMissile.y = 0;
  alienMissile.enabled = false;

  playerMissile.speed = 6;
  playerMissile.x = 0;
  playerMissile.y = 0;
  playerMissile.enabled = false;

  //Init player position and score
  player.x = 10;
  player.y = 40;

  //randomSeed(analogRead(0));//Use unconnected analog pin to get random value
  
  prevUpdateTime = millis();
}

void drawAliens(){
  for (int i=0; i<NB_ALIENS; i++){
    if (aliens[i].lives>0){
      display.drawBitmap(aliens[i].x, aliens[i].y, alien_bmp, 8, 8, 1);
    }
  }
}

void moveAliens(){
  boolean moveDown = false;
  //Check if position needs switching
  for (int i=0; i<NB_ALIENS; i++){
    if (aliens[i].lives > 0){
      if ((aliens[i].x > 74) || aliens[i].x <2){
        aliensSpeedX = -aliensSpeedX;//Switch X direction
        moveDown = true;
        break;
      }
    }
  }
  
  //Move aliens one row down if needed
  if (moveDown == true){
    for (int i=0; i<NB_ALIENS; i++){
      aliens[i].y = aliens[i].y + aliensSpeedY;
      if (aliens[i].y > 34){
        die();
      }
    }
  }
  moveDown = false;

  //Move position of row of aliens left/right
  for (int i=0; i<NB_ALIENS; i++){
    aliens[i].x = aliens[i].x + aliensSpeedX;
  }
}

void moveMissiles(){
  if (alienMissile.enabled == false){
    //Generate new missile start position by selecting a random alien (actually, should check if it is dead first...)
    int alienNb = random(NB_ALIENS-1);
    alienMissile.x = aliens[alienNb].x + 3;//Missile starts in the "middle" of an alien
    alienMissile.y = aliens[alienNb].y + 8;//Missile starts just below alien
    alienMissile.enabled = true;
  } 
  else {
    //Move missile down and remove if it reaches the ground
    alienMissile.y = alienMissile.y + alienMissile.speed;
    if (alienMissile.y>48){
      alienMissile.enabled = false;
    }
  }

  if (playerMissile.enabled == true){
    playerMissile.y = playerMissile.y - playerMissile.speed;
    if (playerMissile.y<0){
      playerMissile.enabled = false;
    } 
  }
}

void drawMissiles(){
  if (alienMissile.enabled){
    display.drawRect(alienMissile.x, alienMissile.y, 2, 3, BLACK);
  }
  if (playerMissile.enabled){
    display.drawRect(playerMissile.x, playerMissile.y, 2, 3, BLACK);
  }
}

void drawPlayer(){
  display.drawBitmap(player.x, player.y, player_bmp, 8, 8, 1);
}

#define PLAYER_SPEED_X  4
int dir = 0;
int shoot = 0;
void movePlayer(){
  if (dir == RIGHT){
    player.x = player.x + PLAYER_SPEED_X;
  }
  if (dir == LEFT){
    player.x = player.x - PLAYER_SPEED_X;
  }
  dir = 0;
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
    //Do nothing
  }
}

/* Set direction from current button state */
void processButtons(){
  if (buttonStates > 0){
    if (buttonStates & BUTTON2){
      //Move right by one
      dir = RIGHT;
    } else if (buttonStates & BUTTON0){
      //Move left by one
      dir = LEFT;
    } else {
      dir = 0;
    }
    
    if ((buttonStates & BUTTON3) || (buttonStates & BUTTON4)){
      //Fire missile
      if (playerMissile.enabled == false){//Only one missile at a time can be active...
        playerMissile.x = player.x + 3;
        playerMissile.y = player.y;
        playerMissile.enabled = true;
      }
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





