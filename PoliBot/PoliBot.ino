// ROBOT POLIBOT = Arduino UNO - Puente H con Relays - Sensor SRF04
// Compilado en Arduino 1.6.5
// Andrés Cintas 18/03/2017

//Declaración de constantes

// Para manejar el Puente H

// motor A
#define in1 8
#define in2 9

// motor B
#define in3 10
#define in4 11

long sensor1;    //distancia sensor de Ultrasonido SRF04
long sensor2;    //distancia sensor de Ultrasonido SRF04
long sensor3;    //distancia sensor de Ultrasonido SRF04
long sensor4;    //distancia sensor de Ultrasonido SRF04

//Configuración de puertos

void setup() 
{
  Serial.begin(9600);
  //Puente H
  // configurar las lineas de control como salidas
  pinMode(in1, OUTPUT);
  pinMode(in2, OUTPUT);
  pinMode(in3, OUTPUT);
  pinMode(in4, OUTPUT);
  //Apagar motores
  detenido();
  delay(2000); // espera 2 segundos
 
 //Sensores de distancia SRF04 
 
 pinMode(A0, OUTPUT); // pin  de disparo sensor SRF04
 pinMode(A1, INPUT);  // pin eco sensor SRF04 Derecha
 pinMode(A2, INPUT);  // pin eco sensor SRF04 Frente_der
 pinMode(A3, INPUT);  // pin eco sensor SRF04 Frente_izq
 pinMode(A4, INPUT);  // pin eco sensor SRF04 Izquierda
 
}

// DECLARACION DE SUBRUTINAS

//Función para ejecutar un movimiento

void robot(int Estado_A, int Estado_B, int Estado_C, int Estado_D){
  digitalWrite(in1, Estado_A);
  digitalWrite(in2, Estado_B);
  digitalWrite(in3, Estado_C);
  digitalWrite(in4, Estado_D); 
}

void adelante()
{
  robot(HIGH,LOW,HIGH,LOW);
}

void atras()
{
  robot(LOW,HIGH,LOW,HIGH);
}

void detenido()
{
  robot(LOW,LOW,LOW,LOW);
}


void giroderecha()
{
  robot(LOW,HIGH,HIGH,LOW);
}


void giroizquierda()
{
  robot(HIGH,LOW,LOW,HIGH);
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

  sensor1=distancia(A0,A1);  // Leer sensor

  Serial.print(sensor1);
  Serial.print(" cm");
  Serial.println();
  delay(150); //espera 150 ms para visualizar
}


void detectarobstaculo(int radio)
{
  sensor1=distancia(A0,A1);  // Lectura del sensor

  if (sensor1 < radio) // Deteccion a la derecha
      {
      detenido();
      delay(100);
      giroizquierda();
      delay(600);
      detenido();
      delay(100);
      }
}



// PROGRAMA PRINCIPAL

void loop() 
{

              detectarobstaculo(30); // Detecta obstaculos en un radio de 50 cm
              adelante();
              //leer_sensores_de_distancia();  
}
