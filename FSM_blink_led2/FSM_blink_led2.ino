#define S_IDLE 1
#define S_LEDON 2
#define S_WAITON 3
#define S_LEDOFF 4
#define S_WAITOFF 5
#define S_TURNOFF 6

void setup()
{
  pinMode(13,OUTPUT);
}

void loop()
{
  static int state = S_LEDON; // initial state is 1, the "idle" state.
  static unsigned long ts;  // To store the "current" time in for delays.

  switch(state)
  {
    case S_IDLE:
      // We don't need to do anything here, waiting for a forced state change.
      break;
    case S_LEDON:
      digitalWrite(13,HIGH);  // Turn on the light
      ts = millis();  // Remember the current time
      state = S_WAITON;  // Move to the next state
      break;
    case S_WAITON:
      // If one second has passed, then move on to the next state.
      if(millis() > ts + 1000)
      {
        state = S_LEDOFF;
      }
      break;
    case S_LEDOFF:
      digitalWrite(13,LOW);  // Turn off the light
      ts = millis();  // Remember the current time
      state = S_WAITOFF;
      break;
    case S_WAITOFF:
      // If one second has passed, then go back to state 2.
      if(millis() > ts + 1000)
      {
        state = S_LEDON;
      }
      break;
    case S_TURNOFF:
      // We only get here when forced from outside.
      digitalWrite(13,LOW);  // Turn off the light
      state = S_IDLE;  // Return to the "idle" state.
      break;
    default:
      state = S_LEDON;
      break;
  }
}
