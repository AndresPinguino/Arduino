
#include <QTRSensors.h>

#define NUM_SENSORS   3     // number of sensors used
#define TIMEOUT       2500  // waits for 2500 microseconds for sensor outputs to go low
#define EMITTER_PIN   QTR_NO_EMITTER_PIN

int pulsador=2;
int motor1=3;
int motor2=5;
int buzzer=6;


// sensors 0 through 2 are connected to digital pins 9 through 11, respectively
QTRSensorsRC qtrrc((unsigned char[]) {9, 10, 11}, NUM_SENSORS, TIMEOUT, EMITTER_PIN); 
unsigned int sensorValues[NUM_SENSORS];

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
  delay(1000);
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
        }
}


void seguirlinea()
{
unsigned int sensors[3];
// get calibrated sensor values returned in the sensors array, along with the line position
// position will range from 0 to 2000, with 1000 corresponding to the line over the middle sensor
unsigned int position = qtrrc.readLine(sensorValues,QTR_EMITTERS_ON,1); // Linea Blanca = 1   Linea Negra = 0

// compute our "error" from the line position. We will make it so that the error is zero when
// the middle sensor is over the line, because this is our goal. Error will range from
// -1000 to +1000. If we have sensor 0 on the left and sensor 2 on the right, a reading of -1000
// means that we see the line on the left and a reading of +1000 means we see the line on
// the right.

int error = position - 1000;

int leftMotorSpeed = 45;
int rightMotorSpeed = 45;

if (error < -600) // the line is on the left
	leftMotorSpeed = 0; // turn left
if (error > 600) // the line is on the right
	rightMotorSpeed = 0; // turn right

// set motor speeds using the two motor speed variables above

analogWrite(motor1,leftMotorSpeed);
analogWrite(motor2,rightMotorSpeed);
}

void loop()
{
  //leersensores();
  largada();
  seguirlinea();
}

