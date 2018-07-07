
/*
 Copyright (C) 2014 - A. Khorshidi

 This file is licensed under the Creative Commons
 Attribution-Share Alike 4.0 International license.
 
*/

const int ActuatorPin = 52;
const int SensorPin = A1;
float inByte;

void setup(){
  analogReference(INTERNAL1V1);
  pinMode(ActuatorPin, OUTPUT);
  Serial.begin(9600);
}

void loop(){
  float Volt = analogRead(SensorPin);
  float Temp = 1.1/1023/.01 * Volt;
  if(!Serial.available()){
  Serial.println(Temp,1);
  delay(1000);
  }
  while (Serial.available() > 0) {
      inByte = Serial.read();
      if (inByte == 'H')
      digitalWrite(ActuatorPin, HIGH);
      if (inByte == 'L')
      digitalWrite(ActuatorPin, LOW);
  }
}
