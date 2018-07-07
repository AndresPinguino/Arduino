//Variables:

int sensorizquierdo, sensorderecho;

void setup() {
  Serial.begin(9600);
  pinMode(8, INPUT); //Irá conectado un CNY70 en el pin 8
  pinMode(9, INPUT); //Irá conectado un CNY70 en el pin 9
}

void loop() {
  sensorizquierdo=digitalRead(8);
  sensorderecho=digitalRead(9);
  Serial.print(sensorizquierdo);
  Serial.print("   ");
  Serial.println(sensorderecho);
  delay(100); //Retardo de lectura
}
