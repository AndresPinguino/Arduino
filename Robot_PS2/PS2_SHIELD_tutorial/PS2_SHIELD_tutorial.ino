#include <PS2X_lib.h>
#include <AFMotor.h>

PS2X ps2x;
AF_DCMotor motorA(1);
AF_DCMotor motorB(2);

int error = 0; 
byte type = 0;
byte vibrate = 0;


void setup() 
{
  motorA.setSpeed(250);
  motorB.setSpeed(250);
  
  error = ps2x.config_gamepad(A3,A1,A0,A2, true, true);  //(clock, command, attention, data)
}

void loop() 
{
  int temp;
  ps2x.read_gamepad(false, vibrate); 
  
  if(ps2x.Button(PSB_PAD_UP))
  {
    temp = ps2x.Analog(PSAB_PAD_UP), DEC;    
    motorA.run(FORWARD);
    motorB.run(FORWARD);
    motorA.setSpeed(temp);
    motorB.setSpeed(temp);
  }
  
  else if(ps2x.Button(PSB_PAD_RIGHT))
  {
    temp = ps2x.Analog(PSAB_PAD_RIGHT), DEC;
    motorA.run(FORWARD);
    motorB.run(BACKWARD);
    motorA.setSpeed(temp);
    motorB.setSpeed(temp);
  }
  
  else if(ps2x.Button(PSB_PAD_LEFT))
  {
    temp = ps2x.Analog(PSAB_PAD_LEFT), DEC;
    motorA.run(BACKWARD);
    motorB.run(FORWARD);
    motorA.setSpeed(temp);
    motorB.setSpeed(temp);    
  }
  
  else if(ps2x.Button(PSB_PAD_DOWN))
  {
    temp = ps2x.Analog(PSAB_PAD_DOWN), DEC;
    motorA.run(BACKWARD);
    motorB.run(BACKWARD);
    motorA.setSpeed(temp);
    motorB.setSpeed(temp); 
  }
  
  else
  {
    motorA.run(RELEASE);
    motorB.run(RELEASE);
  }
  
  delay(50);
}
