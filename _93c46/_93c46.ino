#include <util/delay.h>


//difining pins for eeprom   
int CLOCK =15;          
int DATA_OUT =14;
int DATA_IN = 16;
int CHIP_SEL =9;

int high = 0,low =0;     //high and low address 
byte dataword[]={"hello world!"};    //data to send in eeprom

byte READ  = 0b1100;                //read instruction 
byte WRITE = 0b1010;               //write instruction
byte EWEN  = 0b10011;         //erase write enable instruction

void setup(){
  pinMode(CLOCK ,OUTPUT);
  pinMode(DATA_OUT ,OUTPUT);
  pinMode(DATA_IN ,INPUT);
  pinMode(CHIP_SEL ,OUTPUT);
  digitalWrite(CHIP_SEL ,LOW);
  Serial.begin(9600); 
}

void loop()
{

  //digitalWrite(CHIP_SEL ,HIGH);                  
  //shiftOut(DATA_OUT,CLOCK,MSBFIRST,EWEN);     //sending EWEN instruction
  //digitalWrite(CHIP_SEL ,LOW);
  /*delay(10);
  for(int i=0;i<=13;i++)
  {
    digitalWrite(CHIP_SEL ,HIGH);
    shiftOut(DATA_OUT,CLOCK,MSBFIRST,WRITE); //sending WRITE instruction 
    shiftOut(DATA_OUT,CLOCK,MSBFIRST,low);   //sending low address
    shiftOut(DATA_OUT,CLOCK,MSBFIRST,dataword[i]); //sendind data
    digitalWrite(CHIP_SEL ,LOW);
    delay(100);
    low++;       //incrementing low address
  }*/
  low=0;
  for (int i=0;i<64;i++)
  { 
    digitalWrite(CHIP_SEL ,HIGH);
    shiftOut(DATA_OUT,CLOCK,MSBFIRST,READ); //sending READ instruction 
    shiftOut(DATA_OUT,CLOCK,MSBFIRST,low);   //sending low address
    uint16_t incoming = shiftIn(DATA_IN,CLOCK,MSBFIRST); //sendind data
    digitalWrite(CHIP_SEL ,LOW);
    low++;       //incrementing low address
  //  Serial.println(char(incoming));
      analogWrite(10, (unsigned char)(incoming >> 8));
    _delay_ms(50);
    analogWrite(10, (unsigned char)incoming);
    _delay_ms(50);
      Serial.print(incoming, HEX);
    Serial.print(" ");
    if((i + 1) % 8 == 0)
      Serial.println();
  } 
  analogWrite(10, 0);
  _delay_ms(10000);  
}
