#include <Arduino.h>
#include <irHack.h>

irHack::irHack(int IRpin){
	pinMode(IRpin,INPUT);
	attachInterrupt(0, irRoutine(this), CHANGE);
}

/*void irHack::init(int IRpin){
	pinMode(IRpin,INPUT);
	//attachInterrupt(0, irHack::irRoutine, CHANGE);
}*/

void irHack::onInterrupt(void){
	state = !state;
	int k = (micros() - lastmicros + IRRES/2) / IRRES;
	int j = 0;
	if(k > 15*8 || i>=30*8){
		i = 0;
		//for(j=0; j<100; j++)  Serial.print(buffer[j]);
		//for(j = 0; j<100; j++) buffer[j] = LOW;
		buffer[0] = state;
	}
	else{
		for(j = 0; j<k; j++){
		//buffer[i+j] = state;
		bitWrite(buffer[(i+j)/8], (i+j)%8, state);
		}
		i+=k;  
	}
	lastmicros = micros();
}

int irHack::getNewCode(byte[]){

}

void irRoutine(irHack *irhack){
	irhack->onInterrupt();
}