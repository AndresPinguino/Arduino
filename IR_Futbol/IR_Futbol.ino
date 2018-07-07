/*
   IRremote: IRrecvDemo - demonstrates receiving IR codes with IRrecv
   An IR detector/demodulator must be connected to the input RECV_PIN.
   Version 0.1 July, 2009
   Copyright 2009 Ken Shirriff
   http://arcfn.com
*/

#include <IRremote.h>
#define Red_Pin 6
#define Green_Pin 5
#define Blue_Pin 3
const int code1 = 0xB4B4E21D;
const int code2 = 0xB4B49A65;
const int code3 = 0xB4B45AA5;
const int code4 = 0xB4B412ED;
const int code5 = 0xB4B41AE5;
int var;
int RECV_PIN = A0;

IRrecv irrecv(RECV_PIN);

decode_results results;

void setup()
{
  pinMode(Red_Pin, OUTPUT);
  pinMode(Green_Pin, OUTPUT);
  pinMode(Blue_Pin, OUTPUT);
  color(255, 255, 255);
  
  Serial.begin(9600);
  // In case the interrupt driver crashes on setup, give a clue
  // to the user what's going on.
  Serial.println("Enabling IRin");
  irrecv.enableIRIn(); // Start the receiver
  Serial.println("Enabled IRin");
}

void color(int Rojo, int Verde, int Azul) {
  analogWrite(Red_Pin, Rojo);
  analogWrite(Green_Pin, Verde);
  analogWrite(Blue_Pin, Azul);
  delay(500);
}

void loop() {
  if (irrecv.decode(&results)) {
    var = results.value;
    Serial.println(var, DEC);
    irrecv.resume(); // Receive the next value
  }
  delay(100);
  switch (var) {
    case code1:
      color(0, 255, 255); // Rojo
      break;
    case code2:
      color(255, 0, 255); // Verde
      break;
    case code3:
      color(255, 255, 0); // Azul
      break;
    case code4:
      color(0, 0, 255);
      break;
    case code5:
      color(255, 0, 0);
      break;
    default:
      // if nothing else matches, do the default
      // default is optional
      //color(255, 255, 255);
      break;
  }
}
