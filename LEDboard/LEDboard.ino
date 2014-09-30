#include "Tlc5940.h"
#include <IRremote.h>
#include "LEDBoard.h"

int RECV_PIN = 3;
IRrecv irrecv(RECV_PIN);
decode_results results;
uint8_t channel = 0;
LEDBoard board(4095, 2048, 0);
uint32_t val = 512, oldval = 512;
uint8_t ison = 1;
unsigned long lasttime = 0, curtime = 0;

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
  oldval = val;
  val = analogRead(A0);
  //if (val < oldval - 3) val = oldval - 3;
  if (val < oldval - 1) val -= (oldval - val) / 10;
  //Serial.println(val);
  //Serial.println(val >= 567);
  //Serial.println(val >= 513 ? ((val - 513) * 1023) / (750 - 513) : 0);
  
  if (ison)
    board.setVolume(val >= 515 ? ((val - 515) * 1023) / (750 - 515) : 0);
  else
 {
   Tlc.clear();
   Tlc.update();
 }
  Tlc.update();
  delay(5);
  curtime = millis();
  if (irrecv.decode(&results))
 {
   Serial.print("lasttime: ");
   Serial.print(lasttime);
   Serial.print(" curtime: ");
   Serial.print(curtime);
   Serial.print(" value: ");
   Serial.println(results.value, HEX);
   if((curtime - lasttime > 120) && results.value != 0xFFFFFFFF)
    {
    ison = !ison;
    lasttime = curtime;
    }
   irrecv.resume();
 }

 /*
  for (int i = 0; i < 1024; i++)
  {
    board.setVolume(i);
    Tlc.update();
    delayMicroseconds(2500);
  }
  */
  /*
  if (irrecv.decode(&results))
  {
    Serial.println(results.value, HEX);
    board.transitionHSV(channel, (channel + 1) % 5, random(4096), 4095, 4095);
    //board.getLED((channel+1)%5)->setHSV(4000,4095,1000);
    Serial.println(board.getLED((channel + 1) % 5)->getR());
    Serial.println(board.getLED((channel + 1) % 5)->getG());
    Serial.println(board.getLED((channel + 1) % 5)->getB());
    board.setLED((channel + 1) % 5);
    Tlc.update();
    
    channel++;
    channel %= 5;

    Tlc.clear();
    irrecv.resume(); // Receive the next value

  }
  */
}

