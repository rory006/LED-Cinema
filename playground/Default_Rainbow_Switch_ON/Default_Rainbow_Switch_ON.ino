

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


/*Wall door:
left to right, top to bottom (facing door)
   |4As      |3De
-------------------4B
   |4Ae      |3Ds
-------------------3C
   |4C       |3B
-------------------3A
   |4D       |2D
-------------------2C
   |4E       |2B
-------------------1C
   |1Ae      |2Ae
-------------------1B
   |1As      |2As
*/

/*Ceiling door:
left to right, top to bottom (facing wall door)
   |         |
-------------------
   |         |
-------------------
   |         |
-------------------
   |         |
-------------------
   |         |
-------------------
   |         |
-------------------
   |         |
*/

//output pins, 32 pins are needed
//don't use pin 4 or pin 0
#define DATA_PIN0 0 //4B
#define DATA_PIN1 1 //4A
#define DATA_PIN2 2 //4E
#define DATA_PIN3 3 //3D
#define DATA_PIN4 4 //3C
#define DATA_PIN5 5 //3B
#define DATA_PIN6 6 //3A
#define DATA_PIN7 7 //2B
#define DATA_PIN8 8 //2D
#define DATA_PIN9 9 //2C
#define DATA_PIN10 10 //2A
#define DATA_PIN11 11 //1A
#define DATA_PIN12 12 //1C
#define DATA_PIN13 13 //1B
#define DATA_PIN14 14 //4C
#define DATA_PIN15 15 //4D
//#define DATA_PIN16 16
#define DATA_PIN17 17 //6?
#define DATA_PIN18 18 //6?
#define DATA_PIN19 19 //6?
#define DATA_PIN20 20 //3A
#define DATA_PIN21 21 //5B
#define DATA_PIN22 22 //3B
#define DATA_PIN23 23 //5A
#define DATA_PIN24 24 //5C
#define DATA_PIN25 25 //9C
#define DATA_PIN26 26 //7A
#define DATA_PIN27 27 //7B
#define DATA_PIN28 28 //7C
#define DATA_PIN29 29 //8A
#define DATA_PIN30 30 //8B
#define DATA_PIN31 31 //8C
#define DATA_PIN32 32 //8D
//#define DATA_PIN33 33

//number of LEDs per strip
#define NUM_LEDS_PER_STRIP 144

//LED brightness 0-255
const uint8_t brightness = 255;

//FastLED RGB to FastLED RGBW Conversion
CRGBW leds[NUM_LEDS_PER_STRIP];
CRGB *ledsRGB = (CRGB *) &leds[0];
    

void setup() {
  //tell FastLED there're 32 strips on DATAPIN2-DATAPIN33, each has 144 LEDs/strip
  FastLED.addLeds<SK6812, DATA_PIN0>(ledsRGB, getRGBWsize(NUM_LEDS_PER_STRIP));
  FastLED.addLeds<SK6812, DATA_PIN1>(ledsRGB, getRGBWsize(NUM_LEDS_PER_STRIP));
  FastLED.addLeds<SK6812, DATA_PIN2>(ledsRGB, getRGBWsize(NUM_LEDS_PER_STRIP));
  FastLED.addLeds<SK6812, DATA_PIN3>(ledsRGB, getRGBWsize(NUM_LEDS_PER_STRIP));
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
  //FastLED.addLeds<SK6812, DATA_PIN16>(ledsRGB, getRGBWsize(NUM_LEDS_PER_STRIP));
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
  
  //setting brightness and turning the strips on
  FastLED.setBrightness(brightness);
  colorRGBW(CRGBW(255, 255, 255, 255));
}

void loop()
{
  //rainbow();
}
                
//solid color by calling color's name eg. Red, Blue, Black
void colorFill(CRGB c){
  for(int i = 0; i < NUM_LEDS_PER_STRIP; i++){
    leds[i] = c;
  }
  FastLED.show();
}

//choosing color based on RGBW values 0-255
void colorRGBW(CRGBW c){
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
  hue+=10;
}

//rainbow loop
void rainbowLoop(){
  //long millisIn = millis();
  //long loopTime = 5000000; // 5000 seconds
  while(true){
    //while(millis() < millisIn + loopTime){
    rainbow();
    //delay(1);
  }
}
