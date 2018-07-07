
String str;

void setup() {
  // Open serial communications and wait for port to open:
  Serial.begin(9600);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }

  // send an intro:
  Serial.println("Enviar la cadena adelante o detenido");
  Serial.println();
  pinMode(13,OUTPUT);
  digitalWrite(13,LOW);
}



void loop() 
{
    if(Serial.available() > 0)
    {
        str = Serial.readStringUntil('\n');

        if (str=="adelante") {
             digitalWrite(13,HIGH);
             }
        if (str=="detenido") {
             digitalWrite(13,LOW);
             }
    }
}
