//http://playground.arduino.cc/uploads/Code/FSM.zip
#include <FiniteStateMachine.h>

const byte NUMBER_OF_STATES = 4; //how many states are we cycling through?

//initialize states
State On = State(ledOn);
State Off = State(ledOff); 
State FadeIn = State(ledFadeIn);
State FadeOut = State(ledFadeOut); 

FSM ledStateMachine = FSM(On);     //initialize state machine, start in state: On

byte buttonPresses = 0;            //counter variable, hols number of button presses

void setup(){ 
  Serial.begin(9600);
}

//poor example, but then again; it's just an example
void loop(){
 if (Serial.available() > 0) {
    int inByte = Serial.read();
    //increment buttonPresses and constrain it to [ 0, 1, 2, 3 ]
    buttonPresses = ++buttonPresses % NUMBER_OF_STATES; 
    switch (buttonPresses){
      case 0: ledStateMachine.transitionTo(On); break;
      case 1: ledStateMachine.transitionTo(Off); break;
      case 2: ledStateMachine.transitionTo(FadeIn); break;
      case 3: ledStateMachine.transitionTo(FadeOut); break;
    }
  }
  ledStateMachine.update();
}

//utility functions
void ledOn(){ Serial.println("LED ON"); }
void ledOff(){ Serial.println("LED OFF"); }
void ledFadeIn(){ Serial.println("FADE IN"); }
void ledFadeOut(){ Serial.println("FADE OUT"); }
//end utility functions
