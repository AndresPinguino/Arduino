/*
  Switch statement  with serial input

 Demonstrates the use of a switch statement.  The switch
 statement allows you to choose from among a set of discrete values
 of a variable.  It's like a series of if statements.

 To see this sketch in action, open the Serial monitor and send any character.
 The characters a, b, c, d, and e, will turn on LEDs.  Any other character will turn
 the LEDs off.

 The circuit:
 * 5 LEDs attached to digital pins 2 through 6 through 220-ohm resistors

 created 1 Jul 2009
 by Tom Igoe

This example code is in the public domain.

 http://www.arduino.cc/en/Tutorial/SwitchCase2
 */

void setup() {
  // initialize serial communication:
  Serial.begin(9600);
}

void loop() {
  // read the sensor:
  if (Serial.available() > 0) {
    int inByte = Serial.read();
    // do something different depending on the character received.
    // The switch statement expects single number values for each case;
    // in this exmaple, though, you're using single quotes to tell
    // the controller to get the ASCII value for the character.  For
    // example 'a' = 97, 'b' = 98, and so forth:

    switch (inByte) {
      case 'a':
        Serial.println("Apretaste letra A");
        break;
      case 'b':
        Serial.println("Apretaste letra B");
        break;
      case 'c':
        Serial.println("Apretaste letra C");
        break;
      case 'd':
        Serial.println("Apretaste letra D");
        break;
      case 'e':
        Serial.println("Apretaste letra E");
        break;
      default:
        Serial.println("Apretaste cualquier tecla");
    }
  }
}

