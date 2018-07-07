#include "PinChangeInt.h"  // PinChange Interrupt library
#include "FastLED.h"  // FastLED library

// WS2812 RGB Stick connection
#define led_pin 5  // RGB Stick DI pin connected to UNO pin 5
#define NUM_LEDS 8  // # of WS2812 LEDs on stick
CRGB leds[NUM_LEDS];  // FastLED Library Init

// Rotary Encoder Module connections
#define PinCLK 2   // Rotary CLK pin connected to UNO interrupt 0 on pin 2
#define PinDT 4   // Rotary DT pin connected to UNO pin 4
#define PinCLK_pin11 11 // Rotary CLK pin connected to UNO normal pin 11

volatile boolean led_changed; // to detect that led need to be changed
volatile int led_to_light=0;      // number of led to light up

// Interrupt routine on Pin 2 (Interrupt zero)runs if CLK pin changes state 
void rotarydetect_pin2 ()  {
delay(1);  // delay for Debouncing Rotary Encoder

if (digitalRead(PinCLK)) {
if (digitalRead(PinDT)) {
if (led_to_light > 1){
led_to_light=led_to_light-1; }}
if (!digitalRead(PinDT)) {
if (led_to_light < 9){ led_to_light=led_to_light+1; }} led_changed = true; } } // Interrupt routine on Pin 11 runs if CLK pin changes state void rotarydetect_pin11 ()  { delay(1);  // delay for Debouncing Rotary Encoder if (digitalRead(PinCLK_pin11)) { if (digitalRead(PinDT)) { if (led_to_light > 1){
led_to_light=led_to_light-1; }}
if (!digitalRead(PinDT)) {
if (led_to_light < 9){
led_to_light=led_to_light+1; }}
led_changed = true;
}
}

void setup ()  {
FastLED.addLeds<NEOPIXEL,led_pin>(leds, NUM_LEDS);  // Setup FastLED Library
FastLED.clear();
FastLED.show();

pinMode(PinCLK,INPUT);  // Set Pin 2 (Interrupt zero) to Input
pinMode(PinCLK_pin11, INPUT);  // Set normal 8 pin to Input
pinMode(PinDT,INPUT);  

attachInterrupt (0, rotarydetect_pin2, CHANGE); // interrupt 0 always connected to pin 2 on Arduino UNO
attachPinChangeInterrupt(PinCLK_pin11, rotarydetect_pin11, CHANGE);  // interrupt connected to pin 11
}

void loop ()  {
// Runs if Rotary Encoder rotation is detected
if (led_changed) {
led_changed = false;  // do Not repeat IF loop until new rotation detected

// Which LED's to light up
FastLED.clear();
for(int x = 0; x != (led_to_light - 1); x++) {
if (x < 2) leds[x] = CRGB::Red;       if (x > 1 & x < 5) leds[x] = CRGB::Orange;       if (x > 4) leds[x] = CRGB::Green; }
FastLED.setBrightness(50);
FastLED.show();
}
}
