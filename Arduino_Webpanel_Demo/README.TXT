1. Download the Arduino websocket library from https://github.com/ejeklint/ArduinoWebsocketServer (Thanks, Per Ejecklint!)
2. Copy the ArduinoWebsocketServer folder to your Arduino/libraries folder.
3. Copy the Arduino_Webpanel_Demo folder to your Arduino sketchbook folder.
4. Open the Arduino IDE (1.0.5 or later recommended) and open the Arduino_Webpanel_Demo.ino sketch.
5. Edit the "byte ip[] = { 192, 168, 1 , 77 };" line of code in the sketch to set the IP address you want to use, or leave it to use the default
6. Compile and load the sketch to your controller and give it a few moments to get started after rebooting.
7. Go to the Arduino_Webpanel_Demo folder and open the webpanel_arduino.js folder with a text editor (Notepad++ recommended)
8. Edit the line "var wsURL="ws://192.168.1.77/";" to match the IP you selected in step 5, or leave it to use the default.
9. In the Arduino_Webpanel_Demo folder, right-click the WebPanel.svg document and select Open with -> Google Chrome or Mozilla Firefox from the menu.
10. If all goes well, the webpanel script will connect to the Arduino websocket server and begin streaming data to the SVG document objects.
11. If you have trouble with the socket closing unexpectedly, try slowing down the Arduino program by changing the "delay(10);" line of code to delay(20) or more.
12. Have fun!