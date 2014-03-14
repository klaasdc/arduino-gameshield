/*
  Example code to read a button on the Arduino Game Console Shield.
  
  The 8 buttons on the board are connected to an analog multiplexer (type 4051)
  This means it is necessary to first select the button that will be read
  by setting the address on pins A,B and C. Then the output of this button is
  readble at the button input pin.
  
*/

#define  ledPin      13      // the number of the LED pin
#define  addressPinA    4
#define  addressPinB    3
#define  addressPinC    2
#define  buttonInputPin    5

void setup(){
  Serial.begin(9600);
  Serial.print("Read button test");
  
  pinMode(ledPin, OUTPUT);
  
  pinMode(addressPinA, OUTPUT);
  pinMode(addressPinB, OUTPUT);
  pinMode(addressPinC, OUTPUT);
  
  pinMode(buttonInputPin, INPUT);
}


int currentAddress = 0;
int buttonState = 0;

void loop(){
  
  //Select address
  currentAddress = currentAddress +1;
  if (currentAddress  > 7){
    currentAddress = 0;
  }
  
  //Turn on pins depending on selected address
  if (currentAddress & 1){
    digitalWrite(addressPinA, HIGH);
  } else {
    digitalWrite(addressPinA,LOW);
  }
   if (currentAddress & 2){
    digitalWrite(addressPinB, HIGH);
  } else {
    digitalWrite(addressPinB,LOW);
  }
   if (currentAddress & 4){
    digitalWrite(addressPinC, HIGH);
  } else {
    digitalWrite(addressPinC,LOW);
  }
  
  //Read currently selected button input
  delay(1);
  buttonState = digitalRead(5);
  if (buttonState == LOW){
    digitalWrite(ledPin, HIGH);
    Serial.println(currentAddress);
  } else {
    digitalWrite(ledPin, LOW);
  }
 
}
