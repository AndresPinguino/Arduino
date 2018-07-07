/*
  Melody

 Plays a melody

 http://arduinomelodies.blogspot.com.ar/2013/12/happy-birthday.html

 circuit:
 * 8-ohm speaker on digital pin 8

 created 21 Jan 2010
 modified 30 Aug 2011
 by Tom Igoe

This example code is in the public domain.

 http://www.arduino.cc/en/Tutorial/Tone

 */
#include "pitches.h"
int buzzer = 9;
int Cantidad_de_Notas = 25;  // modificar segun el tema elegido

// notes in the melody:
int melody[] = {NC4,NC4,ND4,NC4,NF4,NE4,
                NC4,NC4,ND4,NC4,NG4,NF4,
                NC4,NC4,NC5,NA4,NF4,NE4,ND4,
                NAS4,NAS4,NA4,NF4,NG4,NF4  
                 };

int noteDurations[] = {6,12,4,4,4,2,
                       6,12,4,4,4,2,
                       6,12,4,4,4,4,2,
                       6,12,4,4,4,2,    
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
