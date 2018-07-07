/************************* http://geekfactory.mx *******************************
 * 
 * Sketch de prueba para modulos de bluetooth HC-05 y HC-06. Este programa puede
 * controlar el led de la tarjeta arduino a través de la conexión bluetooth. El
 * programa puede ampliarse para controlar una mayor cantiad de salidas.
 * 
 * ESTE SKETCH USA EL PUERTO SERIE (UART), MISMO QUE ES UTILIZADO PARA
 * LA COMUNICACIÓN USB. EL USO DE BLUETOOTH O USB ES MUTUAMENTE EXCLUSIVO
 * (NO SE PUEDEN USAR AMBOS AL MISMO TIEMPO).
 * 
 * PARA USAR LA CONEXIÓN USB Y BLUETOOTH AL MISMO TIEMPO ES NECESARIO EMULAR UNA
 * UART POR SOFTWARE.
 * 
 * El funcionamiento es sencillo:
 * 
 * Enviar un caracter 'A' para encender el led
 * Enviar un caracter 'a' para apagar el led
 * 
 */

char rxChar;    // Variable para recibir datos del puerto serie
int ledpin = 6;  // Pin donde se encuentra conectado el led (pin 13)

// Configurar el arduino
void setup()
{
  // Pin 6 como salida
  pinMode(ledpin, OUTPUT);
  // Comunicación serie a 9600 baudios
  Serial.begin(9600);
}

// Ciclo infinito, programa principal
void loop()
{
  // Si hay datos disponibles en el buffer
  if( Serial.available() )
  {
    // Leer un byte y colocarlo en variable
    rxChar = Serial.read();

    // Procesar comando de un solo byte
    if( rxChar == 'a' )
    {
      digitalWrite(ledpin, HIGH);
      Serial.println("ON");
    }
    else if ( rxChar == 'b' )
    { 
      digitalWrite(ledpin, LOW);
      Serial.println("OFF");
    }
  }

  // Podemos hacer otras cosas aquí
  delay(100);
}
