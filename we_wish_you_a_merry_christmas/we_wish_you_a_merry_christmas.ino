/*
  Melody

 Plays a melody

 http://arduinomelodies.blogspot.com.ar/2013/12/we-wish-you-merry-christmas.html

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
int Cantidad_de_Notas = 84;  // modificar segun el tema elegido

// notes in the melody:
int melody[] = {
   NG4,NC5,NC5,ND5,NC5,NB4,NA4,NA4,
   NA4,ND5,ND5,NE5,ND5,NC5,NB4,NG4,
   NG4,NE5,NE5,NF5,NE5,ND5,NC5,NA5,NG4,NG4,NA4,ND5,NB4,NC5,
   NG4,NC5,NC5,ND5,NC5,NB4,NA4,NA4,
   NA4,ND5,ND5,NE5,ND5,NC5,NB4,NG4,
   NG4,NE5,NE5,NF5,NE5,ND5,NC5,NA5,NG4,NG4,NA4,ND5,NB4,NC5,
   NG4,NC5,NC5,NC5,NB4,NB4,NC5,NB4,NA4,NG4,
   ND5,NE5,ND5,ND5,NC5,NC5,NG5,NG4,NG4,NG4,NA4,ND5,NB4,NC5 
   };

int noteDurations[] = {
                        4,4,8,8,8,8,4,4,
                        4,4,8,8,8,8,4,4,
                        4,4,8,8,8,8,4,4,8,8,4,4,4,2,
                        4,4,8,8,8,8,4,4,
                        4,4,8,8,8,8,4,4,
                        4,4,8,8,8,8,4,4,8,8,4,4,4,2,
                        4,4,4,4,2,4,4,4,4,2,
                        4,4,8,8,8,8,4,4,8,8,4,4,4,2,   
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
