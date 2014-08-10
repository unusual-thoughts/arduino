#include "Tlc5940.h"
#include <IRremote.h>
#include "LEDBoard.h"

int RECV_PIN = 3;
IRrecv irrecv(RECV_PIN);
decode_results results;
uint8_t channel=0;
LEDBoard board(4095,2048,0);

void setup()
{
  irrecv.enableIRIn(); // Start the receiver
  Tlc.init();
  Serial.begin(9600);
  board.setLEDs();
    Tlc.update();
    Serial.println(board.getLED(0)->getR());
    Serial.println(board.getLED(0)->getG());
    Serial.println(board.getLED(0)->getB());
}

void loop()
{
  //Serial.println("loop");
  if (irrecv.decode(&results)) {
    Serial.println(results.value, HEX);
    board.transitionHSV(channel,(channel+1)%5,random(4096),4095,4095);
    //board.getLED((channel+1)%5)->setHSV(4000,4095,1000);
    Serial.println(board.getLED((channel+1)%5)->getR());
    Serial.println(board.getLED((channel+1)%5)->getG());
    Serial.println(board.getLED((channel+1)%5)->getB());
    board.setLED((channel+1)%5);
    Tlc.update();

    
    channel++;
    channel%=5;

    Tlc.clear();
    irrecv.resume(); // Receive the next value

   }
  

}
