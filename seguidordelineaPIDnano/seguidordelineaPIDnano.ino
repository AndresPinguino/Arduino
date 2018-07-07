// http://samvrit.tk/tutorials/pid-control-arduino-line-follower-robot/

float Kp=20,Ki=0,Kd=0;
float error=0, P=0, I=0, D=0, PID_value=0;
float previous_error=0, previous_I=0;
int sensor[2]={0, 0};
int initial_motor_speed=100;

void setup()
{
 pinMode(2,INPUT); // pulsador
 pinMode(8,INPUT); // sensor izquierdo
 pinMode(9,INPUT); // sensor derecho

 pinMode(3,OUTPUT); //PWM Motor izquierdo
 pinMode(5,OUTPUT); //PWM Motor derecho
 //Serial.begin(9600); //Habilitar Comunicacion Serial

}


void read_sensor_values()
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
}

void calculate_pid()
{
    P = error;
    I = previous_I + error;
    D = error-previous_error;
    
    PID_value = (Kp*P) + (Ki*I) + (Kd*D);
    
    previous_I=I;
    previous_error=error;
}

void motor_control()
{
    // Calculating the effective motor speed:
    int left_motor_speed = initial_motor_speed+PID_value;
    int right_motor_speed = initial_motor_speed-PID_value;
    
    // The motor speed should not exceed the max PWM value
    constrain(left_motor_speed,0,255);
    constrain(right_motor_speed,0,255);
    /* 
    Serial.print(error);
    Serial.print("      ");
    Serial.print(left_motor_speed);
    Serial.print("      ");
    Serial.println(right_motor_speed);
    */

    analogWrite(3,left_motor_speed);   //Left Motor Speed
    analogWrite(5,right_motor_speed);  //Right Motor Speed
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
    read_sensor_values();
    calculate_pid();
    motor_control();
}
