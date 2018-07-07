//Variables:

int pulsador, sensorizquierdo, sensorderecho;
int motorizquierdo = 3;
int motorderecho = 5;
int velocidadALTA = 255; //Variable de la velocidad
int velocidadBAJA = 80;


void setup() {
  Serial.begin(9600);
  pinMode(2, INPUT); //Irá conectado un PULSADOR en el pin 2
  pinMode(8, INPUT); //Irá conectado un CNY70 en el pin 8
  pinMode(9, INPUT); //Irá conectado un CNY70 en el pin 9
  pinMode(motorizquierdo, OUTPUT); //Irá conectado un MOTOR en el pin 3
  pinMode(motorderecho, OUTPUT); //Irá conectado un MOTOR en el pin 5
}


void adelante(){
    analogWrite(motorizquierdo, velocidadALTA);
    analogWrite(motorderecho, velocidadALTA);
    }
void giroderecha(){
    analogWrite(motorizquierdo, velocidadALTA);
    analogWrite(motorderecho, velocidadBAJA);
    }    
void giroizquierda(){
    analogWrite(motorizquierdo, velocidadBAJA);
    analogWrite(motorderecho, velocidadALTA);
    } 
void detenido(){
    digitalWrite(motorizquierdo, LOW);
    digitalWrite(motorderecho, LOW);
    }

void pruebasensores(){
  
    pulsador=digitalRead(2);
    sensorizquierdo=digitalRead(8);
    sensorderecho=digitalRead(9);
    
    Serial.print("Pulsador: ");
    Serial.print(pulsador);
    Serial.print("    SensorIzq: ");
    Serial.print(sensorizquierdo);
    Serial.print("    SensorDer: ");
    Serial.println(sensorderecho);
    delay(100);        // espera entre lecturas para estabilidad
    }

void pruebamotores(){

    detenido();
    delay(1000);

    adelante();
    delay(1000);

    detenido();
    delay(1000);

    giroderecha();
    delay(1000);

    detenido();
    delay(1000);

    giroizquierda();
    delay(1000);
}


void seguirlinea(){

    sensorizquierdo=digitalRead(8);
    sensorderecho=digitalRead(9);

    if (sensorizquierdo == LOW && sensorderecho == LOW)// NEGRO - NEGRO
            {
            giroizquierda();
            } 
    if (sensorizquierdo == HIGH && sensorderecho == LOW)// BLANCO - NEGRO
            {
            adelante();
            }
    if (sensorizquierdo == HIGH && sensorderecho == HIGH)// BLANCO - BLANCO
            {
           giroderecha();
            }
    if (sensorizquierdo == LOW && sensorderecho == HIGH)// NEGRO - BLANCO
           {
           giroderecha();
           }
}

void largada(){
  
    // Pulsador de largada en pin 2  
    while (digitalRead(2)==LOW){
        detenido();
        }
}

void loop() {
    //largada();          //lectura del pulsador de largada
    //seguirlinea();      //realiza el seguimiento de linea Blanca sobre fondo Negro
    //pruebamotores();    //activa los motores en forma secuencial
    pruebasensores();   //muestra el estado de los CNY70 en el Monitor Serial
}
