// NeoPixel Ring simple sketch (c) 2013 Shae Erisson
// released under the GPLv3 license to match the rest of the AdaFruit NeoPixel library
#include <Adafruit_NeoPixel.h>

// Which pin on the Arduino is connected to the NeoPixels?
#define PIN            11

// How many NeoPixels are attached to the Arduino?
#define NUMPIXELS      1

// When we setup the NeoPixel library, we tell it how many pixels, and which pin to use to send signals.
// Note that for older NeoPixel strips you might need to change the third parameter--see the strandtest
// example for more information on possible values.
Adafruit_NeoPixel pixels = Adafruit_NeoPixel(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);

int delayval = 10; // delay for half a second

void setup() {
  pixels.begin(); // This initializes the NeoPixel library.
}

void loop() {
  for(int zone=1; zone<6; zone++)
  {
     for(int i=0;i<255;i++){
       switch(zone)
        {
          case 0:
            pixels.setPixelColor(0, pixels.Color(255,i,0));
            break;
          case 1:
            pixels.setPixelColor(0, pixels.Color(255-i,255,0));
            break;
          case 2:
            pixels.setPixelColor(0, pixels.Color(0,255,i));
            break;
          case 3:
            pixels.setPixelColor(0, pixels.Color(0,255-i,255));
            break;
          case 4:
            pixels.setPixelColor(0, pixels.Color(i,0,255));
            break;
          case 5:
            pixels.setPixelColor(0, pixels.Color(255,0,255-i));
            break;
        }
        // pixels.setPixelColor(0, pixels.Color(0,255-i,255));
          pixels.show(); // This sends the updated pixel color to the hardware.
          delay(delayval); // Delay for a period of time (in milliseconds).
    }
  }
}
