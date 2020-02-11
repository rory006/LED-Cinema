#include <Ethernet.h>
#include <SPI.h>
#include "FastLED.h"
#include "FastLED_RGBW.h"



// Arduino server MAC address
byte mac[] = { 0xA8, 0x61, 0x0A, 0xAE, 0x3D, 0x64 };
// Arduino server IP address
IPAddress ip(192, 168, 2, 254);
// Arduino server port
const int port = 23;
 
EthernetServer server(port);
// For storing command from client
String commandStr;
#define DATA_PIN 7
#define NUM_LEDS_PER_STRIP 60

    CRGBW leds[NUM_LEDS_PER_STRIP];
    CRGB *ledsRGB = (CRGB *) &leds[0];


const uint8_t brightness = 120;

void setup() {
   
    FastLED.addLeds<SK6812, DATA_PIN>(ledsRGB , getRGBWsize(NUM_LEDS_PER_STRIP));
        FastLED.setBrightness(brightness);
        FastLED.show();
        
    Serial.begin(9600);
 
    // Ethernet server initialization
    Ethernet.begin(mac, ip);
    server.begin();
 
    // Print Arduino server IP address to serial monitor
    Serial.print("Server is at ");
    Serial.println(Ethernet.localIP());

}

void loop() {
 EthernetClient client = server.available();
    if (client.available()) {
        // Read char until linefeed
        char c = client.read(); 
        if (c != '\n') {
            // Add received char to string variable 
            commandStr += c;
        } else {
            // Print received command to serial monitor
            Serial.println("Command: " + commandStr);
   
            // Process the received command
            processCommand(commandStr);
   
            // Clear variable for receive next command
            commandStr = "";
        }
    }

}

void processCommand(String cmd) {
    if (cmd == "led=1") {
        // Turn on LED
         colorFill(CRGB::Red);    // set our current dot to red
    FastLED.show();
    
  
    }
     else if (cmd == "led=0") {
        // Turn off LED
        
       colorFill(CRGB::Black);   // set our current dot to red
    FastLED.show();
        }
    } 

    void colorFill(CRGB c){
  for(int i = 0; i < NUM_LEDS_PER_STRIP; i++){
    leds[i] = c;
    FastLED.show();
    delay(50);
  }
  delay(500);
}
