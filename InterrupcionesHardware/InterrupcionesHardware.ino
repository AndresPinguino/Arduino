#include "PinChangeInt.h"  // PinChange Interrupt library, convierte cualquier pin en una linea de interrupcion

const byte ledPin = 13;
const byte interruptPin = 2;
//const byte interruptPin = 3;
volatile byte state = LOW;
volatile int bLED = 0;

void setup() {
  pinMode(ledPin, OUTPUT);
  pinMode(interruptPin, INPUT);
  //pinMode(interruptPin, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(interruptPin), blink, HIGH);
  attachPinChangeInterrupt(4, interrupt_function_4, HIGH);  // interrupt connected to pin 4
  digitalWrite(ledPin,LOW);
}

void loop() {
  
  digitalWrite(ledPin, state);
  
   if(bLED) { //if this statement becomes true then the interrupt occurred
      for(int i=0; i<=5; i+=1){
        digitalWrite(ledPin, HIGH);   // turn the LED on (HIGH is the voltage level)
        delay(300);               // wait 
        digitalWrite(ledPin, LOW);    // turn the LED off by making the voltage LOW
        delay(300);               // wait 
      }
   bLED=0;
  }
}

void blink() {
  state = !state;
}

void interrupt_function_4(){
  bLED = 1;
}

