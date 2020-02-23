#include <Ethernet.h>
#include <SPI.h>
#include "FastLED.h"
#include "FastLED_RGBW.h"

/*Because one Arduino Mega has only 8kb of variable memory, and 20m of 144 4-byte-LEDs
 * are too much to handle, we have to copy some LED strips
 */
/*
-> New Convention:
- W/C = Wall/Ceiling
- L/R/H = Left/Right/Horizontal

Wall door:
left to right, top to bottom (facing door)
s/e means start end (one LED strip on both sections)
   |4As-WL0    |3De-WR0
-------------------------------4B-WH0
   |4Ae-WL1    |3Ds-WR1
-------------------------------3C-WH1
   |4C -WL2    |3B -WR2
-------------------------------3A-WH2
   |4D -WL3    |2D -WR3
-------------------------------2C-WH3
   |4E -WL4    |2B -WR4
-------------------------------1C-WH4
   |1Ae-WL5    |2Ae-WR5
-------------------------------1B-WH5
   |1As-WL6    |2As-WR6
*/

/*Ceiling door:
left to right, top to bottom (facing wall door)
s/e means start end (one LED strip on both sections)
   |5As-23-CL0    |9As-CR0
-------------------------------5B-CH0
   |5Ae-CL1    |9Ae-CR1
-------------------------------9B-CH1
   ^              ^
   |5C -CL2    |9C -CR2
-------------------------------6A-CH2
   ^              ^
   |6B -CL3    |8D -CR3
-------------------------------6C-CH3
   ^              ^
   |7A -CL4    |8C -CR4
-------------------------------7B-CH4
   |8Ae-CL5    |8Bs-CR5
-------------------------------7C-CH5
   |8As-CL6    |8Be-CR6
*/

//number of leds per strip type on wall and ceiling
//number are guessed, need to be measured:
//we are using the WALL for both CEILING AND WALL because not enough memory
//also, we ignore the double/short strips and simply show the long strips again
#define W_LONG_STRIP 200 //actually 144
#define W_DOUBLE_STRIP 0
#define W_SHORT_STRIP 0

#define WL0_OFFSET 0
#define WL1_OFFSET 0 //>0
#define WL5_OFFSET 0 //>0
#define WL6_OFFSET 0

#define WR0_OFFSET 0 //>0
#define WR1_OFFSET 0
#define WR5_OFFSET 0 //>0
#define WR6_OFFSET 0

#define C_LONG_STRIP 0 //actually 144
#define C_DOUBLE_STRIP 0
#define C_SHORT_STRIP 0

#define CL0_OFFSET 0
#define CL1_OFFSET 0 //>0
#define CL5_OFFSET 0 //>0
#define CL6_OFFSET 0

#define CR0_OFFSET 0
#define CR1_OFFSET 0 //>0
#define CR5_OFFSET 0
#define CR6_OFFSET 0 //>0

/*---------------------------------------
 * From here on, everything is computed
 * --------------------------------------
 */
//number of strips per wall (so e.g. 12 long strips total because 2 walls)
#define N_LONG 6
#define N_DOUBLE 4
#define N_SHORT 6
#define NUM_TOTAL_LEDS (N_LONG*W_LONG_STRIP+N_DOUBLE*W_DOUBLE_STRIP+N_SHORT*W_SHORT_STRIP+N_LONG*C_LONG_STRIP+N_DOUBLE*C_DOUBLE_STRIP+N_SHORT*C_SHORT_STRIP)
/* memory layout:
 * wlong*n_lon wshort*n_short wdouble*n_double clong*n_lon cshort*n_short cdouble*n_double
 */

//memory positions
#define W_LONG_SIZE (W_LONG_STRIP*4)
#define W_DOUBLE_SIZE (W_DOUBLE_STRIP*4)
#define W_SHORT_SIZE (W_SHORT_STRIP*4)
#define C_LONG_SIZE (C_LONG_STRIP*4)
#define C_DOUBLE_SIZE (C_DOUBLE_STRIP*4)
#define C_SHORT_SIZE (C_SHORT_STRIP*4)

#define W_LONG_START 0
#define W_SHORT_START (W_LONG_SIZE*N_LONG)
#define W_DOUBLE_START (W_SHORT_START+W_SHORT_SIZE*N_SHORT)
#define C_LONG_START (W_DOUBLE_START+W_DOUBLE_SIZE*N_DOUBLE)
#define C_SHORT_START (C_LONG_SIZE*N_LONG)
#define C_DOUBLE_START (C_SHORT_START+C_SHORT_SIZE*N_SHORT)

