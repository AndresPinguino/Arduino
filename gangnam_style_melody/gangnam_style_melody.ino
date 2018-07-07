/*
  Melody

  Plays a melody
  
  http://arduinomelodies.blogspot.com.ar/2013/11/gangnam-style.html
 

 circuit:
 * 8-ohm speaker on digital pin 8

 created 21 Jan 2010
 modified 30 Aug 2011
 by Tom Igoe

This example code is in the public domain.

 http://www.arduino.cc/en/Tutorial/Tone

 */
#include "pitches.h"
int buzzer = 6;
int Cantidad_de_Notas = 64;  // modificar segun el tema elegido

// notes in the melody:
int melody[] = {
                NA4,NA4,NC5,0,NA4,0,NE5,NE5,NE5,0,ND5,0,
                ND5,ND5,NE5,0,0,NE5,NE5,NE5,NE5,0,0,
                NE5,NE5,NE5,NE5,NE5,NE5,NE5,NE5,NE5,NE5,NE5,NE5,NE5,NE5,NE5,NE5,NE5,
                NE5,NE5,NE5,NE5,NE5,NE5,NE5,NE5,NE5,NE5,NE5,NE5,NE5,NE5,NE5,0,0,
                NA4,NA4,NC5,NC5,NA4,0,0
                };


int noteDurations[] = {
                       8,8,4,8,8,4,8,8,4,8,8,4, // 12
                       8,8,4,8,4,8,8,8,4,8,4, // 11
                       8,8,8,8,8,8,8,8,8,8,8,4,4,4,4,16,16, // 17
                       16,16,16,16,16,16,16,16,16,16,16,16,16,16,1,1,4, // 17
                       8,8,8,8,4,1,4 // 7
                       };
void setup() {
  // iterate over the notes of the melody:
  for (int thisNote = 0; thisNote < Cantidad_de_Notas; thisNote++) {

    // to calculate the note duration, take one second
    // divided by the note type.
    //e.g. quarter note = 1000 / 4, eighth note = 1000/8, etc.
    int noteDuration = 1000 / noteDurations[thisNote];
    tone(buzzer, melody[thisNote], noteDuration);

    // to distinguish the notes, set a minimum time between them.
    // the note's duration + 30% seems to work well:
    int pauseBetweenNotes = noteDuration * 1.30;
    delay(pauseBetweenNotes);
    // stop the tone playing:
    noTone(buzzer);
  }
}

void loop() {
  // no need to repeat the melody.
}
