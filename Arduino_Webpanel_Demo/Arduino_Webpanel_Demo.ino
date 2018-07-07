#include <SPI.h>
#include <Ethernet.h>
#include <stdlib.h>


// Enabe debug tracing to Serial port.
//#define DEBUG

// Here we define a maximum framelength to 64 bytes. Default is 256.
#define MAX_FRAME_LENGTH 64

#include <WebSocket.h>

byte mac[] = { 0x52, 0x4F, 0x43, 0x4B, 0x45, 0x54 };
byte ip[] = { 192, 168, 1 , 77 };

// Create a Websocket server
WebSocket wsServer;

void onConnect(WebSocket &socket) {
#ifdef DEBUG
  Serial.println("onConnect called");
#endif
}

// You must have at least one function with the following signature.
// It will be called by the server when a data frame is received.
void onData(WebSocket &socket, char* dataString, byte frameLength) {
  
#ifdef DEBUG
  Serial.print("Got data: ");
  Serial.write((unsigned char*)dataString, frameLength);
  Serial.println();
#endif
  
  // Just echo back data for fun.
  socket.send(dataString, strlen(dataString));
  //digitalWrite(7, atoi(dataString));
}

void onDisconnect(WebSocket &socket) {
#ifdef DEBUG
  Serial.println("onDisconnect called");
#endif
}

void setup() {
//#ifdef DEBUG  
  Serial.begin(57600);
//#endif
  Ethernet.begin(mac, ip);
  
  wsServer.registerConnectCallback(&onConnect);
  wsServer.registerDataCallback(&onData);
  wsServer.registerDisconnectCallback(&onDisconnect);  
  wsServer.begin();
  
  delay(100); // Give Ethernet time to get ready
  
   pinMode(7, OUTPUT);      //LED on thia pin
   digitalWrite(7, LOW);    //initialize LED off
   delay(1000); //for debug
   pinMode(8, INPUT);       //Button on this pin
}

void loop() {
  // Should be called for each loop.
  wsServer.listen();
  
  // Read Inputs
  String TempSensor = String(analogRead(A0));  //Temperature Sensor
  String LightSensor = String(analogRead(A1));  //Photo Sensor
  String Potentiometer = String(analogRead(A2));  //Rotary Pot
  String Button = String(digitalRead(8));
  // digitalWrite(7, HIGH);
 
  //assemble the websocket outgoing message
  String wsMessage = "wpGraph,Arduino," + TempSensor + "," + LightSensor + "," + Potentiometer +  "," + Button;
  int MsgBufLen = wsMessage.length()+1;
  char MsgBuf[MsgBufLen];
  wsMessage.toCharArray(MsgBuf, MsgBufLen);
  delay(10);
  if (wsServer.isConnected()) {
    wsServer.send(MsgBuf, byte(MsgBufLen));
  }
}