//memory offsets for each strip section
//WALL
#define WL0 (W_DOUBLE_START+WL0_OFFSET)
#define WL1 (W_DOUBLE_START+WL1_OFFSET)
#define WL2 (W_SHORT_START)
#define WL3 (W_SHORT_START+W_SHORT_SIZE)
#define WL4 (W_SHORT_START+W_SHORT_SIZE*2)
#define WL5 (W_DOUBLE_START+W_DOUBLE_SIZE+WL5_OFFSET)
#define WL6 (W_DOUBLE_START+W_DOUBLE_SIZE+WL6_OFFSET)

#define WR0 (W_DOUBLE_START+W_DOUBLE_SIZE*2+WR0_OFFSET)
#define WR1 (W_DOUBLE_START+W_DOUBLE_SIZE*2+WR1_OFFSET)
#define WR2 (W_SHORT_START+W_SHORT_SIZE*3)
#define WR3 (W_SHORT_START+W_SHORT_SIZE*4)
#define WR4 (W_SHORT_START+W_SHORT_SIZE*5)
#define WR5 (W_DOUBLE_START+W_DOUBLE_SIZE*3+WR5_OFFSET)
#define WR6 (W_DOUBLE_START+W_DOUBLE_SIZE*3+WR6_OFFSET)

#define WH0 W_LONG_START
#define WH1 (W_LONG_START + W_LONG_SIZE)
#define WH2 (W_LONG_START + W_LONG_SIZE*2)
#define WH3 (W_LONG_START + W_LONG_SIZE*3)
#define WH4 (W_LONG_START + W_LONG_SIZE*4)
#define WH5 (W_LONG_START + W_LONG_SIZE*5)

//CEILING
#define CL0 (C_DOUBLE_START+CL0_OFFSET)
#define CL1 (C_DOUBLE_START+CL1_OFFSET)
#define CL2 (C_SHORT_START)
#define CL3 (C_SHORT_START+C_SHORT_SIZE)
#define CL4 (C_SHORT_START+C_SHORT_SIZE*2)
#define CL5 (C_DOUBLE_START+C_DOUBLE_SIZE+CL5_OFFSET)
#define CL6 (C_DOUBLE_START+C_DOUBLE_SIZE+CL6_OFFSET)

#define CR0 (C_DOUBLE_START+C_DOUBLE_SIZE*2+CR0_OFFSET)
#define CR1 (C_DOUBLE_START+C_DOUBLE_SIZE*2+CR1_OFFSET)
#define CR2 (C_SHORT_START+C_SHORT_SIZE*3)
#define CR3 (C_SHORT_START+C_SHORT_SIZE*4)
#define CR4 (C_SHORT_START+C_SHORT_SIZE*5)
#define CR5 (C_DOUBLE_START+C_DOUBLE_SIZE*3+CR5_OFFSET)
#define CR6 (C_DOUBLE_START+C_DOUBLE_SIZE*3+CR6_OFFSET)

#define CH0 C_LONG_START
#define CH1 (C_LONG_START + C_LONG_SIZE)
#define CH2 (C_LONG_START + C_LONG_SIZE*2)
#define CH3 (C_LONG_START + C_LONG_SIZE*3)
#define CH4 (C_LONG_START + C_LONG_SIZE*4)
#define CH5 (C_LONG_START + C_LONG_SIZE*5)

   
//output pins, 32 pins are needed
//data pin 4, 10, 50, 51, 52, 53 not usable, due to ethernet
//data pin 0, 1 not usable, due to serial IO
//data pin 13 not usable due to bootloader LED flash
//data pin 14,16 not usable due to unkown reasons
#define WH0_PIN 34 //4B
#define WL1_PIN 33 //4A
#define WL4_PIN 2 //4E
#define WR1_PIN 3 //3D
#define WR2_PIN 5 //3B
#define WH2_PIN 6 //3A
#define WR4_PIN 7 //2B
#define WR3_PIN 8 //2D
#define WH3_PIN 9 //2C
#define WR5_PIN 15 //2A
#define WL5_PIN 11 //1A
#define WH4_PIN 12 //1C
#define WL2_PIN 35 //4C
#define WL3_PIN 36 //4D
#define WH5_PIN 17 //1B
#define WH1_PIN 19 //3C

