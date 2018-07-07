// ROBOT SUMO LA TOPADORA = Arduino Nano - Puente H L298 - Sensor SRF04 - Optoacoplador CNY70
// Compilado en Arduino 1.6.5
// Andrés Cintas 12/12/2016

#define S_IDLE 1
#define S_LARGADA 2
#define S_ESPERAR 3
#define S_BUSCAR 4
#define S_DETENIDO 5
#define S_SENSOR_CNY70_DERECHO 6
#define S_CNY70_DERECHO 7
#define S_SENSOR_CNY70_IZQUIERDO 8
#define S_CNY70_IZQUIERDO 9
#define S_SENSOR_CNY70_ATRAS 10
#define S_CNY70_ATRAS 11
#define S_LEER_DISTANCIA 12
#define S_LEER_OPTICOS 13

//Declaración de constantes

// Para manejar el Puente H

// motor A
int enA = 10;
int in1 = 12;
int in2 = 11;

// motor B
int enB = 9;
int in3 = 8;
int in4 = 7;

// Para sensores que detectan línea blanca, guarda el estado del sensor optico CNY70
int CNY70_2,CNY70_3,CNY70_4,CNY70_5;

// Para manejo de interruptores de largada
int SW1 = A5;       // Switch 1

boolean flag;   // Bandera de Estado
long sensor1;    //distancia sensor de Ultrasonido SRF04
long sensor2;    //distancia sensor de Ultrasonido SRF04
long sensor3;    //distancia sensor de Ultrasonido SRF04
long sensor4;    //distancia sensor de Ultrasonido SRF04

// Interrupciones para detectar borde blanco
#include "PinChangeInt.h"  // PinChange Interrupt library, convierte cualquier pin en una linea de interrupcion

const byte interruptPin2 = 2;
const byte interruptPin3 = 3;
const byte interruptPin4 = 4;
const byte interruptPin5 = 5;
volatile int state = S_IDLE;    // ver que no haya conflicto con la definida en la FSM


//Configuración de puertos

void setup() 
{
  Serial.begin(9600);
  //Puente H
  // configurar las lineas de control como salidas
  pinMode(enA, OUTPUT);
  pinMode(enB, OUTPUT);
  pinMode(in1, OUTPUT);
  pinMode(in2, OUTPUT);
  pinMode(in3, OUTPUT);
  pinMode(in4, OUTPUT);
  //Apagar motores
  detenido();

 //Sensores opticos CNY70
 pinMode(interruptPin2,INPUT);    // pin 2 es una entrada
 pinMode(interruptPin3,INPUT);    // pin 3 es una entrada
 pinMode(interruptPin4,INPUT);    // pin 4 es una entrada
 pinMode(interruptPin5,INPUT);    // pin 5 es una entrada
 attachInterrupt(digitalPinToInterrupt(interruptPin2), interrupt_function_2, HIGH); // interrupcion conectada al pin 2
 attachInterrupt(digitalPinToInterrupt(interruptPin3), interrupt_function_3, HIGH); // interrupcion conectada al pin 3
 attachPinChangeInterrupt(interruptPin4, interrupt_function_4, HIGH);  // interrupcion conectada al pin 4, usando libreria.
 
 //Interruptores de largada
 pinMode(SW1,INPUT);    // pin es una entrada
 //pinMode(SW2,INPUT);    // pin es una entrada
 
 //Sensores de distancia SRF04 
 
 pinMode(A0, OUTPUT); // pin  de disparo sensor SRF04
 pinMode(A1, INPUT);  // pin eco sensor SRF04 Derecha
 pinMode(A2, INPUT);  // pin eco sensor SRF04 Frente_der
 pinMode(A3, INPUT);  // pin eco sensor SRF04 Frente_izq
 pinMode(A4, INPUT);  // pin eco sensor SRF04 Izquierda
 
}

// DECLARACION DE SUBRUTINAS

//Función para ejecutar un movimiento
void adelante(int velocidad)
{
  // Encender motor A
  digitalWrite(in1, HIGH);
  digitalWrite(in2, LOW);
  analogWrite(enA, velocidad);
  // Encender motor B
  digitalWrite(in3, HIGH);
  digitalWrite(in4, LOW);
  analogWrite(enB, velocidad);
}


