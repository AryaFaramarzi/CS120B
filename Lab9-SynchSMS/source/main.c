/*	Author: afara017
 *  Partner(s) Name: 
 *	Lab Section: 23
 *	Assignment: Lab 9  Exercise 4
 *	Exercise Description: [optional - include for your own benefit]
 *
 *	I acknowledge all content contained herein, excluding template or example
 *	code, is my own original work.
 * 
 * 	Demo Link: 
 */

#include <avr/io.h>
#ifdef _SIMULATE_
#include "simAVRHeader.h"
#endif

#include "timer.h"
// Sets the kth bit of x to b
unsigned char SetBit(unsigned char x, unsigned char k, unsigned char b) {
   return (b ?  (x | (0x01 << k))  :  (x & ~(0x01 << k)));
}
unsigned char threeLEDs = 0x00;
unsigned char soundBit = 0x00, limit = 0;
unsigned char freq[6] = {1, 2, 3, 4, 5, 6}, idx = 0;
enum SM1_States {Begin1, First, Second, Third} SM1_States;
enum SM2_States {Begin2, Off, On} SM2_States;
enum Sound_States {SoundOff, SoundOn, FreqUp, FreqDown} Sound_States;

void ThreeLEDsSM() {
	switch(SM1_States) {
		case Begin1:
			threeLEDs = SetBit(threeLEDs, 0, 0);
			threeLEDs = SetBit(threeLEDs, 1, 0);
			threeLEDs = SetBit(threeLEDs, 2, 0);
			SM1_States = First;
			break;
		case First:
			threeLEDs = SetBit(threeLEDs, 0, 0);
			threeLEDs = SetBit(threeLEDs, 1, 0);
			threeLEDs = SetBit(threeLEDs, 2, 0);
			threeLEDs = SetBit(threeLEDs, 0, 1);
			SM1_States = Second;
			break;
		case Second:
			threeLEDs = SetBit(threeLEDs, 0, 0);
			threeLEDs = SetBit(threeLEDs, 1, 0);
			threeLEDs = SetBit(threeLEDs, 2, 0);
			threeLEDs = SetBit(threeLEDs, 1, 1);
			SM1_States = Third;
			break;
		case Third:
			threeLEDs = SetBit(threeLEDs, 0, 0);
			threeLEDs = SetBit(threeLEDs, 1, 0);
			threeLEDs = SetBit(threeLEDs, 2, 0);
			threeLEDs = SetBit(threeLEDs, 2, 1);
			SM1_States = First;
			break;
	}
}

void BlinkingLEDSM() {
	switch(SM2_States) {
		case Begin2:
			threeLEDs = SetBit(threeLEDs, 3, 0);
			SM2_States = On;
			break;
		case Off:
			threeLEDs = SetBit(threeLEDs, 3, 0);
			SM2_States = On;
			break;
		case On:
			threeLEDs = SetBit(threeLEDs, 3, 1);
			SM2_States = Off;
			break;
	}
}
void SoundSM() {
	switch(Sound_States) {
		case SoundOff:
			limit = 0;
			soundBit = 0;
			if((~PINA & 0x07) == 0x04) {
				Sound_States = SoundOn;
			}
			else if((~PINA & 0x07) == 0x01) {
				idx++;
				idx %= 6;
				Sound_States = FreqUp;
			}
			else if((~PINA & 0x07) == 0x02) {
				idx--;
				idx += 6;
				idx %= 6;
				Sound_States = FreqDown;
			}
			else {
				Sound_States = SoundOff;
			}
			break;
		case SoundOn:
			if(limit < freq[idx]) {
				soundBit = 1;
			}
			else if(limit < freq[idx] * 2)
				soundBit = 0;
			limit++;
			limit %= freq[idx] * 2;
			if((~PINA & 0x07) == 0x04) {
				Sound_States = SoundOn;
			}
			else {
				Sound_States = SoundOff;
			}
			break;
		case FreqUp:
			if((~PINA & 0x07) == 0x01) {
				Sound_States = FreqUp;
			}
			else {
				Sound_States = SoundOff;
			}
			break;
		case FreqDown:
			if((~PINA & 0x07) == 0x02) {
				Sound_States = FreqDown;
			}
			else {
				Sound_States = SoundOff;
			}
			break;
	}
}
void CombineLEDsSM() {
	PORTB = ((soundBit << 4) | (threeLEDs));
}

int main(void) {
    /* Insert DDR and PORT initializations */
	DDRA = 0x00; PORTA = 0xFF;
	DDRB = 0xFF; PORTB = 0x00;
	
	TimerSet(1);
	TimerOn();
	SM1_States = Begin1;
	SM2_States = Begin2;
	Sound_States = SoundOff;
	unsigned long ThreeLEDsTimer = 0, BlinkingLEDsTimer = 0;
	unsigned long period = 1;
    /* Insert your solution below */
    while (1) {
		if(ThreeLEDsTimer >= 300) {
					ThreeLEDsSM();
					ThreeLEDsTimer = 0;
		}
		if(BlinkingLEDsTimer >= 1000) {
					BlinkingLEDSM();
					BlinkingLEDsTimer = 0;
		}
		SoundSM();
		CombineLEDsSM();
		while(!TimerFlag);
		TimerFlag = 0;
		ThreeLEDsTimer += period;
		BlinkingLEDsTimer += period;
    }
    return 1;
}
