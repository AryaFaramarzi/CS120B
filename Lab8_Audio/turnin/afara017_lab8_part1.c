/*	Author: afara017
 *  Partner(s) Name: 
 *	Lab Section:
 *	Assignment: Lab 8  Exercise 1
 *	Exercise Description: [optional - include for your own benefit]
 *
 *	I acknowledge all content contained herein, excluding template or example
 *	code, is my own original work.
 * 
 *  Demo link: https://www.youtube.com/watch?v=b9CkYRyAA-o
 */
#include <avr/io.h>
#ifdef _SIMULATE_
#include "simAVRHeader.h"
#endif
#include "timer.h"
void set_PWM(double frequency)
{
		static double current_frequency;
		if(frequency != current_frequency) {
			if(!frequency) { TCCR3B &= 0x08; }
			else { TCCR3B |= 0x03; }
			
			if(frequency < 0.954) { OCR3A = 0xFFFF;}
			
			else if(frequency > 31250) { OCR3A = 0x0000;}
			
			else { OCR3A = (short) (8000000 / (128 * frequency)) - 1;}
			
			TCNT3 = 0;
			current_frequency = frequency;
		} 
}

void PWM_on() {
		TCCR3A = (1 << COM3A0);
		TCCR3B = (1 << WGM32) | (1 << CS31) | (1 << CS30);
		set_PWM(0);
}

void PWM_off() {
	TCCR3A = 0x00;
	TCCR3B = 0x00;
}

enum SM_States {Begin, Wait, C, D, E} SM_States;

void SM() {
		switch(SM_States) {
				case Begin:
					set_PWM(0);
					SM_States = Wait;
					break;
				case Wait:
					set_PWM(0);
					if((~PINA & 0x07) == 0x01) SM_States = C;
					else if((~PINA & 0x07) == 0x02) SM_States = D;
					else if((~PINA & 0x07) == 0x04) SM_States = E;
					break;
				case C:
					set_PWM(261.63);
					if((~PINA & 0x07) == 0x01)
						SM_States = C;
					else
						SM_States = Wait;
					break;
				case D:
					set_PWM(293.66);
					if((~PINA & 0x07) == 0x02)
						SM_States = D;
					else
						SM_States = Wait;
					break;
				case E:
					set_PWM(329.63);
					if((~PINA & 0x07) == 0x04)
						SM_States = E;
					else
						SM_States = Wait;
					break;
				default:
					SM_States = Begin;
					break;
		}
}

int main(void) {
    /* Insert DDR and PORT initializations */
	DDRA = 0x00; PORTA = 0xFF;
	DDRB = 0xFF; PORTB = 0x00;
	
	SM_States = Begin;
	TimerSet(100);
	PWM_on();
	
	TimerOn();
    /* Insert your solution below */
    while (1) {
		SM();
		while(!TimerFlag);
		TimerFlag = 0;
    }
    return 1;
}
