//#include "PinChangeInt.h"  // PinChange Interrupt library, convierte cualquier pin en una linea de interrupcion

const int ledPin = 13;
const int buzzerPin = 6;
const int interruptPin2 = 2;
const int interruptPin3 = 3;
volatile int state = 0;
static unsigned long ts;  // To store the "current" time in for delays.

#define S_DETENIDO 0
#define S_LARGADA 1
#define S_ESPERAR 2
#define S_BUSCAR 3
#define S_SENSOR_CNY70_DERECHO 4
#define S_SENSOR_CNY70_IZQUIERDO 5
#define S_ESPERAR_CNY70 6

// Interrupciones
void interrupt_function_2(){
  state = S_SENSOR_CNY70_DERECHO;
}

void interrupt_function_3(){
  state = S_SENSOR_CNY70_IZQUIERDO;
}


// Funciones utiles
void blinkled(){
  //for ( int i=0; i<=5; i+=1){
        digitalWrite(ledPin, HIGH);   // turn the LED on (HIGH is the voltage level)
        delay(300);               // wait 
        digitalWrite(ledPin, LOW);    // turn the LED off by making the voltage LOW
        delay(300);               // wait 
 // }
}


void blinkbuzzer(){
  //for ( int i=0; i<=5; i+=1){
        digitalWrite(buzzerPin, HIGH);   // turn the LED on (HIGH is the voltage level)
        delay(1500);               // wait 
        digitalWrite(buzzerPin, LOW);    // turn the LED off by making the voltage LOW
        delay(1500);               // wait 
  //}
}



void flipflop(){
  for ( int i=0; i<=5; i+=1){
        digitalWrite(buzzerPin, HIGH);   // turn the LED on (HIGH is the voltage level)
        digitalWrite(ledPin, LOW);    // turn the LED off by making the voltage LOW
        delay(400);               // wait 
        digitalWrite(buzzerPin, LOW);    // turn the LED off by making the voltage LOW
        digitalWrite(ledPin, HIGH);    // turn the LED off by making the voltage LOW
        delay(400);               // wait 
  }
}

void setup() {
  Serial.begin(9600);
  pinMode(ledPin, OUTPUT);
  pinMode(buzzerPin, OUTPUT);
  //attachInterrupt(digitalPinToInterrupt(interruptPin2),interrupt_function_2, HIGH);
  //attachInterrupt(digitalPinToInterrupt(interruptPin3),interrupt_function_3, HIGH);
  //attachPinChangeInterrupt(4, interrupt_function_4, HIGH);  // interrupt connected to pin 4
  digitalWrite(ledPin,LOW);
  digitalWrite(buzzerPin,LOW);
}

void loop() {

  // Leer Entradas
  if (Serial.available() > 0){
  // get the new byte:
    char inChar = (char)Serial.read();
    state = (int)inChar-48;
    Serial.println(state);
    //delay(500);    
  }

  // Finite State Machine ó Máquina de Estados
  switch(state)
  {
    case S_DETENIDO:
        Serial.println("Estado 0 - DETENIDO");
        digitalWrite(ledPin, LOW);    // turn the LED off by making the voltage LOW = 0V
        state = S_DETENIDO; // Return to the "DETENIDO" state.
        break;
    case S_LARGADA:
        Serial.println("Estado 1 - LARGADA");
        //acciones a realizar en la largada, estar detenido 5 segundos
        digitalWrite(ledPin, HIGH);    // turn the LED oN by making the voltage HIGH = 5V
        ts = millis();  // Remember the current time
        state = S_ESPERAR;  // Return to the "ESPERAR" state.
        break;

    //Static State
    case S_ESPERAR:
      Serial.println("Estado 2 - ESPERAR 5 SEGUNDOS");
      // If five seconds has passed, then move on to the next state.
      if(millis() > ts + 5000)
      {
        state = S_BUSCAR;
      }
      break;

    case S_BUSCAR:
      Serial.println("Estado 3 - BUSCAR");
      blinkled(); // parpadea un led
      state = S_BUSCAR;
      break;


    //Transitional State
    case S_SENSOR_CNY70_DERECHO:
      Serial.println("Estado 4 - CNY70 DERECHO");
      //acciones a realizar para no caerse del tablero
      blinkbuzzer();  // parpadea un buzzer
      ts = millis();  // Remember the current time
      state = S_ESPERAR_CNY70;  // Move to the next state
      break;
    
    //Transitional State
    case S_SENSOR_CNY70_IZQUIERDO:
      Serial.println("Estado 5 - CNY70 IZQUIERDO");
      //acciones a realizar para no caerse del tablero
      flipflop();   // activa un flip-flop
      ts = millis();  // Remember the current time
      state = S_ESPERAR_CNY70;  // Move to the next state
      break;

     //Static State
     case S_ESPERAR_CNY70:
       Serial.println("Estado 6 - ESPERAR CNY70");
       if(millis() > ts + 500)
        {
          state = S_BUSCAR;
        }
      break;
      
    default:
      state = S_DETENIDO;
      break;
   }
}
