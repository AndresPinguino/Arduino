
#include "pitches.h"

//Variables:

int pulsador, sensorizquierdo, sensorderecho;
int motorizquierdo = 3;
int motorderecho = 5;
int velocidadALTA = 60; //Variable de la velocidad
int velocidadBAJA = 0;


int buzzer = 6;
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
  Serial.begin(9600);
  pinMode(2, INPUT); //Irá conectado un PULSADOR en el pin 2
  pinMode(8, INPUT); //Irá conectado un CNY70 en el pin 8
  pinMode(9, INPUT); //Irá conectado un CNY70 en el pin 9
  pinMode(motorizquierdo, OUTPUT); //Irá conectado un MOTOR en el pin 3
  pinMode(motorderecho, OUTPUT); //Irá conectado un MOTOR en el pin 5
  //musica();
}

void musica(){
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

void adelante(){
    analogWrite(motorizquierdo, velocidadALTA);
    analogWrite(motorderecho, velocidadALTA);
    }
void giroderecha(){
    analogWrite(motorizquierdo, velocidadALTA);
    analogWrite(motorderecho, velocidadBAJA);
    }    
void giroizquierda(){
    analogWrite(motorizquierdo, velocidadBAJA);
    analogWrite(motorderecho, velocidadALTA);
    } 
void detenido(){
    digitalWrite(motorizquierdo, LOW);
    digitalWrite(motorderecho, LOW);
    }

void pruebasensores(){
  
    pulsador=digitalRead(2);
    sensorizquierdo=digitalRead(8);
    sensorderecho=digitalRead(9);
    
    Serial.print("Pulsador: ");
    Serial.print(pulsador);
    Serial.print("    SensorIzq: ");
    Serial.print(sensorizquierdo);
    Serial.print("    SensorDer: ");
    Serial.println(sensorderecho);
    delay(100);        // espera entre lecturas para estabilidad
    }

void pruebamotores(){

    detenido();
    delay(1000);

    adelante();
    delay(1000);

    detenido();
    delay(1000);

    giroderecha();
    delay(1000);

    detenido();
    delay(1000);

    giroizquierda();
    delay(1000);
}

void seguirlinea(){

    sensorizquierdo=digitalRead(8);
    sensorderecho=digitalRead(9);

    if (digitalRead(sensorizquierdo) == LOW && digitalRead(sensorderecho) == LOW)// NEGRO - NEGRO
            {
            giroizquierda();
            Serial.println("IZQUIERDA_UNO");
            } 
    if (digitalRead(sensorizquierdo) == HIGH && digitalRead(sensorderecho) == LOW)// BLANCO - NEGRO
            {
            adelante();
            Serial.println("ADELANTE");
            }
    if (digitalRead(sensorizquierdo) == HIGH && digitalRead(sensorderecho) == HIGH)// BLANCO - BLANCO
            {
           giroderecha();
           Serial.println("DERECHA");
            }
    if (digitalRead(sensorizquierdo) == LOW && digitalRead(sensorderecho) == HIGH)// NEGRO - BLANCO
           {
           giroizquierda();
           Serial.println("IZQUIERDA_DOS");
           }
}

void largada(){
  
    // Pulsador de largada en pin 2  
    while (digitalRead(2)==LOW){
        detenido();
        }
}

void loop() {
    musica();
    //largada();          //lectura del pulsador de largada
    //seguirlinea();      //realiza el seguimiento de linea Blanca sobre fondo Negro
    //pruebamotores();    //activa los motores en forma secuencial
    //pruebasensores();   //muestra el estado de los CNY70 en el Monitor Serial
}
