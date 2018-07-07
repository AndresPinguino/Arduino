#include <Servo.h>
//Para probar servos usar fuente externa de 5V ó cargador USB, sino se reinicia el arduino alimentando desde el usb de la notebook.
Servo myservo;  // create servo object to control a servo
int pos;
int flag;

void setup() {
  myservo.attach(0);  // attaches the servo on pin 0 to the servo object
  myservo.write(0);              // tell servo to go to position in variable 'pos'
  delay(15);
  flag=1;
}


void soltar_pala(){
  for (pos = 0; pos <= 90; pos += 1) { // goes from 0 degrees to 180 degrees
    // in steps of 1 degree
    myservo.write(pos);              // tell servo to go to position in variable 'pos'
    delay(10);                       // waits 15ms for the servo to reach the position
    }
    myservo.detach();              //libera el pin del servo y habilita pines 9 y 10 para usar PWM 
    delay(10); 
  }


void loop() {
if (flag==1)
{
  soltar_pala();
  flag=0;
}
}
