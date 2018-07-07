// ROBOT SUMO LA TOPADORA = Arduino Nano - Puente H L298 - Sensor SRF04 - Optoacoplador CNY70
// Compilado en Arduino 1.6.5
// Andrés Cintas 12/12/2016

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
 pinMode(2,INPUT);    // pin 2 es una entrada
 pinMode(3,INPUT);    // pin 3 es una entrada
 pinMode(4,INPUT);    // pin 4 es una entrada
 pinMode(5,INPUT);    // pin 5 es una entrada
 
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

  CNY70_2= digitalRead(2);
  CNY70_3= digitalRead(3);
  //CNY70_4= digitalRead(4);
  //CNY70_5= digitalRead(5);

  Serial.print("Sensor_1 ");
  Serial.print(CNY70_2);
  Serial.print(" Sensor_2 ");
  Serial.print(CNY70_3);
  //Serial.print(" Sensor_3 ");
  //Serial.print(CNY70_4);
  //Serial.print(" Sensor_4 ");
  //Serial.print(CNY70_5);
  Serial.println();
  delay(150);
}



//Detectar línea blanca
void detectarlinea()
{
    // Consulta SI los sensores digitales están en nivel HIGH mediante una función OR, para detectar blanco
   // Es LOW para color NEGRO
   // Es HIGH para color BLANCO
    if (digitalRead(2) || digitalRead(3) || digitalRead(4) || digitalRead(5))
   {
     
      // Detecta cual sensor se activo y toma una acción en consecuencia
      if(digitalRead(2))
    {
      detenido();
      delay(100);
      giroderecha(150);
      delay(500);
        atras(255);
      delay(500);
     }
      if (digitalRead(4))
    {
        adelante(255);
      delay(800);      
      }
      if (digitalRead(5))
    {
        adelante(255);
      delay(800);
      }
      if (digitalRead(3))
    {
        detenido();
      delay(100);
      giroizquierda(150);
      delay(500);
      atras(255);
      delay(500);       
      }
    }
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


void largada()
{
            while (digitalRead(SW1) == LOW)
            {
            detenido();
            flag = false;
            }
            if (flag == false) // Llave de Largada en RB0
            {
                    detenido();
                    delay(5500); // Tiempo de Largada 5 segundos, solo se ejecuta una vez
                    flag = true; // Actualizar bandera
            }
  
}

// PROGRAMA PRINCIPAL

void loop() 
{
            //largada();
            //detectarlinea();
            //detectarobstaculo(90); // Detecta obstaculos en un radio de 90 cm
            //adelante(150);
            //leer_sensores_de_distancia(); 
            leer_sensores_opticos();  
}
