#include "PinChangeInt.h"  // PinChange Interrupt library, convierte cualquier pin en una linea de interrupcion

const int ledPin = 13;
const int buzzerPin = 6;
const int interruptPin2 = 2;
const int interruptPin3 = 3;
volatile int state = 1;


// Interrupciones
void interrupt_function_2(){
  state = 2;
}

void interrupt_function_3(){
  state = 3;
}


void setup() {
  pinMode(ledPin, OUTPUT);
  pinMode(buzzerPin, OUTPUT);
  pinMode(interruptPin2, INPUT);
  pinMode(interruptPin3, INPUT);
  //pinMode(interruptPin, INPUT_PULLUP);
  //attachInterrupt(digitalPinToInterrupt(interruptPin2),interrupt_function_2, RISING);
  //attachInterrupt(digitalPinToInterrupt(interruptPin3),interrupt_function_3, RISING);
  //attachPinChangeInterrupt(4, interrupt_function_4, HIGH);  // interrupt connected to pin 4
  digitalWrite(ledPin,LOW);
  digitalWrite(buzzerPin,LOW);
}

void loop() {
  
  if (digitalRead(interruptPin2)){
    state=2;
    }else if (digitalRead(interruptPin3)){
              state=3;
  }
  
  switch(state)
  {
    case 1:
      // IDLE state, We don't need to do anything here, waiting for a forced state change.
      break;
    case 2:
        blinkled();
        state = 1; // Return to the "IDLE" state.
        break;
    case 3:
        blinkbuzzer();
        state = 1;  // Return to the "IDLE" state.
        break;
    default:
      state = 1;
      break;
   }
}

// Funciones utiles
void blinkled(){
  for ( int i=0; i<=5; i+=1){
        digitalWrite(ledPin, HIGH);   // turn the LED on (HIGH is the voltage level)
        delay(300);               // wait 
        digitalWrite(ledPin, LOW);    // turn the LED off by making the voltage LOW
        delay(300);               // wait 
  }
}


void blinkbuzzer(){
  for ( int i=0; i<=5; i+=1){
        digitalWrite(buzzerPin, HIGH);   // turn the LED on (HIGH is the voltage level)
        delay(100);               // wait 
        digitalWrite(buzzerPin, LOW);    // turn the LED off by making the voltage LOW
        delay(100);               // wait 
  }
}





