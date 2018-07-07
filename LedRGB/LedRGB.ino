#define Red_Pin 6
#define Green_Pin 5
#define Blue_Pin 3

void setup() {
  // put your setup code here, to run once:
  pinMode(Red_Pin, OUTPUT);
  pinMode(Green_Pin, OUTPUT);
  pinMode(Blue_Pin, OUTPUT);
}


void color(int Rojo, int Verde, int Azul){
  analogWrite(Red_Pin, Rojo);
  analogWrite(Green_Pin, Verde);
  analogWrite(Blue_Pin, Azul);
  delay(500);  
}

void loop() {
  // put your main code here, to run repeatedly:
  color(0,255,255); // Rojo
  color(255,0,255); // Verde
  color(255,255,0); // Azul
  color(0,0,255);
  color(255,0,0);
  color(0,255,0);
}