void atras(int velocidad)
{
  // Encender motor A
  digitalWrite(in1, LOW);
  digitalWrite(in2, HIGH);
  analogWrite(enA, velocidad);
  // Encender motor B
  digitalWrite(in3, LOW);
  digitalWrite(in4, HIGH);
  analogWrite(enB, velocidad);
}

void detenido()
{
  // Encender motor A
  digitalWrite(in1, LOW);
  digitalWrite(in2, LOW);
  analogWrite(enA, 0);
  // Encender motor B
  digitalWrite(in3, LOW);
  digitalWrite(in4, LOW);
  analogWrite(enB, 0);
}


void giroizquierda(int velocidad)
{
  // Encender motor A
  digitalWrite(in1, LOW);
  digitalWrite(in2, HIGH);
  analogWrite(enA, velocidad);
  // Encender motor B
  digitalWrite(in3, HIGH);
  digitalWrite(in4, LOW);
  analogWrite(enB, velocidad);
}


void giroderecha(int velocidad)
{
  // Encender motor A
  digitalWrite(in1, HIGH);
  digitalWrite(in2, LOW);
  analogWrite(enA, velocidad);
  // Encender motor B
  digitalWrite(in3, LOW);
  digitalWrite(in4, HIGH);
  analogWrite(enB, velocidad);
}


//Convierte microsegundos a centimetros usando la velocidad del sonido
long microsecondsToCentimeters(long microseconds)
{
  // The speed of sound is 340 m/s or 29 microseconds per centimeter.
  // The ping travels out and back, so to find the distance of the
  // object we take half of the distance travelled.
  return microseconds / 29 / 2;
}


//Medir distancia en centimetros
long distancia(int trigger, int echo)
{
  // establish variables for duration of the ping, 
  // and the distance result in centimeters:
  long duration, cm;

  // The SRF04 is triggered by a HIGH pulse of 5 or more microseconds.
  // Give a short LOW pulse beforehand to ensure a clean HIGH pulse:
  
  digitalWrite(trigger, LOW);
  delayMicroseconds(2);
  digitalWrite(trigger, HIGH);
  delayMicroseconds(5);
  digitalWrite(trigger, LOW);

  // The echoPin is used to read the signal from the SRF04, a HIGH
  // pulse whose duration is the time (in microseconds) from the sending
  // of the ping to the reception of its echo off of an object.
  
  duration = pulseIn(echo, HIGH,100000); // Timeout son 60 microsegundos/1 cm

  // convierte el tiempo en distancia
  cm = microsecondsToCentimeters(duration);
  //Restringo a distancias mayores a 2cm y menores a 100cm
  if(2 < cm < 100){
    return cm;
  }
}


void leer_sensores_de_distancia(){

  sensor1=distancia(A0,A1);  //Derecha
  sensor2=distancia(A0,A2);  //Frontal_der
  sensor3=distancia(A0,A3);  //Frontal_izq
  sensor4=distancia(A0,A4);  //Izquierda

  Serial.print(sensor1);
  Serial.print(" cm ");
  Serial.print(sensor2);
  Serial.print(" cm  ");
  Serial.print(sensor3);
  Serial.print(" cm ");
  Serial.print(sensor4);
  Serial.print(" cm");
  Serial.println();
  delay(150); //espera 50 ms para visualizar
}

void leer_sensores_opticos(){

  CNY70_2= digitalRead(interruptPin2);
  CNY70_3= digitalRead(interruptPin3);
  CNY70_4= digitalRead(interruptPin4);
  CNY70_5= digitalRead(interruptPin5);

  Serial.print("Sensor_1 ");
  Serial.print(CNY70_2);
  Serial.print(" Sensor_2 ");
  Serial.print(CNY70_3);
  Serial.print(" Sensor_3 ");
  Serial.print(CNY70_4);
  Serial.print(" Sensor_4 ");
  Serial.print(CNY70_5);
  Serial.println();
}

void detectarobstaculo(int radio)
{
  sensor1=distancia(A0,A1);  //Derecha
  sensor2=distancia(A0,A2);  //Frontal_der
  sensor3=distancia(A0,A3);  //Frontal_izq
  sensor4=distancia(A0,A4);  //Izquierda
  
  if (2 < sensor2 || sensor3 < radio) // Deteccion al frente
      {
      adelante(255);
      }
 
  if (2 < sensor1 < radio) // Deteccion a la derecha
      {
      giroderecha(150);
      delay(800);
      }
 
 if (2 < sensor4 < radio) // Deteccion a la izquierda
      {
      giroizquierda(150);
      delay(800);
      }
}

