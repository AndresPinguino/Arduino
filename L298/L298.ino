
// Conectar lineas de control a los pines digitales del Arduino

// motor A
int enA = 10;
int in1 = 12;
int in2 = 11;

// motor B
int enB = 9;
int in3 = 8;
int in4 = 7;


void setup()
{
  // configurar las lineas de control como salidas
  pinMode(enA, OUTPUT);
  pinMode(enB, OUTPUT);
  pinMode(in1, OUTPUT);
  pinMode(in2, OUTPUT);
  pinMode(in3, OUTPUT);
  pinMode(in4, OUTPUT);
  detenido();
}

void adelante(int velocidad)
{
  // Encender motor A
  digitalWrite(in1, HIGH);
  digitalWrite(in2, LOW);
  analogWrite(enA, velocidad);
  // Encender motor B
  digitalWrite(in3, HIGH);
  digitalWrite(in4, LOW);
  analogWrite(enB, velocidad);
}


void atras(int velocidad)
{
  // Encender motor A
  digitalWrite(in1, LOW);
  digitalWrite(in2, HIGH);
  analogWrite(enA, velocidad);
  // Encender motor B
  digitalWrite(in3, LOW);
  digitalWrite(in4, HIGH);
  analogWrite(enB, velocidad);
}

void detenido()
{
  // Encender motor A
  digitalWrite(in1, LOW);
  digitalWrite(in2, LOW);
  analogWrite(enA, 0);
  // Encender motor B
  digitalWrite(in3, LOW);
  digitalWrite(in4, LOW);
  analogWrite(enB, 0);
}


void giroizquierda(int velocidad)
{
  // Encender motor A
  digitalWrite(in1, LOW);
  digitalWrite(in2, HIGH);
  analogWrite(enA, velocidad);
  // Encender motor B
  digitalWrite(in3, HIGH);
  digitalWrite(in4, LOW);
  analogWrite(enB, velocidad);
}


void giroderecha(int velocidad)
{
  // Encender motor A
  digitalWrite(in1, HIGH);
  digitalWrite(in2, LOW);
  analogWrite(enA, velocidad);
  // Encender motor B
  digitalWrite(in3, LOW);
  digitalWrite(in4, HIGH);
  analogWrite(enB, velocidad);
}



void loop() {
  detenido();
  delay(500);
  adelante(200);
  delay(2000);
  detenido();
  delay(500);
  atras(100);
  delay(2000);
  detenido();
  delay(500);
  giroderecha(255);
  delay(2000);
  detenido();
  delay(500);
  giroizquierda(100);
  delay(2000);
}
