/*
  Character analysis operators

 Examples using the character analysis operators.
 Send any byte and the sketch will tell you about it.

 created 29 Nov 2010
 modified 2 Apr 2012
 by Tom Igoe

 This example code is in the public domain.
 */

void setup() {
  // Open serial communications and wait for port to open:
  Serial.begin(9600);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }

  // send an intro:
  Serial.println("send any byte and I'll tell you everything I can about it");
  Serial.println();
  pinMode(13,OUTPUT);
  digitalWrite(13,LOW);
}

void loop() {
  // get any incoming bytes:
  if (Serial.available() > 0) {
    int thisChar = Serial.read();

    // say what was sent:
    Serial.print("You sent me: \'");
    Serial.write(thisChar);
    Serial.print("\'  ASCII Value: ");
    Serial.println(thisChar);

    // analyze what was sent:
    if (thisChar=='a') {
      digitalWrite(13,HIGH);
    }
    if (thisChar=='b') {
      digitalWrite(13,LOW);
    }
  }
}
