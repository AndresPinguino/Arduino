#include <Time.h>

int relay = 13;

void setup()
  {   Serial.begin(9600);
      setTime(21,20,00,3,2,2016);   // formato hora:min:seg:dia:mes:año
      pinMode(relay, OUTPUT);
      digitalWrite(relay, LOW);
  }
void loop()
  {   
      time_t t = now();
      /*
      Serial.print(day(t));
      Serial.print(+ "/") ;
      Serial.print(month(t));
      Serial.print(+ "/") ;
      Serial.print(year(t)); 
      Serial.print( " ") ;
      
      Serial.print(hour(t));  
      Serial.print(+ ":") ;
      Serial.print(minute(t));
      Serial.print(":") ;
      Serial.println(second(t));
      */

      if (hour(t) < 7 || hour(t) >= 21)
      {
      //Serial.println(" Luces prendidas ");
      digitalWrite(relay, HIGH);  
      } 
      else
      {
      //Serial.println(" Luces apagadas ");
      digitalWrite(relay, LOW);        
      }

      delay(30*60000);   // consulta cada media hora
 }
