void setup()
{
  pinMode(13,OUTPUT);
}

void loop()
{
  static int state = 1; // initial state is 1, the "idle" state.
  static unsigned long ts;  // To store the "current" time in for delays.

  switch(state)
  {
    case 1:
      // We don't need to do anything here, waiting for a forced state change.
      break;
    case 2:
      digitalWrite(13,HIGH);  // Turn on the light
      ts = millis();  // Remember the current time
      state = 3;  // Move to the next state
      break;
    case 3:
      // If one second has passed, then move on to the next state.
     if(millis() > ts + 1000)
      {
        state = 4;
      }
      break;
    case 4:
      digitalWrite(13,LOW);  // Turn off the light
      ts = millis();  // Remember the current time
      state = 5;
      break;
    case 5:
      // If one second has passed, then go back to state 2.
      if(millis() > ts + 1000)
      {
        state = 2;
      }
      break;
    case 6:
      // We only get here when forced from outside.
      digitalWrite(13,LOW);  // Turn off the light
      state = 1;  // Return to the "idle" state.
      break;
    default:
      state = 1;
      break;
  }
}
