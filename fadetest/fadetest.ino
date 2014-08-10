int Rled = 9;
int Gled = 10;
int Bled = 11;
int brightness = 0;  
int fadeAmount = 5; 


void setup() {
  pinMode(Rled, OUTPUT);
  pinMode(Gled, OUTPUT);
  pinMode(Bled, OUTPUT);

}

void fade(int r1,int g1,int b1,int r2,int g2,int b2,int steps, int delay_amount){
  for(int i=0; i<steps; i++){
    analogWrite(Rled,r1+((r2-r1)/steps)*i);
    analogWrite(Gled,g1+((g2-g1)/steps)*i);
    analogWrite(Bled,b1+((b2-b1)/steps)*i);
    delay(delay_amount);
  }
}

void fadehsv(){}
void set(int r, int g, int b){
  analogWrite(Rled,r);
  analogWrite(Gled,g);
  analogWrite(Bled,b);
}
void loop() {
  fade(0,0,0,255,255,255,255,30);
  //fade(255,0,0,0,255,0,255,3);
 //fade(0,255,0,0,0,255,255,3);
 // fade(0,0,255,255,0,0,255,3);
  //fade(0,255,0,255,0,255,255,10);
  //fade(255,0,255,0,255,0,255,10);
//set(0,0,10);
//delay(1000);

//set(10,10,10);

//delay(1000);
//set(10,0,0);

//delay(1000);


//fade(0,0,0,255,255,255,255,30);
//fade(255,255,255,0,0,0,255,30);

}


