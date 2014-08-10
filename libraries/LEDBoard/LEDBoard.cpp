#include "LEDBoard.h"
#include <Arduino.h>
#include "Tlc5940.h"

LED::LED(uint16_t R, uint16_t G, uint16_t B)
{
	_R = R;
	_G = G;
	_B = B;
}	
	

LED::LED()
{
	_R = 0;
	_G = 0;
	_B = 0;
}

uint16_t LED::getR()
{
	return _R;
}

uint16_t LED::getG()
{
	return _G;
}

uint16_t LED::getB()
{
	return _B;

}

uint16_t LED::getH()
{
    uint16_t rgbMin, rgbMax;

    rgbMin = _R < _G ? (_R < _B ? _R : _B) : (_G < _B ? _G : _B);
    rgbMax = _R > _G ? (_R > _B ? _R : _B) : (_G > _B ? _G : _B);

    if (rgbMax == 0)
    {
        return 0;
    }

    if (rgbMax == rgbMin)
    {
        return 0;
    }

    if (rgbMax == _R)
        return 0 + 682 * (_G - _B) / (rgbMax - rgbMin);
    else if (rgbMax == _G)
        return 1365 + 682 * (_B - _R) / (rgbMax - rgbMin);
    else
        return 2730 + 682 * (_R - _G) / (rgbMax - rgbMin);

}

uint16_t LED::getS()
{
    uint16_t rgbMin, rgbMax;

    rgbMin = _R < _G ? (_R < _B ? _R : _B) : (_G < _B ? _G : _B);
    rgbMax = _R > _G ? (_R > _B ? _R : _B) : (_G > _B ? _G : _B);

    if (rgbMax == 0)
    {
        return 0;
    }
    return 4096 * ((uint32_t)(rgbMax - rgbMin)) / rgbMax;
}

uint16_t LED::getV()
{
    return _R > _G ? (_R > _B ? _R : _B) : (_G > _B ? _G : _B);
}

void LED::setRGB(uint16_t R, uint16_t G, uint16_t B)
{
	_R = R;
	_G = G;
	_B = B;
}

void LED::setHSV(uint32_t h, uint32_t s, uint32_t v)
{
    uint32_t region, p, q, t, remainder;
	
   /* if (s == 0)
    {
        r = v;
        g = v;
        b = v;
    }*/

    region = h / 682;
    remainder = (h - (region * 682)) * 6; 

    p = (v * (4095 - s)) >> 12;
    q = (v * (4095 - ((s * remainder) >> 12))) >> 12;
    t = (v * (4095 - ((s * (4095 - remainder)) >> 12))) >> 12;

	/*Serial.println(p);
	Serial.println(q);
	Serial.println(t);
	Serial.println(region);
	Serial.println(remainder);*/
	
    switch (region)
    {
        case 0:
            _R = v;
            _G = t;
            _B = p;
            break;
        case 1:
            _R = q;
            _G = v;
            _B = p;
            break;
        case 2:
            _R = p;
            _G = v;
            _B = t;
            break;
        case 3:
            _R = p;
            _G = q;
            _B = v;
            break;
        case 4:
            _R = t;
            _G = p;
            _B = v;
            break;
        default:
            _R = v;
            _G = p;
            _B = q;
            break;
    }
}

LEDBoard::LEDBoard()
{
	LED _leds[5]; 
	for(uint8_t i = 0; i < 5; i++)
    {
		_leds[i] = *(new LED());
	}
}

LEDBoard::LEDBoard(uint16_t R,uint16_t G,uint16_t B)
{
	LED _leds[5]; 
	for(uint8_t i = 0; i < 5; i++)
    {
		_leds[i] = *(new LED(R,G,B));
	}
}

void LEDBoard::setLED(uint8_t led)
{
	if(led<3)
    {
		Tlc.set(led*3, _leds[led].getR());
		Tlc.set(led*3 + 1, _leds[led].getG());
		Tlc.set(led*3 + 2, _leds[led].getB());
	}
	else
    {
		Tlc.set(led*3 + 1, _leds[led].getR());
		Tlc.set(led*3 + 2, _leds[led].getG());
		Tlc.set(led*3 + 3, _leds[led].getB());
	}
}

void LEDBoard::setLEDs()
{
	for(uint8_t i = 0; i < 5; i++)
    {
		LEDBoard::setLED(i);
	}
}

LED* LEDBoard::getLED(uint8_t led)
{
	return &_leds[led];
}

void LEDBoard::transition(uint8_t previous, uint8_t channel, uint16_t R, uint16_t G, uint16_t B)
{
    uint32_t cur = 0;
    uint8_t i = 0;
	uint16_t pR = _leds[previous].getR(), pG=_leds[previous].getG(), pB=_leds[previous].getB();
    for(i = 0; i < 64; i++)
    {
      cur += i;
      _leds[channel].setRGB((cur * R) / 4095, (cur * G) / 4095, (cur * B) / 4095);
      _leds[previous].setRGB(((4095 - cur) * pR) / 4095, ((4095 - cur) * pG) / 4095, ((4095 - cur) * pB) / 4095);
	  LEDBoard::setLED(channel);
	  LEDBoard::setLED(previous);
      Tlc.update();
	  delayMicroseconds(2500);
    }
    cur += 63;
    for(i = 63; i > 0 ; i--)
    {
      cur += i;
      _leds[channel].setRGB((cur*R) / 4095, (cur*G) / 4095, (cur*B) / 4095);
      _leds[previous].setRGB(((4095 - cur) * pR) / 4095, ((4095 - cur) * pG) / 4095, ((4095 - cur) * pB) / 4095);
	  LEDBoard::setLED(channel);
	  LEDBoard::setLED(previous);
      Tlc.update();
	  delayMicroseconds(2500);
    }   
}

void LEDBoard::transitionHSV(uint8_t previous, uint8_t channel, uint16_t H, uint16_t S, uint16_t V)
{
    uint32_t cur = 0;
    uint8_t i = 0;
	uint16_t pH = _leds[previous].getH(), pS = _leds[previous].getS(), pV = _leds[previous].getV();

    for(i = 0; i < 64; i++)
    {
      cur += i;
      _leds[channel].setHSV((cur * H) / 4095, (cur * S) / 4095, (cur * V) / 4095);
      _leds[previous].setRGB(((4095 - cur) * pH) / 4095, ((4095 - cur) * pS) / 4095, ((4095 - cur) * pV) / 4095);
	  LEDBoard::setLED(channel);
	  LEDBoard::setLED(previous);
      Tlc.update();
	  delayMicroseconds(2500);
    }
    cur += 63;
    for(i = 63; i > 0; i--)
    {
      cur += i;
      _leds[channel].setHSV((cur*H) / 4095, (cur*S) / 4095, (cur * V) / 4095);
      _leds[previous].setRGB(((4095 - cur) * pH) / 4095, ((4095 - cur) * pS) / 4095, ((4095 - cur) * pV) / 4095);
	  LEDBoard::setLED(channel);
	  LEDBoard::setLED(previous);
      Tlc.update();
	  delayMicroseconds(2500);
    }
}
