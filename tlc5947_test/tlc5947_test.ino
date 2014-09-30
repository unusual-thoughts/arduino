#include "TLC5947_me.h"

// How many boards do you have chained?
#define NUM_TLC5974 1

#define DATA        3  
#define CLOCK       5
#define LATCH       0
#define BLANK       1
#define C0          5
#define C1          6
#define C2          7
#define BITMASK     799

TLC5947 tlc = TLC5947(NUM_TLC5974, CLOCK, DATA, LATCH, BLANK);

const uint8_t PROGMEM gamma[] = {
    0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
    0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  1,  1,  1,  1,
    1,  1,  1,  1,  1,  1,  1,  1,  1,  2,  2,  2,  2,  2,  2,  2,
    2,  3,  3,  3,  3,  3,  3,  3,  4,  4,  4,  4,  4,  5,  5,  5,
    5,  6,  6,  6,  6,  7,  7,  7,  7,  8,  8,  8,  9,  9,  9, 10,
   10, 10, 11, 11, 11, 12, 12, 13, 13, 13, 14, 14, 15, 15, 16, 16,
   17, 17, 18, 18, 19, 19, 20, 20, 21, 21, 22, 22, 23, 24, 24, 25,
   25, 26, 27, 27, 28, 29, 29, 30, 31, 32, 32, 33, 34, 35, 35, 36,
   37, 38, 39, 39, 40, 41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 50,
   51, 52, 54, 55, 56, 57, 58, 59, 60, 61, 62, 63, 64, 66, 67, 68,
   69, 70, 72, 73, 74, 75, 77, 78, 79, 81, 82, 83, 85, 86, 87, 89,
   90, 92, 93, 95, 96, 98, 99,101,102,104,105,107,109,110,112,114,
  115,117,119,120,122,124,126,127,129,131,133,135,137,138,140,142,
  144,146,148,150,152,154,156,158,160,162,164,167,169,171,173,175,
  177,180,182,184,186,189,191,193,196,198,200,203,205,208,210,213,
  215,218,220,223,225,228,231,233,236,239,241,244,247,249,252,255 };
  
int gammapow(int i) {
  return int(pow(float(i)/4095,2.8)*4095);
}

void setup() {
  pinMode(C0,OUTPUT);
  pinMode(C1,OUTPUT);
  pinMode(C2,OUTPUT);
  digitalWrite(C0, LOW);
  digitalWrite(C1, LOW);
  digitalWrite(C2, LOW);
  tlc.begin();
}

int a = 0;
int order = 1;
void loop() {
    for(int c = 0; c < 8; c++)
    {
      for(int r = 0; r < 8; r++)
      {
         //tlc.setLED(r, gammapow((c) * 512), 0,  gammapow((r) * 512));
         //tlc.setLED(r, pgm_read_byte(&gamma[((c > r ? c  - r : 0) + 1) * 32 - 1]) * 16, pgm_read_byte(&gamma[((r > 7 - c ? r  - 7 + c : 0) + 1) * 32 - 1]) * 16, pgm_read_byte(&gamma[((r > c ? r  - c : 0) + 1) * 32 - 1]) * 16);
         tlc.setLED(r, pgm_read_byte(&gamma[((c > r ? c  - r : 0) + 1) * 32 - 1]) * 16, pgm_read_byte(&gamma[((r > 7 - c ? r  - 7 + c : 0) + 1) * 32 - 1]) * 16, pgm_read_byte(&gamma[((r > c ? r  - c : 0) + 1) * 32 - 1]) * 16);
                  Tlc.setLED(r, pgm_read_byte(&gamma[((c > r ? c  - r : 0) + 1) * 32 - 1]) * 16, pgm_read_byte(&gamma[((r > 7 - c ? r  - 7 + c : 0) + 1) * 32 - 1]) * 16, pgm_read_byte(&gamma[((r > c ? r  - c : 0) + 1) * 32 - 1]) * 16);

         //tlc.setLED(int ( 3.5 * (sin(a + r / 32 * 3.14 ) + 1)), 4095, 4095, 4905);
    }
            tlc.write();
            setColumn(c);
  
           //tlc.setLED(n, 0, 0, 0);
  
            //delay(100);
    }
  a += order;
  if (a == 32 || a == 0) order = -order;
}

void setColumn(uint8_t col)
{
  switch (col)
  {
    case 0:
      PORTD = (PORTD & BITMASK) | (3 << C0);
      break;
    case 1:
      PORTD = (PORTD & BITMASK) | (6 << C0);
      break;
    case 2:
      PORTD = (PORTD & BITMASK) | (5 << C0);
      break;
    case 3:
      PORTD = (PORTD & BITMASK) | (4 << C0);
      break;
    case 4:
      PORTD = (PORTD & BITMASK) | (7 << C0);
      break;
    case 5:
      PORTD = (PORTD & BITMASK) | (0 << C0);
      break;
    case 6:
      PORTD = (PORTD & BITMASK) | (1 << C0);
      break;
    case 7:
      PORTD = (PORTD & BITMASK) | (2 << C0);
      break;
  }
}
// Fill the dots one after the other with a color
void colorWipe(uint16_t val, uint8_t wait) {
  for(uint16_t i=0; i<24; i++) {
      tlc.setPWM(i, val);
      //delay(wait);
      //tlc.setPWM(i, 0);
      //tlc.write();
  }
 tlc.write();
}

