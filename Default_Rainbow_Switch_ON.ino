

//Default rainbow setting both ceiling and wall, switched on/off from outlet switch

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

//output pins, 32 pins are needed
//don't use pin 4 or pin 0
  #define DATA_PIN2 2
  #define DATA_PIN3 3
  #define DATA_PIN4 9
  #define DATA_PIN5 5
  #define DATA_PIN6 32
  #define DATA_PIN7 7
  #define DATA_PIN8 8
  #define DATA_PIN9 10
  #define DATA_PIN10 11
  #define DATA_PIN11 12
  #define DATA_PIN12 13
  #define DATA_PIN13 22
  #define DATA_PIN14 28
  #define DATA_PIN15 24
  #define DATA_PIN16 30
  #define DATA_PIN17 1
  #define DATA_PIN18 16
  #define DATA_PIN19 14
  #define DATA_PIN20 18
  #define DATA_PIN21 17
  #define DATA_PIN22 19
  #define DATA_PIN23 15
  #define DATA_PIN24 20
  #define DATA_PIN25 21
  #define DATA_PIN26 23
  #define DATA_PIN27 37
  #define DATA_PIN28 26
  #define DATA_PIN29 29
  #define DATA_PIN30 33
  #define DATA_PIN31 34
  #define DATA_PIN32 35
  #define DATA_PIN33 36

//number of LEDs per strip
 #define NUM_LEDS_PER_STRIP 144

//LED brightness 0-255
  const uint8_t brightness = 255;

//FastLED RGB to FastLED RGBW Conversion
  CRGBW leds[NUM_LEDS_PER_STRIP];
  CRGB *ledsRGB = (CRGB *) &leds[0];
    

  void setup() {
               //tell FastLED there're 32 strips on DATAPIN2-DATAPIN33, each has 144 LEDs/strip
                 FastLED.addLeds<SK6812, DATA_PIN2>(ledsRGB , getRGBWsize(NUM_LEDS_PER_STRIP));
                 FastLED.addLeds<SK6812, DATA_PIN3>(ledsRGB,getRGBWsize(NUM_LEDS_PER_STRIP));
                 FastLED.addLeds<SK6812, DATA_PIN4>(ledsRGB, getRGBWsize(NUM_LEDS_PER_STRIP));
                 FastLED.addLeds<SK6812, DATA_PIN5>(ledsRGB, getRGBWsize(NUM_LEDS_PER_STRIP));
                 FastLED.addLeds<SK6812, DATA_PIN6>(ledsRGB, getRGBWsize(NUM_LEDS_PER_STRIP));
                 FastLED.addLeds<SK6812, DATA_PIN7>(ledsRGB, getRGBWsize(NUM_LEDS_PER_STRIP));
                 FastLED.addLeds<SK6812, DATA_PIN8>(ledsRGB, getRGBWsize(NUM_LEDS_PER_STRIP));
                 FastLED.addLeds<SK6812, DATA_PIN9>(ledsRGB, getRGBWsize(NUM_LEDS_PER_STRIP));
                 FastLED.addLeds<SK6812, DATA_PIN10>(ledsRGB, getRGBWsize(NUM_LEDS_PER_STRIP));
                 FastLED.addLeds<SK6812, DATA_PIN11>(ledsRGB, getRGBWsize(NUM_LEDS_PER_STRIP));
                 FastLED.addLeds<SK6812, DATA_PIN12>(ledsRGB, getRGBWsize(NUM_LEDS_PER_STRIP));
                 FastLED.addLeds<SK6812, DATA_PIN13>(ledsRGB, getRGBWsize(NUM_LEDS_PER_STRIP));
                 FastLED.addLeds<SK6812, DATA_PIN14>(ledsRGB, getRGBWsize(NUM_LEDS_PER_STRIP));
                 FastLED.addLeds<SK6812, DATA_PIN15>(ledsRGB, getRGBWsize(NUM_LEDS_PER_STRIP));
                 FastLED.addLeds<SK6812, DATA_PIN16>(ledsRGB, getRGBWsize(NUM_LEDS_PER_STRIP));
                 FastLED.addLeds<SK6812, DATA_PIN17>(ledsRGB, getRGBWsize(NUM_LEDS_PER_STRIP));
                 FastLED.addLeds<SK6812, DATA_PIN18>(ledsRGB, getRGBWsize(NUM_LEDS_PER_STRIP));
                 FastLED.addLeds<SK6812, DATA_PIN19>(ledsRGB, getRGBWsize(NUM_LEDS_PER_STRIP));
                 FastLED.addLeds<SK6812, DATA_PIN20>(ledsRGB, getRGBWsize(NUM_LEDS_PER_STRIP));
                 FastLED.addLeds<SK6812, DATA_PIN21>(ledsRGB, getRGBWsize(NUM_LEDS_PER_STRIP));
                 FastLED.addLeds<SK6812, DATA_PIN22>(ledsRGB, getRGBWsize(NUM_LEDS_PER_STRIP));
                 FastLED.addLeds<SK6812, DATA_PIN23>(ledsRGB, getRGBWsize(NUM_LEDS_PER_STRIP));
                 FastLED.addLeds<SK6812, DATA_PIN24>(ledsRGB, getRGBWsize(NUM_LEDS_PER_STRIP));
                 FastLED.addLeds<SK6812, DATA_PIN25>(ledsRGB, getRGBWsize(NUM_LEDS_PER_STRIP));
                 FastLED.addLeds<SK6812, DATA_PIN26>(ledsRGB, getRGBWsize(NUM_LEDS_PER_STRIP));
                 FastLED.addLeds<SK6812, DATA_PIN27>(ledsRGB, getRGBWsize(NUM_LEDS_PER_STRIP));
                 FastLED.addLeds<SK6812, DATA_PIN28>(ledsRGB, getRGBWsize(NUM_LEDS_PER_STRIP));
                 FastLED.addLeds<SK6812, DATA_PIN29>(ledsRGB, getRGBWsize(NUM_LEDS_PER_STRIP));
                 FastLED.addLeds<SK6812, DATA_PIN30>(ledsRGB, getRGBWsize(NUM_LEDS_PER_STRIP));
                 FastLED.addLeds<SK6812, DATA_PIN31>(ledsRGB, getRGBWsize(NUM_LEDS_PER_STRIP));
                 FastLED.addLeds<SK6812, DATA_PIN32>(ledsRGB, getRGBWsize(NUM_LEDS_PER_STRIP));
                 FastLED.addLeds<SK6812, DATA_PIN33>(ledsRGB, getRGBWsize(NUM_LEDS_PER_STRIP));

               //setting brightness and turning the strips on
                 FastLED.setBrightness(brightness);
  }
                  

  void loop() {
       
                rainbowLoop();
  }

   
//refrence functions, updated 24.01.2020
/*

  colorFill(CRGB::Red);
  colorFill(CRGB::Green);
  colorFill(CRGB::Blue);
  fillWhite();
  colorRGBW();
  rainbowLoop(); 

*/

//soild color by calling color's name eg. Red, Blue, Black
  void colorFill(CRGB c){
       for(int i = 0; i < NUM_LEDS_PER_STRIP; i++){
           leds[i] = c;
       }
       FastLED.show();
  }



//choosing color based on RGBW values 0-255
  void colorRGBW(){
       for(int i = 0; i < NUM_LEDS_PER_STRIP; i++){
           leds[i] = CRGBW(150, 200, 98, 10);
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
       long loopTime = 5000000; // 5000 seconds
       while(millis() < millisIn + loopTime){
             rainbow();
            // delay(5);
       }
  }
