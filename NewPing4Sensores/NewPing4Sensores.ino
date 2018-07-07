// ---------------------------------------------------------------------------
// Example NewPing library sketch that pings 3 sensors 20 times a second.
// ---------------------------------------------------------------------------

#include <NewPing.h>

#define SONAR_NUM 4      // Number of sensors.
#define MAX_DISTANCE 500 // Maximum distance (in cm) to ping.
int iterations=5;

NewPing sonar[SONAR_NUM] = {   // Sensor object array.
  NewPing(A0, A1, MAX_DISTANCE), // Each sensor's trigger pin, echo pin, and max distance to ping. 
  NewPing(A0, A2, MAX_DISTANCE), 
  NewPing(A0, A3, MAX_DISTANCE),
  NewPing(A0, A4, MAX_DISTANCE)
};

void setup() {
  Serial.begin(9600); // Open serial monitor at 9600 baud to see ping results.
}

void loop() { 
  for (uint8_t i = 0; i < SONAR_NUM; i++) { // Loop through each sensor and display results.
    delay(50); // Wait 50ms between pings (about 20 pings/sec). 29ms should be the shortest delay between pings.
    Serial.print("Sensor ");
    Serial.print(i+1);
    Serial.print(" = ");
    //Serial.print(sonar[i].ping_cm()); // devuelve 0cm si no hay un eco dentro de la distancia maxima.
    Serial.print((sonar[i].ping_median(iterations))/29/2); // devuelve microsegundos y luego se convierte a cm, retorna 0 si no hay un eco dentro de la distancia maxima.
    Serial.print("cm    ");
  }
  Serial.println();
}
