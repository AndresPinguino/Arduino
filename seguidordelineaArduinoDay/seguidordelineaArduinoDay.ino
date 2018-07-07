//Variables:

int sensorizquierdo, sensorderecho;
int motorizquierdo = 3;
int motorderecho = 5;

// Bloque de Configuraciones

void setup() {
  pinMode(2, INPUT); //Irá conectado un PULSADOR en el pin 2
  pinMode(8, INPUT); //Irá conectado un CNY70 en el pin 8
  pinMode(9, INPUT); //Irá conectado un CNY70 en el pin 9
  pinMode(motorizquierdo, OUTPUT); //Irá conectado un MOTOR en el pin 3
  pinMode(motorderecho, OUTPUT); //Irá conectado un MOTOR en el pin 5
}

// Funciones

void adelante(){
    digitalWrite(motorizquierdo, HIGH);
    digitalWrite(motorderecho, HIGH);
    }
    
void giroderecha(){
    digitalWrite(motorizquierdo, HIGH);
    digitalWrite(motorderecho, LOW);
    } 
   
void giroizquierda(){
    digitalWrite(motorizquierdo, LOW);
    digitalWrite(motorderecho, HIGH);
    }
 
void detenido(){
    digitalWrite(motorizquierdo, LOW);
    digitalWrite(motorderecho, LOW);
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

// Programa Principal

void loop() {
    largada();          //lectura del pulsador de largada
    seguirlinea();      //realiza el seguimiento de linea Blanca sobre fondo Negro
}
