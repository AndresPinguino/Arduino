/*
  Interrupt Example
  Demonstrates how to use interrupts. When a high level is detected at pin 2 interrupt occurs and blinks LED at pin 13

  This example code is in the public domain.
 */

// Pin 13 has an LED connected on most Arduino boards.
// give it a name:
int led = 13;
//create a variable for ISR, has to be volatile if used in interrupt
volatile int bLED = 0;

void setup() {
  //Create interrupt: 0 for pin 2 or 1 for pin 3, the name of the interrupt function or ISR, and condition to trigger interrupt 
  attachInterrupt(0, interruptFunction, HIGH); 
  pinMode(led, OUTPUT); //set up the LED pin to output
  digitalWrite(led,LOW);
}

// the loop routine runs over and over again forever:
void loop() {
  
  if(bLED) { //if this statement becomes true then the interrupt occurred
    digitalWrite(led, HIGH);   // turn the LED on (HIGH is the voltage level)
    delay(300);               // wait 
    digitalWrite(led, LOW);    // turn the LED off by making the voltage LOW
    delay(300);               // wait 
  }
  /*
  noInterrupts(); //disables interrupts
  // critical, time-sensitive code here
  interrupts();//enables interrupts
  */
}

//This is the function called when the interrupt occurs (pin 2 goes high)
//this is often referred to as the interrupt service routine or ISR
//This cannot take any input arguments or return anything
void interruptFunction() {
 bLED = 1; //with this variable set to 1 the LED will blink
 //detachInterrupt(0); this function call will turn the interrupt off
}
