//Variables:
int velocidad; //Variable de la velocidad

void setup() {
  pinMode(2, INPUT); //Irá conectado un botón en el pin 2
  pinMode(3, OUTPUT); //Irá conectado el motor en el pin 3
  pinMode(5, OUTPUT); //Irá conectado el motor en el pin 5
}

void loop() {
  
  if(digitalRead(2)==LOW){
    // ACELERACION
    //For que hara que la variable velocidad incremente
    for(velocidad=0;velocidad < 256;velocidad++){
      analogWrite(3, velocidad); //Incremento en el voltaje en el pin 3 con PWM
      analogWrite(5, velocidad); //Incremento en el voltaje en el pin 5 con PWM
      delay(25); //Retardo que define la rapidez en el aumento de la velocidad
    }
  }
  else{
    // DETENCION    
    //Si el boton no esta presionado
    digitalWrite(3, LOW); //Apaga el motor
    digitalWrite(5, LOW); //Apaga el motor
  }
  
  delay(100); //Retardo de lectura del boton
}
