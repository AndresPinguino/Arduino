
#define Pin_A 2
#define Pin_B 4
#define Pin_C 7
#define Pin_D 8

void setup() {
  // put your setup code here, to run once:
  pinMode(Pin_A, OUTPUT);
  pinMode(Pin_B, OUTPUT);
  pinMode(Pin_C, OUTPUT);
  pinMode(Pin_D, OUTPUT);
  secuencial(0,0,0,0);
}


void secuencial(int Estado_A, int Estado_B, int Estado_C, int Estado_D){
  digitalWrite(Pin_A, Estado_A);
  digitalWrite(Pin_B, Estado_B);
  digitalWrite(Pin_C, Estado_C);
  digitalWrite(Pin_D, Estado_D);
  delay(100);  
}

void loop() {
  // put your main code here, to run repeatedly:
  secuencial(1,0,0,0);
  secuencial(0,1,0,0);
  secuencial(0,0,1,0);
  secuencial(0,0,0,1); 

  secuencial(0,0,0,0);
  secuencial(1,0,0,0);
  secuencial(1,1,0,0);
  secuencial(1,1,1,0);
  secuencial(1,1,1,1); 
}
