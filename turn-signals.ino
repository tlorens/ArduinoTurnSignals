#include <Adafruit_NeoPixel.h>

#define LeftSignal 8
#define RightSignal 9
#define PIN 6
#define BRIGHTNESS 255
#define NUM_PIXELS  24

Adafruit_NeoPixel strip = Adafruit_NeoPixel(NUM_PIXELS, PIN, NEO_GRB + NEO_KHZ800);

int LeftBlinkerState = 0;
int RightBlinkerState = 0;
int runLightState = 0;
int AMBER = strip.Color(255, 130, 0);
int OFF =  strip.Color(0, 0, 0);

void setup() {
  pinMode(LeftSignal, INPUT); 
  strip.begin();
  strip.setBrightness(BRIGHTNESS);
  strip.show();   
  knightRider(1, 16, 2, 0xFF0000);  
  knightRider(1, 16, 2, 0xFFFFFF);
  knightRider(1, 16, 2, 0x0000FF);    
}

void loop() {
  LeftBlinkerState = analogRead(LeftSignal);
  float LeftVoltage = LeftBlinkerState * (5.0 / 1023.0);  

  RightBlinkerState = analogRead(RightSignal);
  float RightVoltage = RightBlinkerState * (5.0 / 1023.0);  

  Serial.print(LeftVoltage);
  Serial.println();

  if (LeftVoltage > 3.0) {
    runLightState = 0;
    clearStrip();
    leftTurn();
    delay(10);
  }

  if (RightVoltage > 3.0 ) {
    runLightState = 0;
    clearStrip();
    rightTurn();
    delay(10);
  }

  if (RightVoltage < 3.0 && LeftVoltage < 3.0) {
    runLight();
  }
  delay(200);
}

void rightTurn() {
  strip.setBrightness(BRIGHTNESS);
  for(uint16_t i = NUM_PIXELS / 2; i < NUM_PIXELS; i++) {
    strip.setPixelColor(i, AMBER);
    strip.show();
    delay(10);
  }

  delay(300);
  
  for(uint16_t i = NUM_PIXELS / 2; i < NUM_PIXELS; i++) {
    strip.setPixelColor(i, OFF);
    strip.show();
    delay(10);
  }
  delay(150);
}

void leftTurn() {
  strip.setBrightness(BRIGHTNESS);
  for(int16_t i = (NUM_PIXELS-1) /2 ; i > -1; i--) {
    strip.setPixelColor(i, AMBER);
    strip.show();
    delay(10);
  }

  delay(300);

  for(int16_t i = NUM_PIXELS / 2; i > -1; i--) {
    strip.setPixelColor(i, OFF);
    strip.show();
    delay(10);
  }  
  delay(150);
}


void all(uint32_t c) {
  for(uint16_t i=0; i < strip.numPixels(); i++) {
    strip.setPixelColor(i, c);  
  }
  strip.show();
}


void runLight()
{
  strip.setBrightness(BRIGHTNESS / 12);
  if (runLightState == 0) {
    spread(15, 0xFF0000);
    runLightState = 1;
    
  }
  all(strip.Color(255, 0, 0));
  strip.show();     
}


void knightRider(uint16_t cycles, uint16_t speed, uint8_t width, uint32_t color) {
  uint32_t old_val[NUM_PIXELS]; // up to 256 lights!

  for(int i = 0; i < cycles; i++){
    for (int count = 1; count < NUM_PIXELS; count++) {
      strip.setPixelColor(count, color);
      old_val[count] = color;
      for(int x = count; x>0; x--) {
        old_val[x-1] = dimColor(old_val[x-1], width);
        strip.setPixelColor(x-1, old_val[x-1]); 
      }
      strip.show();
      delay(speed);
    }
    
    for (int count = NUM_PIXELS-1; count>=0; count--) {
      strip.setPixelColor(count, color);
      old_val[count] = color;
      for(int x = count; x<=NUM_PIXELS ;x++) {
        old_val[x-1] = dimColor(old_val[x-1], width);
        strip.setPixelColor(x+1, old_val[x+1]);
      }
      strip.show();
      delay(speed);
    }

  }
}

void clearStrip() {
  for( int i = 0; i<NUM_PIXELS; i++){
    strip.setPixelColor(i, 0x000000); 
    strip.show();
  }
}

uint32_t dimColor(uint32_t color, uint8_t width) {
   return (((color&0xFF0000)/width)&0xFF0000) + (((color&0x00FF00)/width)&0x00FF00) + (((color&0x0000FF)/width)&0x0000FF);
}

void spread(uint16_t speed, uint32_t color) {
  clearStrip();
  delay(300);
  int center = NUM_PIXELS / 2;
  for(int x = 0; x < center; x++) {
    strip.setPixelColor(center + x, color); 
    strip.setPixelColor(center + (x*-1), color); 
    strip.show();
    delay(speed);
  }    
}
