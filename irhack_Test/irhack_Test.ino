#include <irHack.h>

int RECV_PIN = 2;

irHack irhack(RECV_PIN);
byte code[30];

void setup()
{
  Serial.begin(9600);
  //irhack.init(RECV_PIN); // Start the receiver
}

void loop() {
  if (irhack.getNewCode(code)) {
    for (int i=0;i<30;i++) Serial.println(code[i], HEX);
  }
}
