#ifndef LEDBoard_h
#define LEDBoard_h
#include <Arduino.h>
#include "Tlc5940.h"

class LED
{
	public:
		LED(uint16_t R, uint16_t G, uint16_t B);
		LED();
		uint16_t getR();
		uint16_t getG();
		uint16_t getB();
		uint16_t getH();
		uint16_t getS();
		uint16_t getV();
		void setRGB(uint16_t R, uint16_t G, uint16_t B);
		void setHSV(uint32_t h, uint32_t s, uint32_t v);
		
	private:
		uint16_t _R;
		uint16_t _G;
		uint16_t _B;
};

class LEDBoard
{
	public:
		LEDBoard();
		LEDBoard(uint16_t R,uint16_t G,uint16_t B);
		void setLEDs();
		void setLED(uint8_t led);
		LED* getLED(uint8_t led);
		void transition(uint8_t previous, uint8_t channel, uint16_t R, uint16_t G, uint16_t B);
		void transitionHSV(uint8_t previous, uint8_t channel, uint16_t H, uint16_t S, uint16_t V);
		
	private:
		LED _leds[5];
	

};
#endif



