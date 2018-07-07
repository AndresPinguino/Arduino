/*
A traffic light for an intersection of a
highway and a country road. Normally, the light
should be green for the highway and red for the
country road, but when traffic approaches on
the country road, the highway gets a red light
and the country road gets a green light.
 
When a light turns red it transitions from green to
red it goes through yellow, but a red light changing
to green transitions directly to green.
 
A pushbutton represents a car approaching on
the country road.
 
Implement the solution with a Finite State
Machine or FSM.
 
Following the excellent description at
http://hacking.majenko.co.uk/finite-state-machine
first break down the problem into states.
 
Identify which states are Transitional (T) and
which are Static (S). A Static state is one in
which the FSM is waiting for stimulus, and is
taking no actions. A Transitional State is a
state which causes actions, but doesn't look
for stimulus.
 
A Transitional State runs once and immediately
moves on to a Static State.
 
State 0: highway = green, country = red; (T)
State 1: wait for car on country road (S)
State 2: highway = yellow, make note of current time (T)
State 3: wait for yellow time to pass (S)
State 4: highway = red, country =  green, make note of current time (T)
State 5: wait for highway red time to pass (S)
State 6: country = yellow, make note of current time (T)
state 7: wait for yellow time to pass (S) then go to 0
*/
 
// Use names for states so it's easier to
// understand what the code is doing
const int S_HIGHWAYGREEN = 0;
const int S_WAITCARCOUNTRY = 1;
const int S_HIGHWAYYELLOW = 2;
const int S_WAITHIGHWAYYELLOW = 3;
const int S_HIGHWAYRED = 4;
const int S_WAITHIGHWAYRED = 5;
const int S_COUNTRYYELLOW = 6;
const int S_WAITCOUNTRYYELLOW = 7;
 
// Pin numbers
const int countrySensorPin = 2;
 
const int highwayGreenLEDPin = 3;
const int highwayYellowLEDPin = 4;
const int highwayRedLEDPin = 5;
 
const int countryGreenLEDPin = 6;
const int countryYellowLEDPin = 7;
const int countryRedLEDPin = 8;
 
void setup()
{
  pinMode(highwayGreenLEDPin, OUTPUT);
  pinMode(highwayYellowLEDPin, OUTPUT);
  pinMode(highwayRedLEDPin, OUTPUT);
  pinMode(countryGreenLEDPin, OUTPUT);
  pinMode(countryYellowLEDPin, OUTPUT);
  pinMode(countryRedLEDPin, OUTPUT);
}
 
void loop()
{
 
  // start off with the highway getting green
  // The keyword "static" makes sure the variable
  // isn't destroyed after each loop
  static int state = S_HIGHWAYGREEN ;
 
  // To store the current time  for delays
  static unsigned long ts;
 
  switch (state)
  {
    case S_HIGHWAYGREEN:
      // Highway gets green, country road red
      digitalWrite( highwayGreenLEDPin, HIGH);
      digitalWrite( highwayYellowLEDPin, LOW);
      digitalWrite( highwayRedLEDPin, LOW);
 
      digitalWrite( countryGreenLEDPin, LOW);
      digitalWrite( countryYellowLEDPin, LOW);
      digitalWrite( countryRedLEDPin, HIGH);
 
      state = S_WAITCARCOUNTRY;
 
      break;
 
    case S_WAITCARCOUNTRY:
 
      if ( digitalRead (countrySensorPin) == HIGH) {
        state = S_HIGHWAYYELLOW;
      }
 
      break;
 
    case S_HIGHWAYYELLOW:
      digitalWrite( highwayGreenLEDPin, LOW);
      digitalWrite( highwayYellowLEDPin, HIGH);
      digitalWrite( highwayRedLEDPin, LOW);
 
      digitalWrite( countryGreenLEDPin, LOW);
      digitalWrite( countryYellowLEDPin, LOW);
      digitalWrite( countryRedLEDPin, HIGH);
 
      ts = millis();  // Remember the current time
 
      state = S_WAITHIGHWAYYELLOW;  // Move to the next state
 
      break;
 
    case S_WAITHIGHWAYYELLOW:
      // If two seconds have passed, then move on to the next state.
      if (millis() > ts + 2000)
      {
        state = S_HIGHWAYRED;
      }
 
      break;
 
    case S_HIGHWAYRED:
      // Highway red, country road green
      digitalWrite( highwayGreenLEDPin, LOW);
      digitalWrite( highwayYellowLEDPin, LOW);
      digitalWrite( highwayRedLEDPin, HIGH);
 
      digitalWrite( countryGreenLEDPin, HIGH);
      digitalWrite( countryYellowLEDPin, LOW);
      digitalWrite( countryRedLEDPin, LOW);
 
      ts = millis();  // Remember the current time
 
      state = S_WAITHIGHWAYRED;
 
      break;
 
    case S_WAITHIGHWAYRED:
 
      // If five seconds have passed, then start
      // transition to a red light for the country 
      // road
      if (millis() > ts + 5000)
      {
        state = S_COUNTRYYELLOW;
      }
 
      break;
 
    case S_COUNTRYYELLOW:
      digitalWrite( highwayGreenLEDPin, LOW);
      digitalWrite( highwayYellowLEDPin, LOW);
      digitalWrite( highwayRedLEDPin, HIGH);
 
      digitalWrite( countryGreenLEDPin, LOW);
      digitalWrite( countryYellowLEDPin, HIGH);
      digitalWrite( countryRedLEDPin, LOW);
 
      ts = millis();  // Remember the current time
 
      state = S_WAITCOUNTRYYELLOW;
 
      break;
 
    case S_WAITCOUNTRYYELLOW:
 
      // If two seconds have passed, then go
      // back to the beginning with the highway 
      // getting the green light
      if (millis() > ts + 2000)
      {
        state = S_HIGHWAYGREEN;
      }
 
      break;
 
  } // end of switch
 
  // other things could go on here, and they would not affect the timing 
  // of the traffic light
 
} // end of loop
