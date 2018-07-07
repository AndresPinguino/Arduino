char caracter = 0;
String cadena = "";
boolean newline = false;

void setup() {
  Serial.begin(9600);
  delay(1000);
  pinMode(A0, OUTPUT);
}

void Avance()
{
  digitalWrite(A0, HIGH);
}


void Retroceso()
{ 
   digitalWrite(A0, LOW);
}


void loop() {
  
  while(Serial.available()){
     if (Serial.available()>0){
        caracter = Serial.read();
        cadena = cadena + caracter;
      }

   if (cadena.length()>0){
      if(cadena.equals("adelante")){
          Avance();
      }
      if(cadena.equals("retroceso")){
          Avance();
      }

      cadena = "";
 
   }
 }
}
