
int maLED = 3;
int monBeeper = 2;
void setup()
{
 pinMode(maLED, OUTPUT);  // Set RX LED as an output

 Serial.begin(9600); //This pipes to the serial monitor
 Serial1.begin(9600); //This is the UART, pipes to sensors attached to board
}

void loop()
{
 Serial.println("Joseph est une grosse merde");  // Print "Hello World" to the Serial Monitor
//Serial1.println("Hello!");  // Print "Hello!" over hardware UART
 for(int i=0; i<255; i++){
   analogWrite(maLED, i);   // set the LED on
   tone(monBeeper, 20*i+100,10);
   //delay(10);              // wait for a second
   // wait for a second
 }
 for(int i=255; i>0; i--){
   analogWrite(maLED, i);   // set the LED on
   tone(monBeeper, 20*i+100,10);
   //delay(10);              // wait for a second
   // wait for a second
 }
}

