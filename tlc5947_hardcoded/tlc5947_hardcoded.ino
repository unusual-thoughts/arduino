/* Ports and Pins
 Direct port access is much faster than digitalWrite.
 You must match the correct port and pin as shown in the table below.
 
 Arduino Pin        Port        Pin
 13 (SCK)           PORTB       5
 12 (MISO)          PORTB       4
 11 (MOSI)          PORTB       3
 10 (SS)            PORTB       2
 9                 PORTB       1
 8                 PORTB       0
 7                 PORTD       7
 6                 PORTD       6
 5                 PORTD       5
 4                 PORTD       4
 3                 PORTD       3
 2                 PORTD       2
 1 (TX)            PORTD       1
 0 (RX)            PORTD       0
 A5 (Analog)        PORTC       5
 A4 (Analog)        PORTC       4
 A3 (Analog)        PORTC       3
 A2 (Analog)        PORTC       2
 A1 (Analog)        PORTC       1
 A0 (Analog)        PORTC       0
 */

// Defines for use with Arduino functions
#define clockpin   13 // CI
#define enablepin  10 // EI
#define latchpin    9 // LI
#define datapin    11 // DI

// Defines for direct port access
#define CLKPORT PORTD
#define ENAPORT PORTD
#define LATPORT PORTD
#define DATPORT PORTD
#define CLKPIN  5
#define ENAPIN  2
#define LATPIN  6
#define DATPIN  4

// Number of OctoBrites / TLC5947 devices
#define NumOctoBrites 1

// Array storing color values
//  BLUE: LEDChannels[x][0]   Range: {0 to 4095}
// GREEN: LEDChannels[x][1]   Range: {0 to 4095}
//   RED: LEDChannels[x][2]   Range: {0 to 4095}
uint16_t LEDChannels[NumOctoBrites*8][3] = {0};

// Variables for sample function
float offset = 0;


// Set pins to outputs and initial states
void setup() {
  pinMode(datapin, OUTPUT);
  pinMode(latchpin, OUTPUT);
  pinMode(enablepin, OUTPUT);
  pinMode(clockpin, OUTPUT);
  digitalWrite(latchpin, LOW);
  digitalWrite(enablepin, LOW);
}

// Read all bits in the LEDChannels array and send them on the selected pins
void WriteLEDArray() {

  unsigned int tempOne = 0;

  for (int i = 0; i < (NumOctoBrites * 24); i++) {

    tempOne = *(&LEDChannels[0][0] + i);

    for (int j = 0; j < 12; j++) {
      if ((tempOne >> (11 - j)) & 1) {
        DATPORT |= (1 << DATPIN);
      } 
      else {
        DATPORT &= ~(1 << DATPIN);
      }
      CLKPORT |= (1 << CLKPIN);
      CLKPORT &= ~(1 << CLKPIN); 
    } 

  }
  LATPORT |= (1 << LATPIN);
  LATPORT &= ~(1 << LATPIN);
}

// Sample function to draw a scanning pattern with fading
void LEDscan(int red, int green, int blue, float degreeoffset) {

  float brightnessfactor = 0;
  
  float scanindex = (1.0 + sin(degreeoffset*3.14159/180.0)) * ((float)(NumOctoBrites * 8) / 2.0);
  
    for(int LEDindex = 0; LEDindex < (NumOctoBrites * 8); LEDindex++) {
      
      brightnessfactor = exp(0.0 - fabs(scanindex - ((float)LEDindex + 0.5)) * 1.3);
      
      LEDChannels[LEDindex][0] = blue * brightnessfactor;
      LEDChannels[LEDindex][1] = green * brightnessfactor;  
      LEDChannels[LEDindex][2] = red * brightnessfactor;  
    }
    
    WriteLEDArray();  

}

void loop() {

  // Scan across whole array with fading, in red, green, and blue
  
  for (offset = 0; offset < 360; offset += 0.5) {
    LEDscan(4095, 0, 0, offset);
    delay(2);
  }

  for (offset = 0; offset < 360; offset += 0.5) {
    LEDscan(0, 4095, 0, offset);
    delay(2);
  }
  for (offset = 0; offset < 360; offset += 0.5) {
    LEDscan(0, 0, 4095, offset);
    delay(2);
  }

}
