//#include <ardumidi.h>
#include <ps2.h>


PS2 mouse(13, 12);
unsigned int maxx,minx,maxy,miny,maxz,minz;

void kaoss_init()
{
  mouse.write(0xff);  // reset
  mouse.read();  // ack byte
  mouse.read();  // blank */
  mouse.read();  // blank */
  mouse.write(0xf0);  // remote mode
  mouse.read();  // ack
  delayMicroseconds(100);
      Serial.println("hello2");
  mouse.write(0xe8);
  mouse.read();  // ack byte
  mouse.write(0x03); // x1  ( x1 * 64  +  x2 * 16  +  x3 * 4  +  x4   == modebyte )
  mouse.read();  // ack byte
  mouse.write(0xe8);
  mouse.read();  // ack byte
  mouse.write(0x00); // x2
  mouse.read();  // ack byte
  mouse.write(0xe8);
  mouse.read();  // ack byte
  mouse.write(0x01); // x3
  mouse.read();  // ack byte
  mouse.write(0xe8);
  mouse.read();  // ack byte
  mouse.write(0x00); // x4
  mouse.read();  // ack byte
  mouse.write(0xf3); // set samplerate 20 (stores mode) 
  mouse.read();  // ack byte
  mouse.write(0x14);
  mouse.read();  // ack byte
  delayMicroseconds(100); 
}

void setup()
{
  Serial.begin(9600);
    Serial.println("hello");
  kaoss_init();

  maxx = 0;
  minx = 32000;
  maxy = 0;
  miny = 32000;
  maxz = 0;
  minz = 32000;
}


void loop()
{
  byte mstat1;
  byte mstat2;
  byte mxy;
  byte mx;
  byte my;
  byte mz;

  byte red;
  byte green;
  byte blue;
  byte midix;
  byte midiy;
  byte midiz;

  unsigned int cx,cy;

  mouse.write(0xeb);
  mouse.read();
  
  mstat1 = mouse.read();
  mxy = mouse.read();
  mz = mouse.read();
  mstat2 = mouse.read();
  mx = mouse.read();
  my = mouse.read();


  // collect the bits for x and y
  cx = (((mstat2 & 0x10) << 8) | ((mxy & 0x0F) << 8) | mx );
  cy = (((mstat2 & 0x20) << 7) | ((mxy & 0xF0) << 4) | my );
  

  // change the following digit to adjust the 'sensitivity' to touches  
  if (mz > 3) {

    // autocalibrate

    if (cx < minx) { minx = cx; }
    if (cx > maxx) { maxx = cx; }
    if (cy < miny) { miny = cy; }
    if (cy > maxy) { maxy = cy; }
    if (mz < minz) { minz = mz; }
    if (mz > maxz) { maxz = mz; }

    // determine led value (based on z value)
    red = map(cx,minx,maxx,0,255);
    blue = map(cy,miny,maxy,0,255);
    green = map(mz,minz,maxz,0,255);
    analogWrite(3,red);
    analogWrite(5,green);
    analogWrite(6,blue);
    // determine midi values
 //   midix = map(cx,minx,maxx,0,127);
 //   midiy = map(cy,miny,maxy,0,127);
 //   midiz = map(mz,minz,maxz,0,127);
    
 //   midi_controller_change(0,0,midix);
  //  midi_controller_change(0,1,midiy);
 //   midi_controller_change(0,2,midiz);
  Serial.print(mstat1, BIN);
  Serial.print("\tR=");
  Serial.print(red, DEC);
  Serial.print("\tG=");
  Serial.print(green, DEC);
  Serial.print("\tB=");
  Serial.print(blue, DEC);
  Serial.println();
  
  Serial.print(mstat2, BIN);
  Serial.print("\tx=");
  Serial.print(cx, DEC);
  Serial.print("\ty=");
  Serial.print(cy, DEC);
  Serial.print("\tz=");
  Serial.print(mz, DEC);
  Serial.println();
  Serial.print(map(cx,minx,maxx,30,5000), DEC);
    Serial.println();
tone(8,map(cx,minx,maxx,30,1000),100);
 delay(20);
    //     noTone(8); 
 //        tone(8,map(cy,miny,maxy,30,1000),20);
  //delay(20);
//           tone(8,map(mz,minz,maxz,30,1000),20);
//  delay(20);
 //        noTone(8);

  }


}
