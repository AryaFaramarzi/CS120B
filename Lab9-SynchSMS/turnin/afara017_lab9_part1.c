/*	Author: afara017
 *  Partner(s) Name: 
 *	Lab Section: 23
 *	Assignment: Lab 9  Exercise 1
 *	Exercise Description: [optional - include for your own benefit]
 *
 *	I acknowledge all content contained herein, excluding template or example
 *	code, is my own original work.
 * 
 * 	Demo Link: https://youtu.be/2jzyU7mMJwA
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

enum SM1_States {Begin1, First, Second, Third} SM1_States;
enum SM2_States {Begin2, Off, On} SM2_States;


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

void CombineLEDsSM() {
	PORTB = threeLEDs;
}

int main(void) {
    /* Insert DDR and PORT initializations */
	DDRB = 0xFF; PORTB = 0x00;
	
	TimerSet(1000);
	SM1_States = Begin1;
	SM2_States = Begin2;
	TimerOn();
    /* Insert your solution below */
    while (1) {
		ThreeLEDsSM();
		BlinkingLEDSM();
		CombineLEDsSM();
		//PORTB = ~PORTB;
		while(!TimerFlag);
		TimerFlag = 0;
    }
    return 1;
}
