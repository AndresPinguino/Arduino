/*
  Enviando cadenas de caracteres por la UART y leyendo en
  la consola con miniterm.py

  miniterm.py -p /dev/ttyUSB0 -b 9600

 */
void setup() {
  //Initialize serial and wait for port to open:
  Serial.begin(9600);
 
 while (!Serial) {
    ; // wait for serial port to connect. Needed for Leonardo only
  }
  // prints title with ending line break
  Serial.println("Gracias Nahuel");
}

int tiempo=500;

void loop() {

  Serial.println("Probando miniterm.py");
  delay(tiempo);
  Serial.println("Micropython en la EDUCIAA-NXP");
  delay(tiempo);
}