#define CH2_PIN 37 //6?
#define CH3_PIN 38 //6?
#define CL3_PIN 39 //6?
#define CR1_PIN 20 //9A
#define CH0_PIN 21 //5B
#define CH1_PIN 22 //9B
#define CL1_PIN 23 //5A
#define CL2_PIN 24 //5C
#define CR2_PIN 25 //9C
#define CL4_PIN 26 //7A
#define CH4_PIN 27 //7B
#define CH5_PIN 28 //7C
#define CL5_PIN 29 //8A
#define CR5_PIN 30 //8B
#define CR4_PIN 31 //8C
#define CR3_PIN 32 //8D

// Arduino server
byte mac[] = { 0xA8, 0x61, 0x0A, 0xAE, 0x3D, 0x64 };
IPAddress ip(192, 168, 2, 254);
EthernetServer server(23);

CRGBW color_buffer[NUM_TOTAL_LEDS];

//tell FastLED there're NUM_USED_PINS strips, each has NUM_LEDS_PER_STRIP RGBW-LEDs/strip
void init_strips(){
  //make all white
  for(unsigned int i = 0; i < NUM_TOTAL_LEDS; ++i){
    color_buffer[i] = CRGBW(0, 0, 0, 20);
  }

  byte *d = (byte*)color_buffer; //pointer position

  /*
  //Each strip with individual memory
  //WALL
  FastLED.addLeds<SK6812, WH0_PIN>((CRGB*)(d+WH0), getRGBWsize(W_LONG_STRIP));
  FastLED.addLeds<SK6812, WH1_PIN>((CRGB*)(d+WH1), getRGBWsize(W_LONG_STRIP));
  FastLED.addLeds<SK6812, WH2_PIN>((CRGB*)(d+WH2), getRGBWsize(W_LONG_STRIP));
  FastLED.addLeds<SK6812, WH3_PIN>((CRGB*)(d+WH3), getRGBWsize(W_LONG_STRIP));
  FastLED.addLeds<SK6812, WH4_PIN>((CRGB*)(d+WH4), getRGBWsize(W_LONG_STRIP));
  FastLED.addLeds<SK6812, WH5_PIN>((CRGB*)(d+WH5), getRGBWsize(W_LONG_STRIP));
  
  FastLED.addLeds<SK6812, WL1_PIN>((CRGB*)(d+WL0), getRGBWsize(W_DOUBLE_STRIP));
  FastLED.addLeds<SK6812, WR1_PIN>((CRGB*)(d+WR0), getRGBWsize(W_DOUBLE_STRIP));
  FastLED.addLeds<SK6812, WL5_PIN>((CRGB*)(d+WL5), getRGBWsize(W_DOUBLE_STRIP));
  FastLED.addLeds<SK6812, WR5_PIN>((CRGB*)(d+WR5), getRGBWsize(W_DOUBLE_STRIP));
  
  FastLED.addLeds<SK6812, WL2_PIN>((CRGB*)(d+WL2), getRGBWsize(W_SHORT_STRIP));
  FastLED.addLeds<SK6812, WL3_PIN>((CRGB*)(d+WL3), getRGBWsize(W_SHORT_STRIP));
  FastLED.addLeds<SK6812, WL4_PIN>((CRGB*)(d+WL4), getRGBWsize(W_SHORT_STRIP));
  FastLED.addLeds<SK6812, WR2_PIN>((CRGB*)(d+WR2), getRGBWsize(W_SHORT_STRIP));
  FastLED.addLeds<SK6812, WR3_PIN>((CRGB*)(d+WR3), getRGBWsize(W_SHORT_STRIP));
  FastLED.addLeds<SK6812, WR4_PIN>((CRGB*)(d+WR4), getRGBWsize(W_SHORT_STRIP));

  //CEILING
  FastLED.addLeds<SK6812, CH0_PIN>((CRGB*)(d+CH0), getRGBWsize(C_LONG_STRIP));
  FastLED.addLeds<SK6812, CH1_PIN>((CRGB*)(d+CH1), getRGBWsize(C_LONG_STRIP));
  FastLED.addLeds<SK6812, CH2_PIN>((CRGB*)(d+CH2), getRGBWsize(C_LONG_STRIP));
  FastLED.addLeds<SK6812, CH3_PIN>((CRGB*)(d+CH3), getRGBWsize(C_LONG_STRIP));
  FastLED.addLeds<SK6812, CH4_PIN>((CRGB*)(d+CH4), getRGBWsize(C_LONG_STRIP));
  FastLED.addLeds<SK6812, CH5_PIN>((CRGB*)(d+CH5), getRGBWsize(C_LONG_STRIP));

  FastLED.addLeds<SK6812, CL1_PIN>((CRGB*)(d+CL0), getRGBWsize(C_DOUBLE_STRIP));
  FastLED.addLeds<SK6812, CR1_PIN>((CRGB*)(d+CR0), getRGBWsize(C_DOUBLE_STRIP));
  FastLED.addLeds<SK6812, CL5_PIN>((CRGB*)(d+CL5), getRGBWsize(C_DOUBLE_STRIP));
  FastLED.addLeds<SK6812, CR5_PIN>((CRGB*)(d+CR5), getRGBWsize(C_DOUBLE_STRIP));

  FastLED.addLeds<SK6812, CL2_PIN>((CRGB*)(d+CL2), getRGBWsize(C_SHORT_STRIP));
  FastLED.addLeds<SK6812, CL3_PIN>((CRGB*)(d+CL3), getRGBWsize(C_SHORT_STRIP));
  FastLED.addLeds<SK6812, CL4_PIN>((CRGB*)(d+CL4), getRGBWsize(C_SHORT_STRIP));
  FastLED.addLeds<SK6812, CR2_PIN>((CRGB*)(d+CR2), getRGBWsize(C_SHORT_STRIP));
  FastLED.addLeds<SK6812, CR3_PIN>((CRGB*)(d+CR3), getRGBWsize(C_SHORT_STRIP));
  FastLED.addLeds<SK6812, CR4_PIN>((CRGB*)(d+CR4), getRGBWsize(C_SHORT_STRIP));
  */
  
  /*
  //Each strip with the first 6 horizontal wall memorie (WH0-WH5)
  FastLED.addLeds<SK6812, WH0_PIN>((CRGB*)(d+WH0), getRGBWsize(W_LONG_STRIP));
  FastLED.addLeds<SK6812, WH1_PIN>((CRGB*)(d+WH1), getRGBWsize(W_LONG_STRIP));
  FastLED.addLeds<SK6812, WH2_PIN>((CRGB*)(d+WH2), getRGBWsize(W_LONG_STRIP));
  FastLED.addLeds<SK6812, WH3_PIN>((CRGB*)(d+WH3), getRGBWsize(W_LONG_STRIP));
  FastLED.addLeds<SK6812, WH4_PIN>((CRGB*)(d+WH4), getRGBWsize(W_LONG_STRIP));
  FastLED.addLeds<SK6812, WH5_PIN>((CRGB*)(d+WH5), getRGBWsize(W_LONG_STRIP));

  FastLED.addLeds<SK6812, WL1_PIN>((CRGB*)(d+WH0), getRGBWsize(W_LONG_STRIP));
  FastLED.addLeds<SK6812, WR1_PIN>((CRGB*)(d+WH0), getRGBWsize(W_LONG_STRIP));
  FastLED.addLeds<SK6812, WL5_PIN>((CRGB*)(d+WH4), getRGBWsize(W_LONG_STRIP));
  FastLED.addLeds<SK6812, WR5_PIN>((CRGB*)(d+WH4), getRGBWsize(W_LONG_STRIP));
  
  FastLED.addLeds<SK6812, WL2_PIN>((CRGB*)(d+WH1), getRGBWsize(W_LONG_STRIP));
  FastLED.addLeds<SK6812, WL3_PIN>((CRGB*)(d+WH2), getRGBWsize(W_LONG_STRIP));
  FastLED.addLeds<SK6812, WL4_PIN>((CRGB*)(d+WH3), getRGBWsize(W_LONG_STRIP));
  FastLED.addLeds<SK6812, WR2_PIN>((CRGB*)(d+WH1), getRGBWsize(W_LONG_STRIP));
  FastLED.addLeds<SK6812, WR3_PIN>((CRGB*)(d+WH2), getRGBWsize(W_LONG_STRIP));
  FastLED.addLeds<SK6812, WR4_PIN>((CRGB*)(d+WH3), getRGBWsize(W_LONG_STRIP));

  FastLED.addLeds<SK6812, CH0_PIN>((CRGB*)(d+WH0), getRGBWsize(W_LONG_STRIP));
  FastLED.addLeds<SK6812, CH1_PIN>((CRGB*)(d+WH1), getRGBWsize(W_LONG_STRIP));
  FastLED.addLeds<SK6812, CH2_PIN>((CRGB*)(d+WH2), getRGBWsize(W_LONG_STRIP));
  FastLED.addLeds<SK6812, CH3_PIN>((CRGB*)(d+WH3), getRGBWsize(W_LONG_STRIP));
  FastLED.addLeds<SK6812, CH4_PIN>((CRGB*)(d+WH4), getRGBWsize(W_LONG_STRIP));
  FastLED.addLeds<SK6812, CH5_PIN>((CRGB*)(d+WH5), getRGBWsize(W_LONG_STRIP));

  FastLED.addLeds<SK6812, CL1_PIN>((CRGB*)(d+WH0), getRGBWsize(W_LONG_STRIP));
  FastLED.addLeds<SK6812, CR1_PIN>((CRGB*)(d+WH0), getRGBWsize(W_LONG_STRIP));
  FastLED.addLeds<SK6812, CL5_PIN>((CRGB*)(d+WH4), getRGBWsize(W_LONG_STRIP));
  FastLED.addLeds<SK6812, CR5_PIN>((CRGB*)(d+WH4), getRGBWsize(W_LONG_STRIP));

  FastLED.addLeds<SK6812, CL2_PIN>((CRGB*)(d+WH1), getRGBWsize(W_LONG_STRIP));
  FastLED.addLeds<SK6812, CL3_PIN>((CRGB*)(d+WH2), getRGBWsize(W_LONG_STRIP));
  FastLED.addLeds<SK6812, CL4_PIN>((CRGB*)(d+WH3), getRGBWsize(W_LONG_STRIP));
  FastLED.addLeds<SK6812, CR2_PIN>((CRGB*)(d+WH1), getRGBWsize(W_LONG_STRIP));
  FastLED.addLeds<SK6812, CR3_PIN>((CRGB*)(d+WH2), getRGBWsize(W_LONG_STRIP));
  FastLED.addLeds<SK6812, CR4_PIN>((CRGB*)(d+WH3), getRGBWsize(W_LONG_STRIP));
  */

  //Each strip with the same memory
  FastLED.addLeds<SK6812, WH0_PIN>((CRGB*)(d+WH0), getRGBWsize(W_LONG_STRIP));
  FastLED.addLeds<SK6812, WH1_PIN>((CRGB*)(d+WH0), getRGBWsize(W_LONG_STRIP));
  FastLED.addLeds<SK6812, WH2_PIN>((CRGB*)(d+WH0), getRGBWsize(W_LONG_STRIP));
  FastLED.addLeds<SK6812, WH3_PIN>((CRGB*)(d+WH0), getRGBWsize(W_LONG_STRIP));
  FastLED.addLeds<SK6812, WH4_PIN>((CRGB*)(d+WH0), getRGBWsize(W_LONG_STRIP));
  FastLED.addLeds<SK6812, WH5_PIN>((CRGB*)(d+WH0), getRGBWsize(W_LONG_STRIP));

  FastLED.addLeds<SK6812, WL1_PIN>((CRGB*)(d+WH0), getRGBWsize(W_LONG_STRIP));
  FastLED.addLeds<SK6812, WR1_PIN>((CRGB*)(d+WH0), getRGBWsize(W_LONG_STRIP));
  FastLED.addLeds<SK6812, WL5_PIN>((CRGB*)(d+WH0), getRGBWsize(W_LONG_STRIP));
  FastLED.addLeds<SK6812, WR5_PIN>((CRGB*)(d+WH0), getRGBWsize(W_LONG_STRIP));
  
  FastLED.addLeds<SK6812, WL2_PIN>((CRGB*)(d+WH0), getRGBWsize(W_LONG_STRIP));
  FastLED.addLeds<SK6812, WL3_PIN>((CRGB*)(d+WH0), getRGBWsize(W_LONG_STRIP));
  FastLED.addLeds<SK6812, WL4_PIN>((CRGB*)(d+WH0), getRGBWsize(W_LONG_STRIP));
  FastLED.addLeds<SK6812, WR2_PIN>((CRGB*)(d+WH0), getRGBWsize(W_LONG_STRIP));
  FastLED.addLeds<SK6812, WR3_PIN>((CRGB*)(d+WH0), getRGBWsize(W_LONG_STRIP));
  FastLED.addLeds<SK6812, WR4_PIN>((CRGB*)(d+WH0), getRGBWsize(W_LONG_STRIP));

  FastLED.addLeds<SK6812, CH0_PIN>((CRGB*)(d+WH0), getRGBWsize(W_LONG_STRIP));
  FastLED.addLeds<SK6812, CH1_PIN>((CRGB*)(d+WH0), getRGBWsize(W_LONG_STRIP));
  FastLED.addLeds<SK6812, CH2_PIN>((CRGB*)(d+WH0), getRGBWsize(W_LONG_STRIP));
  FastLED.addLeds<SK6812, CH3_PIN>((CRGB*)(d+WH0), getRGBWsize(W_LONG_STRIP));
  FastLED.addLeds<SK6812, CH4_PIN>((CRGB*)(d+WH0), getRGBWsize(W_LONG_STRIP));
  FastLED.addLeds<SK6812, CH5_PIN>((CRGB*)(d+WH0), getRGBWsize(W_LONG_STRIP));

  FastLED.addLeds<SK6812, CL1_PIN>((CRGB*)(d+WH0), getRGBWsize(W_LONG_STRIP));
  FastLED.addLeds<SK6812, CR1_PIN>((CRGB*)(d+WH0), getRGBWsize(W_LONG_STRIP));
  FastLED.addLeds<SK6812, CL5_PIN>((CRGB*)(d+WH0), getRGBWsize(W_LONG_STRIP));
  FastLED.addLeds<SK6812, CR5_PIN>((CRGB*)(d+WH0), getRGBWsize(W_LONG_STRIP));

  FastLED.addLeds<SK6812, CL2_PIN>((CRGB*)(d+WH0), getRGBWsize(W_LONG_STRIP));
  FastLED.addLeds<SK6812, CL3_PIN>((CRGB*)(d+WH0), getRGBWsize(W_LONG_STRIP));
  FastLED.addLeds<SK6812, CL4_PIN>((CRGB*)(d+WH0), getRGBWsize(W_LONG_STRIP));
  FastLED.addLeds<SK6812, CR2_PIN>((CRGB*)(d+WH0), getRGBWsize(W_LONG_STRIP));
  FastLED.addLeds<SK6812, CR3_PIN>((CRGB*)(d+WH0), getRGBWsize(W_LONG_STRIP));
  FastLED.addLeds<SK6812, CR4_PIN>((CRGB*)(d+WH0), getRGBWsize(W_LONG_STRIP));
  
  //Tests
  //FastLED.addLeds<SK6812, WH1_PIN>((CRGB*)(d+WH1), getRGBWsize(W_LONG_STRIP));

}

