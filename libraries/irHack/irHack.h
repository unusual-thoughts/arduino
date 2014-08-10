#ifndef irHack_h
#define irHack_h
#endif

#include <Arduino.h>
#define IRRES 557UL

class irHack
{
	public:
		//void init(int IRpin);
		irHack(int IRpin);
		int getNewCode(byte[]);
		void onInterrupt(void);
		byte buffer[];

	private:
		volatile boolean state;
		volatile int i;
		volatile unsigned long lastmicros;
};

void irRoutine(irHack *irhack);