
//A code for turning on a single strip

/* FastLED RGBW Example Sketch
 *
 * Example sketch using FastLED for RGBW strips (SK6812). Includes
 * color wipes and rainbow pattern.
 *
 * Written by David Madison
 * http://partsnotincluded.com
*/

  #include "FastLED.h"
  #include "FastLED_RGBW.h"

//output pin,
//don't use pin 4 or pin 0
  #define DATA_PIN1 5
  

//number of LEDs per strip
  #define NUM_LEDS_PER_STRIP 50

//LED brightness 0-255
  const uint8_t brightness = 128;

//FastLED RGB to FastLED RGBW Conversion
  CRGBW leds[NUM_LEDS_PER_STRIP];
  CRGB *ledsRGB = (CRGB *) &leds[0];
    

  void setup() {
               //tell FastLED there's a strip on DATAPIN1 which has 144 LEDs
                 FastLED.addLeds<SK6812, DATA_PIN1>(ledsRGB , getRGBWsize(NUM_LEDS_PER_STRIP));
                
               //setting brightness and turning the strips on
                 FastLED.setBrightness(brightness);
                 
  }

  void loop() {
       
                colorFill(CRGB:: Red);   

  }

//refrence functions 
/*

  colorFill(CRGB::Red);
  colorFill(CRGB::Green);
  colorFill(CRGB::Blue);
  fillWhite();
  rainbowLoop(); 

*/

//soild color by calling color's name eg. Red, Blue, Black
  void colorFill(CRGB c){
       for(int i = 0; i < NUM_LEDS_PER_STRIP; i++){
           leds[i] = c;
           
       }
       FastLED.show();
  }

//white only 
  void fillWhite(){
       for(int i = 0; i < NUM_LEDS_PER_STRIP; i++){
           leds[i] = CRGBW(0, 0, 0, 255);
           
       }
       FastLED.show();
  }

//rainbow 
  void rainbow(){
       static uint8_t hue;
       for(int i = 0; i <NUM_LEDS_PER_STRIP; i++){
           leds[i] = CHSV((i * 256 / NUM_LEDS_PER_STRIP) + hue, 255, 255);
       }
       FastLED.show();
       hue++;
  }

//rainbow loop
  void rainbowLoop(){
       long millisIn = millis();
       long loopTime = 5000; // 5 seconds
       while(millis() < millisIn + loopTime){
             rainbow();
             delay(5);
       }
  }
