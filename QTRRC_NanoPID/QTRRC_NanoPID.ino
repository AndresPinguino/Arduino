#include <QTRSensors.h>
#include "pitches.h"

// This example is designed for use with eight QTR-1RC sensors or the eight sensors of a
// QTR-8RC module.  These reflectance sensors should be connected to digital inputs 3 to 10.
// The QTR-8RC's emitter control pin (LEDON) can optionally be connected to digital pin 2, 
// or you can leave it disconnected and change the EMITTER_PIN #define below from 2 to 
// QTR_NO_EMITTER_PIN.
// The setup phase of this example calibrates the sensor for ten seconds and turns on
// the LED built in to the Arduino on pin 13 while calibration is going on.
// During this phase, you should expose each reflectance sensor to the lightest and 
// darkest readings they will encounter.
// For example, if you are making a line follower, you should slide the sensors across the
// line during the calibration phase so that each sensor can get a reading of how dark the
// line is and how light the ground is.  Improper calibration will result in poor readings.
// If you want to skip the calibration phase, you can get the raw sensor readings
// (pulse times from 0 to 2500 us) by calling qtrrc.read(sensorValues) instead of
// qtrrc.readLine(sensorValues).

// The main loop of the example reads the calibrated sensor values and uses them to
// estimate the position of a line.  You can test this by taping a piece of 3/4" black
// electrical tape to a piece of white paper and sliding the sensor across it.  It
// prints the sensor values to the serial monitor as numbers from 0 (maximum reflectance) 
// to 1000 (minimum reflectance) followed by the estimated location of the line as a number
// from 0 to 5000.  1000 means the line is directly under sensor 1, 2000 means directly
// under sensor 2, etc.  0 means the line is directly under sensor 0 or was last seen by
// sensor 0 before being lost.  5000 means the line is directly under sensor 5 or was
// last seen by sensor 5 before being lost.

#define NUM_SENSORS   5     // number of sensors used
#define TIMEOUT       2500  // waits for 2500 microseconds for sensor outputs to go low
#define EMITTER_PIN   QTR_NO_EMITTER_PIN

int pulsador=2;
int motor1=3;
int motor2=5;
int buzzer=6;

// Variables del control PID
int lastError= 0;
float KP=2;
float KD=50;
const int maximum = 200;   // velocidad maxima de los motores entre 0 y 255

// sensors 0 through 4 are connected to digital pins 8 through 12, respectively
QTRSensorsRC qtrrc((unsigned char[]) {8, 9, 10, 11, 12}, NUM_SENSORS, TIMEOUT, EMITTER_PIN); 
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
            lastError = 0;  // reinicia las variables del PID
        }
}


void controlPID()
{
  // read calibrated sensor values and obtain a measure of the line position from 0 to 5000
  // To get raw sensor values, call:
  //  qtrrc.read(sensorValues); instead of unsigned int position = qtrrc.readLine(sensorValues);
  unsigned int position = qtrrc.readLine(sensorValues,QTR_EMITTERS_ON,1); // Linea Blanca = 1   Linea Negra = 0

  // compute our "error" from the line position.  We will make it so that the error is zero when
  // the middle sensor is over the line, because this is our goal.  Error will range from
  // -2000 to +2000.  If we have sensor 0 on the left and sensor 2 on the right,  a reading of 
  // -2000 means that we see the line on the left and a reading of +2000 means we see the 
  // line on the right.
  int error = position - 2000;
 
  // set the motor speed based on proportional and derivative PID terms
  // KP is the a floating-point proportional constant (maybe start with a value around 0.1)
  // KD is the floating-point derivative constant (maybe start with a value around 5)
  // note that when doing PID, it's very important you get your signs right, or else the
  // control loop will be unstable
  
  // Compute the difference between the two motor power settings,
  // m1 - m2.  If this is a positive number the robot will turn
  // to the right.  If it is a negative number, the robot will
  // turn to the left, and the magnitude of the number determines
  // the sharpness of the turn.  You can adjust the constants by which
  // the proportional, integral, and derivative terms are multiplied to
  // improve performance.
  
  
  int power_difference = KP * error + KD * (error - lastError);
  
  lastError = error;

  
  // Compute the actual motor settings.  We never set either motor
  // to a negative value.

  if (power_difference > maximum)
    power_difference = maximum;
  if (power_difference < -maximum)
    power_difference = -maximum;

  if (power_difference < 0)
    {
    analogWrite(motor1,maximum + power_difference);
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
    analogWrite(motor2,maximum - power_difference);
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
