// Simple demo of three threads
// LED blink thread, medir distancia thread, detectar borde blanco thread, and idle loop
#include <FreeRTOS_AVR.h>

const uint8_t LED_PIN = 13;
const uint8_t buzzer = 6;


//------------------------------------------------------------------------------
// Parpadeo del LED 13
static void vLEDFlashTask(void *pvParameters) {
  pinMode(LED_PIN, OUTPUT);
  
  // Flash led every 300 ms.
  for (;;) {
    // Turn LED on.
    digitalWrite(LED_PIN, HIGH);
    
    // Sleep for 150 milliseconds.
    vTaskDelay((150L * configTICK_RATE_HZ) / 1000L);
    
    // Turn LED off.
    digitalWrite(LED_PIN, LOW);
    
    // Sleep for 150 milliseconds.
    vTaskDelay((150L * configTICK_RATE_HZ) / 1000L);
  }
}

//------------------------------------------------------------------------------
// Parpadeo del BUZZER 
void vTaskBuzzer(void *pvParameters)
{
  (void) pvParameters;
  // Lineas de entrada y salida
  pinMode(buzzer, OUTPUT);

  for (;;)
  {
    // Turn BUZZER on.
    digitalWrite(buzzer, HIGH);
    
    // Sleep for 50 milliseconds.
    vTaskDelay((50L * configTICK_RATE_HZ) / 1000L);
    
    // Turn BUZZER off.
    digitalWrite(buzzer, LOW);
    
    // Sleep for 50 milliseconds.
    vTaskDelay((50L * configTICK_RATE_HZ) / 1000L);
  }
}

//------------------------------------------------------------------------------
// Esto es una funcion para calcular la distancia en funcion del tiempo
long microsecondsToCentimeters(long microseconds)
{
  // The speed of sound is 340 m/s or 29 microseconds per centimeter.
  // The ping travels out and back, so to find the distance of the
  // object we take half of the distance travelled.
  return microseconds / 29 / 2;
}

//------------------------------------------------------------------------------
// Esto es una funcion para medir distancia con el sensor HC-SRF04
long ultrasonidoSRF04(int triggerPin, int echoPin)
{
  // The PING))) is triggered by a HIGH pulse of 2 or more microseconds.
  // Give a short LOW pulse beforehand to ensure a clean HIGH pulse:
  
  digitalWrite(triggerPin, LOW);
  vTaskDelay((0.002L * configTICK_RATE_HZ) / 1000L); // espera 2 us
  digitalWrite(triggerPin, HIGH);
  vTaskDelay((0.005L * configTICK_RATE_HZ) / 1000L); // espera 5 us
  digitalWrite(triggerPin, LOW);

  // The same pin is used to read the signal from the PING))): a HIGH
  // pulse whose duration is the time (in microseconds) from the sending
  // of the ping to the reception of its echo off of an object.
  
  long duration = pulseIn(echoPin, HIGH,100000);

  // convert the time into a distance
  long cm = microsecondsToCentimeters(duration);
  
  vTaskDelay((50L * configTICK_RATE_HZ) / 1000L); // espera 50 ms para visualizar
  
  return cm;
}

//------------------------------------------------------------------------------

// This is a task.
void vTaskDistancia(void *pvParameters)
{
  (void) pvParameters;
  // Lineas de entrada y salida
  
  pinMode(A0, OUTPUT); //TRIGGER
  pinMode(A1, INPUT); //ECHO
  pinMode(A2, INPUT); //ECHO
  pinMode(A3, INPUT); //ECHO
  pinMode(A4, INPUT); //ECHO
  
  for (;;)
  {
  // Lectura de tres sensores ultrasonicos HC-SRF04
  long sensor1=ultrasonidoSRF04(A0, A1);  //ultrasonidoSRF04(triggerPin, echoPin);
  long sensor2=ultrasonidoSRF04(A0, A2);
  long sensor3=ultrasonidoSRF04(A0, A3);
  long sensor4=ultrasonidoSRF04(A0, A4);
  
  /*
  if (sensor1 < sensor2)
  {
    Serial.print("Izquierda sensor1");
    Serial.println();
  }
  else
  {
    Serial.print("Derecha sensor2");
    Serial.println();
  }
  */
  
  Serial.print(sensor1);
  Serial.print(" cm ");
  Serial.print(sensor2);
  Serial.print(" cm  ");
  Serial.print(sensor3);
  Serial.print(" cm ");
  Serial.print(sensor4);
  Serial.print(" cm");
  Serial.println();
  
  }
}

//------------------------------------------------------------------------------
// This is a task.
void vTaskDetectarBordeBlanco(void *pvParameters)
{
  (void) pvParameters;
  // Lineas de entrada y salida
  pinMode(2, INPUT);
  pinMode(3, INPUT);
  pinMode(4, INPUT);
  pinMode(5, INPUT);
  
  
  for (;;)
  {
  // Lectura digital de cuatro sensores opticos CNY70
  int CNY70_1= digitalRead(2);
  int CNY70_2= digitalRead(3);
  int CNY70_3= digitalRead(4);
  int CNY70_4= digitalRead(5);

  /*
  if (CNY70_1==LOW)
  {
    digitalWrite(buzzer,HIGH);
  }
  else
  {
    digitalWrite(buzzer,LOW);
  }
  
  */
  Serial.print("Sensor_1 ");
  Serial.print(CNY70_1);
  Serial.print(" Sensor_2 ");
  Serial.print(CNY70_2);
  Serial.print(" Sensor_3 ");
  Serial.print(CNY70_3);
  Serial.print(" Sensor_4 ");
  Serial.print(CNY70_4);
  Serial.println();
  
  vTaskDelay((10L * configTICK_RATE_HZ) / 1000L); // espera 10 ms para visualizar
  }
}


//------------------------------------------------------------------------------
// Configuraciones

void setup() {
  // initialize serial communication at 9600 bits per second
  Serial.begin(9600);
  // wait for Arduino NANO
  while(!Serial) {}

  // create blink task
  xTaskCreate(vLEDFlashTask, "Flash_LED", configMINIMAL_STACK_SIZE + 50, NULL, tskIDLE_PRIORITY + 3, NULL);

  // create BUZZER task
  //xTaskCreate(vTaskBuzzer, "Blink_BUZZER", configMINIMAL_STACK_SIZE + 50, NULL, tskIDLE_PRIORITY + 3, NULL);

  // create Distancia task
//  xTaskCreate(vTaskDistancia, "Distancia", configMINIMAL_STACK_SIZE + 100, NULL, tskIDLE_PRIORITY + 1, NULL);

  // create DetectarBordeBlanco task
//  xTaskCreate(vTaskDetectarBordeBlanco, "DetectarBordeBlanco", configMINIMAL_STACK_SIZE + 100, NULL, tskIDLE_PRIORITY + 1, NULL);

  // start FreeRTOS
  vTaskStartScheduler();

  // should never return
  Serial.println(F("Die"));
  while(1);
}
//------------------------------------------------------------------------------
// WARNING idle loop has a very small stack (configMINIMAL_STACK_SIZE)
// loop must never block
void loop() {
}
