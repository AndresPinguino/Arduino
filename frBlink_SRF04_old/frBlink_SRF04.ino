#include <Arduino_FreeRTOS.h>

// define two tasks for Blink & Distancia
void TaskBlink( void *pvParameters );
void TaskDistancia( void *pvParameters );


// the setup function runs once when you press reset or power the board
void setup() {

  // Now set up two tasks to run independently.
  xTaskCreate(
    TaskBlink
    ,  (const portCHAR *)"Blink"   // A name just for humans
    ,  128  // This stack size can be checked & adjusted by reading the Stack Highwater
    ,  NULL
    ,  2  // Priority, with 1 being the highest, and 4 being the lowest.
    ,  NULL );

  xTaskCreate(
    TaskDistancia
    ,  (const portCHAR *) "Distancia"
    ,  128  // Stack size
    ,  NULL
    ,  1  // Priority
    ,  NULL );

  // Now the task scheduler, which takes over control of scheduling individual tasks, is automatically started.
}

void loop()
{
  // Empty. Things are done in Tasks.
}

/*--------------------------------------------------*/
/*---------------------- Tasks ---------------------*/
/*--------------------------------------------------*/

void TaskBlink(void *pvParameters)  // This is a task.
{
  (void) pvParameters;

  // initialize digital pin 13 as an output.
  pinMode(13, OUTPUT);

  for (;;) // A Task shall never return or exit.
  {
    digitalWrite(13, HIGH);   // turn the LED on (HIGH is the voltage level)
    vTaskDelay( 100 / portTICK_PERIOD_MS ); // wait for one second
    digitalWrite(13, LOW);    // turn the LED off by making the voltage LOW
    vTaskDelay( 100 / portTICK_PERIOD_MS ); // wait for one second
  }
}

long microsecondsToCentimeters(long microseconds)
  {
  // The speed of sound is 340 m/s or 29 microseconds per centimeter.
  // The ping travels out and back, so to find the distance of the
  // object we take half of the distance travelled.
  return microseconds / 29 / 2;
  }


void TaskDistancia(void *pvParameters)  // This is a task.
{
  (void) pvParameters;

  // initialize serial communication at 9600 bits per second:
  Serial.begin(9600);
  const int triggerPin = 6;
  const int echoPin = 7;
  pinMode(triggerPin, OUTPUT);
  pinMode(echoPin, INPUT);
  
  for (;;)
  {
  // The PING))) is triggered by a HIGH pulse of 2 or more microseconds.
  // Give a short LOW pulse beforehand to ensure a clean HIGH pulse:
  
  digitalWrite(triggerPin, LOW);
  vTaskDelay( 0.002 / portTICK_PERIOD_MS); // espera 2 us
  digitalWrite(triggerPin, HIGH);
  vTaskDelay(0.005 / portTICK_PERIOD_MS); // espera 5 us
  digitalWrite(triggerPin, LOW);

  // The same pin is used to read the signal from the PING))): a HIGH
  // pulse whose duration is the time (in microseconds) from the sending
  // of the ping to the reception of its echo off of an object.
  
  long duration = pulseIn(echoPin, HIGH);

  // convert the time into a distance
  long cm = microsecondsToCentimeters(duration);

  Serial.print(cm);
  Serial.print(" cm");
  Serial.println();

  vTaskDelay(100 / portTICK_PERIOD_MS); // espera 100 ms para visualizar
  }
}
