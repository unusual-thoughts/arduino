#include <Wire.h>
#include <LCD.h>
#include <LiquidCrystal_I2C.h>
#define I2C_ADDR     0x27
LiquidCrystal_I2C lcd(0x27, 2, 1, 0, 4, 5, 6, 7, 3, POSITIVE);

#define IRPIN 2
#define TESTLED 9
//#define IRRES 565UL
//#define IRRES 557UL
#define IRMARK 2224UL
#define IRSPACE 1113UL
#define IRCONF 500UL
#define IRINIT 13617UL
//#define IRREPEAT 11179UL
//#define IRREPEAT 43712UL
#define IRREPEAT 107451UL
#define IRREPEATINIT 11182UL
//#define IRLENGTH 32
//volatile byte code[IRLENGTH];
volatile unsigned int code;
volatile int i = 0;
volatile unsigned long lastmicros = 0;
volatile unsigned long lastdelta = 0;
volatile unsigned long lastframe = 0;
volatile boolean caught = false;
volatile boolean data = true;

char line1[16];
char line2[16];

void setup ( )
{
  lcd.begin ( 16, 2 );
  lcd.backlight();
  lcd.home ();                   // go home
  lcd.print("................");  
  lcd.setCursor ( 0, 1 );        // go to the next line
  lcd.print ("................");    

  int j = 0;
  //for(j = 0; j<IRLENGTH; j++) code[j] = 0;
  code = 0;
  pinMode(IRPIN,INPUT);
  pinMode(TESTLED,OUTPUT);
  attachInterrupt(0, irRoutine, FALLING);
  Serial.begin(9600);
}

void irRoutine()
{
  unsigned long cur_micros = micros();
  unsigned long delta = cur_micros - lastmicros;
  lastmicros = cur_micros;
  if (delta > IRINIT + IRCONF) {
    // first falling edge
    i = 0;
    caught = false;
    code = 0;
    if (((lastframe - cur_micros) >= (IRREPEAT - IRCONF)) && ((lastframe - cur_micros) <= (IRREPEAT + IRCONF))){
      // frame is a repeat/hold frame
      data = false;
    }
    else {
      // frame is a data frame
      data = true;
    }
    lastframe = cur_micros;
  }
  else {
    if (delta >= IRINIT - IRCONF) {
      // data init pulse
      // frame is a data/repeat frame
    }
    if ((delta >= IRREPEATINIT - IRCONF) && (delta <= IRREPEATINIT + IRCONF)){
      // hold init pulse
      // frame is a hold frame
    }
    if ((delta >= IRSPACE - IRCONF) && (delta <= IRSPACE + IRCONF)) {
      // space
      if (i >= 16) {
        // last two bytes
        bitWrite(code, i-16, 0);
      }
      i++;
    }
    if ((delta >= IRMARK - IRCONF) && (delta <= IRMARK + IRCONF)) {
      // mark
      if (i >= 16) {
        // last two bytes
        bitWrite(code, i-16, 1);
      }
      i++;
    }
  }
 lastdelta = delta;
}

void loop ()
{
  if(((micros() - lastmicros) > IRINIT + IRCONF) && !caught && data){
 //   if(i==32){
    caught = true;
    lcd.home ();
    lcd.print("                ");
    lcd.home ();
    lcd.print(i);
    lcd.setCursor(0,1);
    lcd.print("                ");
    lcd.setCursor(0,1);
    lcd.print(code);
 // }
 Serial.print(code);
 Serial.print(" ");
 Serial.println(i);
 //code=0;
 //i=0;
}
// delay(500);
   /*
   
    if(((micros() - lastmicros) > IRMARK*45) && !caught && i == 0){
      caught = true;
          lcd.home ();  
    lcd.print("                ");
    lcd.home ();  
    lcd.print(lastdelta);
   // lcd.print(line1); 
   lcd.setCursor(10,0);
   lcd.print(i); 
         lcd.setCursor(0,1);

       lcd.print("                ");

      lcd.setCursor(8,1);
      lcd.print("       ");
      lcd.setCursor(8,1);
      lcd.print("nothing");
   }
    else
   { 
  
     if(((micros() - lastmicros) > IRMARK*7) && !caught && i == 0){
      caught = true;
          lcd.home ();  
    lcd.print("                ");
    lcd.home ();  
    lcd.print(lastdelta);
   // lcd.print(line1); 
   lcd.setCursor(10,0);
   lcd.print(i); 
         lcd.setCursor(0,1);

       lcd.print("                ");

      lcd.setCursor(8,1);
      lcd.print("       ");
      lcd.setCursor(8,1);
      lcd.print("hold");
   }
   }
  if(((micros() - lastmicros) > IRMARK*7) && !caught && i == 32){
    //if (i == 32){
    caught = true;
    sprintf(line1,"%02X%02X%02X%02X",code[0],code[1],code[2],code[3]);
   // sprintf(line2,"%02X%02X%02X%02X%02X%02X%02X%02X",code[8],code[9],code[10],code[11],code[12],code[13],code[14],code[15]);
    lcd.home ();  
    lcd.print("                ");
    lcd.home ();  
    lcd.print(lastdelta);
   // lcd.print(line1); 
   lcd.setCursor(10,0);
   lcd.print(i); 
    lcd.setCursor(0,1);
        lcd.print("                ");
    lcd.setCursor(0,1);

    lcd.print(line1); 
   
   //for(j = 0; j < IRLENGTH; j++) code[j] = 0;
  // i=0;
  
 }
 */



}
