/********************************************************
 * PID Basic Example
 * Reading analog input 0 to control analog PWM output 3
 ********************************************************/

#include <PID_v1.h>

int sensor[2]={0, 0};
int error;

//Define Variables we'll be connecting to
double Setpoint, Input, Output, MotorIzquierdo, MotorDerecho;

//Specify the links and initial tuning parameters
double Kp=20, Ki=0, Kd=0;
PID myPID(&Input, &Output, &Setpoint, Kp, Ki, Kd, DIRECT);

// Velocidad general
int velocidad=100;

void setup()
{
  pinMode(2,INPUT); // pulsador
  pinMode(8,INPUT); // sensor izquierdo
  pinMode(9,INPUT); // sensor derecho

  pinMode(3,OUTPUT); //PWM Motor izquierdo
  pinMode(5,OUTPUT); //PWM Motor derecho
  Serial.begin(9600); //Enable Serial Communications
  
  //initialize the variables we're linked to
  Input = 1;
  Setpoint = 0;

  //turn the PID on
  myPID.SetMode(AUTOMATIC);
}

int read_sensor_values()
{
  sensor[0]=digitalRead(8); // sensor izquierdo
  sensor[1]=digitalRead(9); // sensor derecho
  
  if((sensor[0]==LOW)&&(sensor[1]==HIGH))
  error=1;  // Posicion a la izquierda de la linea
  else if((sensor[0]==HIGH)&&(sensor[1]==HIGH))
  error=0;  // Posicion centrado
  else if((sensor[0]==HIGH)&&(sensor[1]==LOW))
  error=-1; // Posicion a la derecha de la linea
  else if((sensor[0]==LOW)&&(sensor[1]==LOW))   // Posicion fuera de linea totalmente y recuerda para que lado estaba anteriormente
    {
    if(error==-1) 
        error=-2;   // Muy a la derecha
    else if (error==1)
        error=2;    // Muy a la izquierda
    }
    return error;
}

void largada(){
  
    // Pulsador de largada en pin 2  
    while (digitalRead(2)==LOW){
            digitalWrite(3,LOW);   // Motor izquierdo apagado
            digitalWrite(5,LOW);   // Motor derecho apagado
        }
}

void loop()
{
  largada();
  Input = read_sensor_values();
  myPID.Compute();
  
  MotorIzquierdo=velocidad+Output;
  MotorDerecho=velocidad-Output;
  
  Serial.print(Input);
  Serial.print("    ");
  Serial.print(MotorIzquierdo);
  Serial.print("    ");
  Serial.println(MotorDerecho);
  //analogWrite(PIN_OUTPUT, Output);
}
