/*
  DigitalReadSerial
 Reads a digital input on pin 2, prints the result to the serial monitor

 This example code is in the public domain.
 */

// digital pin 2 has a pushbutton attached to it. Give it a name:
int pushButton2 = 2;
int pushButton3 = 3;
int pushButton4 = 4;
int pushButton5 = 5;
int pushButton = A5;

// the setup routine runs once when you press reset:
void setup() {
  // initialize serial communication at 9600 bits per second:
  Serial.begin(9600);
  // make the pushbutton's pin an input:
  pinMode(pushButton, INPUT);
  pinMode(pushButton2, INPUT);
  pinMode(pushButton3, INPUT);
  pinMode(pushButton4, INPUT);
  pinMode(pushButton5, INPUT);
}

// the loop routine runs over and over again forever:
void loop() {
  // read the input pin:
  int buttonState = digitalRead(pushButton);
  int buttonState2 = digitalRead(pushButton2);
  int buttonState3 = digitalRead(pushButton3);
  int buttonState4 = digitalRead(pushButton4);
  int buttonState5 = digitalRead(pushButton5);
  
  // print out the state of the button:
  Serial.print(buttonState);
  Serial.print("  ");
  Serial.print(buttonState2);
  Serial.print("  ");
  Serial.print(buttonState3);
  Serial.print("  ");
  Serial.print(buttonState4);
  Serial.print("  ");
  Serial.println(buttonState5);
  delay(1);        // delay in between reads for stability
}



