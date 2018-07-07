#include <AFMotor.h>

AF_DCMotor Motor1(1);
AF_DCMotor Motor2(2);
AF_DCMotor Motor3(3);
AF_DCMotor Motor4(4);

int serial;
int Vel = 255;
int tiempo = 500;

void setup(){     
         Serial.begin(9600);           // Bluetooth a 9600 bps
         Paro();
         delay(1000);
   }


void SetVel(int v1, int v2, int v3, int v4)
{
     Motor1.setSpeed(v1);
     Motor2.setSpeed(v2);
     Motor3.setSpeed(v3);
     Motor4.setSpeed(v4);         
}

void Retroceso()
{ 
  SetVel(Vel,Vel,Vel,Vel);
  Motor1.run(FORWARD) ;
  Motor2.run(FORWARD);
  Motor3.run(FORWARD);
  Motor4.run(FORWARD); 
}

void Avance()
{ 
  SetVel(Vel,Vel,Vel,Vel);
  Motor1.run(BACKWARD) ;
  Motor2.run(BACKWARD);
  Motor3.run(BACKWARD);
  Motor4.run(BACKWARD);
}


void Paro()
{ 
  Motor1.run(RELEASE);
  Motor2.run(RELEASE);
  Motor3.run(RELEASE);
  Motor4.run(RELEASE); 
}

void giroIzquierda()
{  
    
    SetVel(Vel,Vel,Vel,Vel);   
    Motor1.run(FORWARD) ;
    Motor2.run(FORWARD);
    Motor3.run(BACKWARD);
    Motor4.run(BACKWARD);   
}
         
void giroDerecha()
{  
    
    SetVel(Vel,Vel,Vel,Vel);   
    Motor1.run(BACKWARD) ;
    Motor2.run(BACKWARD);
    Motor3.run(FORWARD);
    Motor4.run(FORWARD);    
}



void loop(){
  
  if(Serial.available()){
      serial=(Serial.read());
      }
  
    if(serial==2){
      Paro();
    }
    
    if(serial==1){//ADELANTE
      Avance();
    }
    if(serial==5){//ATRAS
      Retroceso();
    }
    if(serial==3){//IZQUIERDA
      giroIzquierda();
    }
    if(serial==7){//DERECHA
      giroDerecha();
    }

    //Con Letras

      if(serial=='p' || serial=='P'){
      Paro();
    }
    
    if(serial=='a' || serial=='A'){//ADELANTE
      Avance();
      delay(tiempo);
      Paro();
    }
    if(serial=='r' || serial=='R'){//ATRAS
      Retroceso();
      delay(tiempo);
      Paro();
    }
    if(serial=='i' || serial=='I'){//IZQUIERDA
      giroIzquierda();
      delay(tiempo);
      Paro();
    }
    if(serial=='d' || serial=='D'){//DERECHA
      giroDerecha();
      delay(tiempo);
      Paro();
    }
    
  delay(90);
}
