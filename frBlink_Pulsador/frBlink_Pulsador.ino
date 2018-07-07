// Simple demo of three threads
// LED blink thread, print thread, and idle loop
#include <FreeRTOS_AVR.h>

const uint8_t LED_PIN = 13;
const uint8_t LED_PIN2 = 6;
const uint8_t Pulsador = A5;

// handle for blink task
TaskHandle_t blink;

//------------------------------------------------------------------------------
// Parpadeo del LED 13
static void vLEDFlashTask(void *pvParameters) {
  pinMode(LED_PIN, OUTPUT);
  
  // Flash led every 400 ms.
  for (;;) {
    // Turn LED on.
    digitalWrite(LED_PIN, HIGH);
    
    // Sleep for 200 milliseconds.
    vTaskDelay((200L * configTICK_RATE_HZ) / 1000L);
    
    // Turn LED off.
    digitalWrite(LED_PIN, LOW);
    
    // Sleep for 200 milliseconds.
    vTaskDelay((200L * configTICK_RATE_HZ) / 1000L);
  }
}


//------------------------------------------------------------------------------
// Parpadeo del LED 8
void vTaskBlink(void *pvParameters)
{
  (void) pvParameters;
  // Lineas de entrada y salida
  pinMode(LED_PIN2, OUTPUT);
    
  for (;;)
  {
    // Turn LED on.
    digitalWrite(LED_PIN2, HIGH);
    
    // Sleep for 50 milliseconds.
    vTaskDelay((50L * configTICK_RATE_HZ) / 1000L);
    
    // Turn LED off.
    digitalWrite(LED_PIN2, LOW);
    
    // Sleep for 50 milliseconds.
    vTaskDelay((50L * configTICK_RATE_HZ) / 1000L);
  
  }
}



//------------------------------------------------------------------------------
// This is a task.
void vTaskPulsador(void *pvParameters)
{
  (void) pvParameters;
  // Lineas de entrada y salida
  pinMode(Pulsador, INPUT);
    
  for (;;)
  {
    // Lee el estado de un pulsador
    int sensor=digitalRead(Pulsador);
    Serial.print(sensor);
    Serial.println();
    // Sleep for 100 milliseconds.
    vTaskDelay((100L * configTICK_RATE_HZ) / 1000L);
  
  }
}

//------------------------------------------------------------------------------
// Configuraciones

void setup() {
  // initialize serial communication at 9600 bits per second
  Serial.begin(9600);
  // wait for Arduino UNO
  while(!Serial) {}


  // create vTaskPulsador
  xTaskCreate(vTaskPulsador, "Lee_un_pulsador", configMINIMAL_STACK_SIZE + 100,NULL, tskIDLE_PRIORITY + 1, NULL);

  // create blink task
  xTaskCreate(vLEDFlashTask, "Flash_LED1", configMINIMAL_STACK_SIZE + 50, NULL, tskIDLE_PRIORITY + 2, &blink);

  // create vTaskBlink
  xTaskCreate(vTaskBlink, "Flash_LED2", configMINIMAL_STACK_SIZE + 100, NULL, tskIDLE_PRIORITY + 1, NULL);

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
