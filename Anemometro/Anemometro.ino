#include <TimerOne.h>

// https://brainy-bits.com/tutorials/speed-sensor-with-arduino/
// Para leer rueda dentada con optoacoplador herradura - Anemometro - Motor DC
// La entrada se realiza por el pin2 del Arduino UNO que corresponde a la interrupción externa 0

unsigned int counter=0;

void docount()  // cuenta las interrupciones del sensor de velocidad con optoacoplador
{
counter++;  // incrementa en +1 el valor del contador
} 
void timerIsr()
{
Timer1.detachInterrupt();  // detiene el timer
Serial.print("Anemometro: "); 
int rotation = (counter / 27);  // divide por el numero de agujeros en el disco optoacoplador, 27 para rueda del mouse
Serial.print(rotation,DEC);  
Serial.println(" Rotaciones por segundos"); 
counter=0;  //  reinicia el contador a cero
Timer1.attachInterrupt( timerIsr );  // habilitar el timer
}
void setup() 
{
Serial.begin(9600);
Timer1.initialize(1000000); // setear timer para 1 segundo
attachInterrupt(0, docount, RISING);  // incrementa el contador cuando la entrada del pin2 pasa a nivel High
Timer1.attachInterrupt( timerIsr ); // habilitar el timer
} 
void loop()
{
}
