#include <FastLED.h>


#define LED_PIN     11
#define NUM_LEDS    8*24
#define BRIGHTNESS  64
#define LED_TYPE    WS2812B
#define COLOR_ORDER GRB
#define UPDATES_PER_SECOND 100
CRGBArray<NUM_LEDS> leds;
void setup() {
    millidel( 2000 ); //  power-up safety millidel
    FastLED.addLeds<LED_TYPE, LED_PIN, COLOR_ORDER>(leds, NUM_LEDS).setCorrection( TypicalLEDStrip );
    FastLED.setBrightness(  BRIGHTNESS );
    Serial.begin(9600);

    fillLeds(255,255,255);
    millidel(100);
    fadeAll();
}


uint8_t RControl = 0;
uint8_t GControl = 0;
uint8_t BControl = 0;
int flashRateControl = 0; //  0 means no flashing, btw.
int flashCountControl = 0;

unsigned long flashingTimer;
bool LEDsFlashingOn = false;
int flashCounter = 0;

void loop() {
    if (Serial.available()) {
      int x = Serial.read();
      // Serial.print(x);
      handleRequest(x);
    }
    if ( flashRateControl != 0 ) {
      if ( millis() > flashingTimer ) {
        flashingTimer = millis() + flashRateControl;
  
        if ( LEDsFlashingOn ) {
          fillLeds( 0 , 0 , 0 );
        } else {
          if ( flashCounter++ == flashCountControl ) {
            setLeds( 0 , 0 , 0 , 0 );
          } else {
            fillLeds( RControl , GControl , BControl );
          }
        }
        LEDsFlashingOn = !LEDsFlashingOn;
      }
    } else {
      fillLeds( RControl , GControl , BControl );
    }
    FastLED.show();
    FastLED.delay(1000 / UPDATES_PER_SECOND);
}

void handleRequest(int x){
  switch ( x ) {
        case (int)'-': // All off
          setLeds( 0 , 0 , 0 , 0 );
          break;
        case 48: // Power On
          setLeds( 255 , 255 , 255 , 0 );
          break;
        case 49: // Disabled
          setLeds( 255 , 255 , 0 , 0 );
          break;
        case 50: // Auto Enabled
          setLeds( 0 , 255 , 0 , 0 );
          break;
        case 51: // Auto Disabled
          setLeds( 0 , 0 , 0 , 0 );
          break;
        case 52: // Telop Enabled
          setLeds( 0 , 0 , 255 , 0 );
          break;
        case 53: // Telop Disable
          setLeds( 0 , 0 , 0 , 0 );
          break;
        case 54: // Test enable
          setLeds( 255 , 0 , 0 , 0 );
          break;
        case 55: // Test disable
          setLeds( 0 , 0 , 0 , 0 );
          break;
        case 56: // Emergency stop
          fillLeds( 255 , 0 , 0 );
          millidel(1000);
          fadeAll();
          setLeds( 0 , 0 , 0 , 0 );
          break;
        case 57: // Battery Critical
          /*FastLED.setBrightness(10);
          cautionStripes(CRGB::Brown);*/
          setLeds( 255 , 150 , 150 , 0 );
          break;
        case 97: // Signal Hatch
          setLeds( 255 , 255 , 0 , 250 );
          break;
        case 98: // Signal Cargo
          setLeds( 255 , 0 , 0 , 250 );
          break;
        case 99: // Compressor Full
          fillLeds(255, 0, 255);
          millidel(100);
          fadeAll();
          setLeds( 0 , 0 , 0 , 0 );
          break;
        case 100: // Connection Fault
          setLeds( 255 , 0 , 255 , 750 );
          break;
        case 101: // Connection Made
          setLeds( 0 , 255 , 0 , 750 );
        default:
          setLeds( 0 , 0 , 0 , 0 );
          break;
  }
}

void setLeds ( uint8_t r , uint8_t g , uint8_t b , int _flashRate ) {
  // set controls:
  RControl = r;
  GControl = g;
  BControl = b;
  flashRateControl = _flashRate;

  // set lesser-used controls to their default values:
  flashCountControl = 5;

  // reset other variables:
  flashCounter = 0;
  flashingTimer = 0;
  LEDsFlashingOn = false;
}

void fillLeds(int r, int g, int b){
  for(int i = 0; i < NUM_LEDS; i++){
    leds[i] = CRGB(r, g, b);
  }
  FastLED.show();
}
void fadeAll() {
  for(int i = 0; i < 100; i++){
    leds.fadeToBlackBy(10);
    FastLED.delay(1000/UPDATES_PER_SECOND);
    FastLED.show();
    
  }
}
void cautionStripes(CRGB color){
  for(int i = 0; i < NUM_LEDS; i++){
    if(i % 2 == 0){
      leds[i] = color;
    }else{
      leds[i] = CRGB::Black;
    }
  }
}
void flash(CRGB color, int count, int del){
  for(int i = 0; i < count; i++){
    if(Serial.available()){
      fillLeds(0,0,0);
      break;
    }
    fillLeds(color.r, color.g, color.b);
    millidel(del);
    fillLeds(0,0,0);
    millidel(del);
  }
}
void millidel(int del){
  int start = millis();
  while(millis() <= start + del){
    if(Serial.available()){
      break;
    }
  }
}
