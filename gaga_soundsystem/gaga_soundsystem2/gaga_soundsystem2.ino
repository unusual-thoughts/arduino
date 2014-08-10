#include "Tlc5940.h"
#include <avr/io.h>
#include <avr/interrupt.h>

#define IRPIN 2
#define TESTLED 9

#define IRRES 281UL
#define IRMARK 1688UL
#define IRSPACE 533UL
#define IRREPEAT 107328UL
#define IRINIT 13617UL
#define IRHOLD 11179UL

// IR STUFF
volatile byte code[4];
volatile int i = 0;
volatile unsigned long lastmicros = 0;
volatile int newcode = 0;
volatile int repeat = 0;
volatile int hold = 0;
volatile unsigned long lastframe = 0;

volatile unsigned long lastdiff = 0;


// LED STUFF

int crossover = 3500;
int channel=0;


void setup() {
  //Tlc.init();
  
  
  int j = 0;
  for(j = 0; j<30; j++) code[j] = 0;
  pinMode(IRPIN,INPUT);
  pinMode(TESTLED,OUTPUT);
 // attachInterrupt(0, irRoutine, FALLING);
  Serial.begin(9600);


  EIMSK |= _BV(INT0);  //Enable INT0
  EICRA |= _BV(ISC01); //Trigger on falling edge of INT0
}

void loop() {
  int j = 0;
  delay(10);
  if(micros() - lastmicros > IRMARK + IRSPACE + IRRES && newcode ){

    for(j=0; j<4; j++)  {
      Serial.print(code[j],HEX);
      Serial.print(" ");
    }
    Serial.println();
    Serial.print(i);
    if (repeat)
      Serial.print(" repeat");
    else
      Serial.print(" norepeat");
    if (hold)
      Serial.print(" hold");
    else
      Serial.print(" nohold");      
    newcode = 0;

    Serial.print(" ");
    Serial.print(lastdiff);

    Serial.println();
    Serial.println();  
    
    
    
    //i=0;
    //lastmicros=micros();
    
    /*
    int change=0;
    int cur=0;
    //Tlc.clear();
    cur=0;
    long i=0;
    for(i=0;i<64;i++){
      cur+=i;
      setRGB(channel, cur, cur, 0);
      setRGB(mod(channel-1,5), 4095-cur,4095-cur, 0);
      delayMicroseconds(3000);
      //Serial.println(cur);
      Tlc.update();
    }
    cur+=63;
    for(i=63;i>=0;i--){
      cur+=i;
      setRGB(channel, cur, cur, 0);
      setRGB(mod(channel-1,5), 4095-cur, 4095-cur, 0);
      delayMicroseconds(3000);
      //Serial.println(cur);
      Tlc.update();
    }
     */
     
    channel++;
    channel%=5;
    
  }
 // setRGB(channel, 4095, 4095, 4095);
 //       Tlc.update();

}

ISR (INT0_vect)
{
  int delay = micros() - lastmicros;
  int j = 0;
  if(delay > IRMARK + IRSPACE + IRRES){
    if (delay < IRINIT + IRRES && delay > IRINIT - IRRES){ // INIT PULSE
      newcode = 1;
      i = 0;
      for(j = 0; j<4; j++) code[j] = LOW;
      hold = 0;
      
      lastdiff=lastmicros-lastframe;
      
      repeat = (lastmicros - lastframe < IRREPEAT + IRRES && lastmicros - lastframe > IRREPEAT - IRRES);
      lastframe = lastmicros;
    }
    if (delay < IRHOLD + IRRES && delay > IRHOLD - IRRES){ // HOLD FRAME INIT PULSE
      newcode = 1;
      i = 32;
      for(j = 0; j<4; j++) code[j] = LOW;
      hold = 1;

      lastdiff=lastmicros-lastframe;

      repeat = (lastmicros - lastframe < IRREPEAT + IRRES && lastmicros - lastframe > IRREPEAT - IRRES);
      lastframe = lastmicros;

    }
  }
  else{
    if (delay > 2*IRSPACE-IRRES && delay < 2*IRSPACE+IRRES){
      bitWrite(code[i/8], i%8, LOW);
      i++;
    }
    if (delay > IRSPACE+IRMARK-IRRES && delay < IRSPACE+IRMARK+IRRES){ //can remove first test
      bitWrite(code[i/8], i%8, HIGH);
      i++;
    }    
  }
  lastmicros = micros();
}

void setRGB(int led,uint16_t R,uint16_t G,uint16_t B){
  if (led<3){
    Tlc.set(led*3,R);
    Tlc.set(led*3+1,G);
    Tlc.set(led*3+2,B);
  }
  else{
    Tlc.set(led*3+1,R);
    Tlc.set(led*3+2,G);
    Tlc.set(led*3+3,B);
  }
}

int mod(int a, int b)
{
    int r = a % b;
    return r < 0 ? r + b : r;
}

