#include <QTRSensors.h>
#include "pitches.h"
#include <PID_v1.h>

#define NUM_SENSORS   3     // number of sensors used
#define TIMEOUT       2500  // waits for 2500 microseconds for sensor outputs to go low
#define EMITTER_PIN   QTR_NO_EMITTER_PIN

int pulsador=2;
int motor1=3;
int motor2=5;
int buzzer=6;

// Variables del control PID

const int maximum = 255;   // velocidad maxima de los motores entre 0 y 255
//Define Variables we'll be connecting to
double Setpoint, Input, Output;
//Specify the links and initial tuning parameters
double Kp=2, Ki=0, Kd=50;
PID myPID(&Input, &Output, &Setpoint, Kp, Ki, Kd, DIRECT);


// sensors 0 through 4 are connected to digital pins 8 through 12, respectively
QTRSensorsRC qtrrc((unsigned char[]) {10, 11, 12}, NUM_SENSORS, TIMEOUT, EMITTER_PIN); 
unsigned int sensorValues[NUM_SENSORS];


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

void setup()
{
  pinMode(pulsador,INPUT); // Pulsador
  pinMode(motor1,OUTPUT); // PWM Motor izquierdo
  pinMode(motor2,OUTPUT); // PWM Motor derecho
  pinMode(buzzer,OUTPUT); // Buzzer
  pinMode(13, OUTPUT);
  calibracion();

  Input = qtrrc.readLine(sensorValues,QTR_EMITTERS_ON,1); // Linea Blanca = 1   Linea Negra = 0
  Setpoint = 1000;

  //turn the PID on
  myPID.SetMode(AUTOMATIC);
  Serial.begin(9600);
}


void calibracion(){
  // aviso de inicio de calibracion
  for (int i = 0; i < 3; i++)
  {
    digitalWrite(buzzer, HIGH);
    delay(100);
    digitalWrite(buzzer, LOW);
    delay(100);
  }
  delay(1000);  // Espera 1 segundo antes de calibrar
  digitalWrite(13, HIGH);    // turn on Arduino's LED to indicate we are in calibration mode
  for (int i = 0; i < 400; i++)  // make the calibration take about 10 seconds
  {
    qtrrc.calibrate();       // reads all sensors 10 times at 2500 us per read (i.e. ~25 ms per call)
    
    if (i == 200)
    {
        for (int j = 0; j < 5; j++)
            {
            digitalWrite(buzzer, HIGH);
            delay(100);
            digitalWrite(buzzer, LOW);
            delay(100);
            }
    }

  }
  // aviso finalizacion de la calibracion
  for (int i = 0; i < 3; i++)
  {
    digitalWrite(buzzer, HIGH);
    delay(100);
    digitalWrite(buzzer, LOW);
    delay(100);
  }
  digitalWrite(13, LOW);     // turn off Arduino's LED to indicate we are through with calibration
  musica();
  delay(1000);
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


void leersensores()
{
  // read calibrated sensor values and obtain a measure of the line position from 0 to 5000
  // To get raw sensor values, call:
  //  qtrrc.read(sensorValues); instead of unsigned int position = qtrrc.readLine(sensorValues);
  unsigned int position = qtrrc.readLine(sensorValues,QTR_EMITTERS_ON,1); // Linea Blanca = 1   Linea Negra = 0

  // print the sensor values as numbers from 0 to 1000, where 0 means maximum reflectance and
  // 1000 means minimum reflectance, followed by the line position
  for (unsigned char i = 0; i < NUM_SENSORS; i++)
  {
    Serial.print(sensorValues[i]);
    Serial.print('\t');
  }
  //Serial.println(); // uncomment this line if you are using raw values
  Serial.println(position); // comment this line out if you are using raw values
  
  delay(250);
}


void largada(){
  
    // Pulsador de largada en pin 2  
    while (digitalRead(pulsador)==LOW){
            digitalWrite(motor1,LOW);   // Motor izquierdo apagado
            digitalWrite(motor2,LOW);   // Motor derecho apagado
            // reinicia las variables del PID
            //turn the PID off
            myPID.SetMode(MANUAL);
            //turn the PID on
            myPID.SetMode(AUTOMATIC);
        }
}


void controlPID()
{

  Input = qtrrc.readLine(sensorValues,QTR_EMITTERS_ON,1); // Linea Blanca = 1   Linea Negra = 0
  myPID.Compute();
              
  // Compute the actual motor settings.  We never set either motor
  // to a negative value.

  if (Output > maximum)
    Output = maximum;
  if (Output < -maximum)
    Output = -maximum;

  if (Output < 0)
    {
    analogWrite(motor1,maximum + Output);
    analogWrite(motor2,maximum);
    /*
    Serial.print(power_difference);
    Serial.print('\t');
    Serial.print(maximum + power_difference);
    Serial.print('\t');
    Serial.print(maximum);
    Serial.println();
    delay(250);
    */
    }
  else
    {
    analogWrite(motor1,maximum);
    analogWrite(motor2,maximum - Output);
    /*
    Serial.print(power_difference);
    Serial.print('\t');
    Serial.print(maximum);
    Serial.print('\t');
    Serial.print(maximum - power_difference);
    Serial.println();
    delay(250);
    */
    }
}

void loop()
{
  //leersensores();
  largada();
  controlPID();
}
