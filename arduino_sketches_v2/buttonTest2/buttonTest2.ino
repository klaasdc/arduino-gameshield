/*
  Example code to read a button on the Arduino Game Console Shield.
 
 The 8 buttons on the board are connected to an analog multiplexer (type 4051)
 This means it is necessary to first select the button that will be read
 by setting the address on pins A,B and C. Then the output of this button is
 readble at the button input pin.
 
 */

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

void setup(){
  Serial.begin(9600);
  Serial.print("Read button test");

  pinMode(addressPinA, OUTPUT);
  pinMode(addressPinB, OUTPUT);
  pinMode(addressPinC, OUTPUT);
  pinMode(buttonInputPin, INPUT);
}

void loop(){
  readButtons();
  //Serial.println(buttonStates,BIN);
  if (buttonStates > 0){   
    Serial.print("Button(s) ");
    if (buttonStates & BUTTON0){
      Serial.print("0 ");
    } 
    if (buttonStates & BUTTON1){
      Serial.print("1 ");
    } 
    if (buttonStates & BUTTON2){
      Serial.print("2 ");
    } 
    if (buttonStates & BUTTON3){
      Serial.print("3 ");
    } 
    if (buttonStates & BUTTON4){
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
     }
    Serial.println(" pressed");
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