/*

https://hackingmajenkoblog.wordpress.com/2016/02/01/the-finite-state-machine/

Stimulus
This is any input coming in from outside the FSM, be it a light sensor, a button, some other part of the program, or whatever.
More importantly, and this is where it differs from just an “input”, it is a change in a value from a sensor. 
Such as a button changing from “not pressed” to “pressed”,or a light sensor changing from “light” to “dark”.

Action
An instruction for something to actually happen, such as opening the gate, turning the light on, etc.

Static State
A state where the FSM is waiting for stimulus, and is taking no actions.

Transitional State
A state which causes actions, but doesn’t look for stimulus. A Transitional State runs once and immediately moves on to a Static State.
*/



// PROGRAMA PRINCIPAL

void loop()
{
  static int state = S_IDLE; // initial state is 1, the "idle" state.
  static unsigned long ts;  // To store the "current" time in for delays.
  
  if (digitalRead(SW1) == LOW && flag==false){
      flag = true;
      state = S_DETENIDO;
      }
        else if (digitalRead(SW1) == HIGH && flag==true){ 
                  flag=false;
                  state=S_LARGADA;
                  }
      
  switch(state)
  {
    case S_IDLE:
      // We don't need to do anything here, waiting for a forced state change.
      break;

    //Transitional State  
    case S_LARGADA:
      ts = millis();  // Remember the current time
      detenido();
      state = S_ESPERAR;  // Move to the next state
      break;
      
    //Static State
    case S_ESPERAR:
      // If five seconds has passed, then move on to the next state.
      if(millis() > ts + 5000)
      {
        state = S_BUSCAR;
      }
      break;
      
    case S_BUSCAR:
      detectarobstaculo(90); // Detecta obstaculos en un radio de 90 cm
      state = S_BUSCAR;
      break;
    
    //Transitional State
    case S_SENSOR_CNY70_DERECHO:
      //acciones a realizar para no caerse del tablero
      atras(200);
      ts = millis();  // Remember the current time
      state = S_CNY70_DERECHO;  // Move to the next state
      break;
      
    //Static State
    case S_CNY70_DERECHO:
      if(millis() > ts + 500)
        {
          state = S_BUSCAR;
        }
      break;
    
    //Transitional State
    case S_SENSOR_CNY70_IZQUIERDO:
      //acciones a realizar para no caerse del tablero
      atras(200);
      ts = millis();  // Remember the current time
      state = S_CNY70_IZQUIERDO;  // Move to the next state
      break;

    //Static State
    case S_CNY70_IZQUIERDO:
      if(millis() > ts + 500)
        {
          state = S_BUSCAR;
        }
      break;

    //Transitional State
    case S_SENSOR_CNY70_ATRAS:
      //acciones a realizar para no caerse del tablero
      adelante(200);
      ts = millis();  // Remember the current time
      state = S_CNY70_ATRAS;  // Move to the next state
      break;
    
    //Static State
    case S_CNY70_ATRAS:
      if(millis() > ts + 500)
        {
          state = S_BUSCAR;
        }
      break;

    case S_LEER_DISTANCIA:
      //lee los sensores de distancia y los muestra en el monitor serial
      leer_sensores_de_distancia();
      state = S_LEER_DISTANCIA;
      break;

    case S_LEER_OPTICOS:
      //lee los sensores opticos y los muestra en el monitor serial
      leer_sensores_opticos();
      state = S_LEER_OPTICOS;
      break;

    //Transitional State
    case S_DETENIDO:
      // We only get here when forced from outside.
      detenido();  // Turn off the robot
      state = S_IDLE;  // Return to the "idle" state.
      break;
    default:
      state = S_IDLE;
      break;
  }
}


//Interrupciones hardware cambian la variable state
void interrupt_function_2(){
  state = S_SENSOR_CNY70_DERECHO;
}

void interrupt_function_3(){
  state = S_SENSOR_CNY70_IZQUIERDO;
}

void interrupt_function_4(){
  state = S_SENSOR_CNY70_ATRAS;
}
