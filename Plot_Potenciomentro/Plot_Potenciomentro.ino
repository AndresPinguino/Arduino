// Ejecutar en terminal pyserialplot.py
// http://arduino-er.blogspot.com.ar/2017/06/python-run-on-raspberry-pi-to-plot.html

const int analogIn = A0;
int analogVal = 0;
bool led = 1;

void setup() {
  Serial.begin(9600);
  pinMode(LED_BUILTIN, OUTPUT);
}

void loop() {
  digitalWrite(LED_BUILTIN, led);
  led = !led; 
  analogVal = analogRead(analogIn);
  Serial.println(analogVal);
  delay(50);
}