void setup() {
  // USB debugging port
  Serial.begin(9600);
  
  // Ethernet server initialization
  Ethernet.begin(mac, ip);
  server.begin();

  init_strips();
  FastLED.setBrightness(255);
  FastLED.show();

  // Print Arduino server IP address to serial monitor
  Serial.print("Server is at ");
  Serial.println(Ethernet.localIP());
  Serial.print("Expected LEDs: ");
  Serial.println(NUM_TOTAL_LEDS);
  Serial.print("Expected bytes: ");
  Serial.println(NUM_TOTAL_LEDS*4);
  Serial.println("Expected Start Byte: 0xFF");
}

void loop() {
  EthernetClient client = server.available();
  unsigned int expected_bytes = NUM_TOTAL_LEDS*4;
  static char state = 0; //waiting for start
  static unsigned int bytes_read = 0;
  unsigned int available_bytes = client.available();
  if (available_bytes) {
    if(state == 0){  // waiting for byte array
      for(unsigned int i = 0; i < available_bytes; ++i) {
        if (client.read() == 0xFF) {
          state = 1;
          bytes_read = 0;
          break;
        }
        bytes_read += 1;
      }
      if(bytes_read != 0)
        Serial.println(bytes_read);
    } else {  //reading bytes
      for(unsigned int i = 0; i < available_bytes; ++i) {
        ((byte*)color_buffer)[bytes_read] = client.read();
        bytes_read++;
        if(bytes_read == expected_bytes){
          Serial.print("Received full color package with ");
          Serial.print(expected_bytes);
          Serial.println(" bytes.");
          state = 0;
          FastLED.show();
          break;
        }
      }
    }
  }
}
