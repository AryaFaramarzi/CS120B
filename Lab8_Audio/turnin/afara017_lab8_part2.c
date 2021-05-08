/*	Author: afara017
 *  Partner(s) Name: 
 *	Lab Section:
 *	Assignment: Lab 8  Exercise 2
 *	Exercise Description: [optional - include for your own benefit]
 *
 *	I acknowledge all content contained herein, excluding template or example
 *	code, is my own original work.
 * 
 *  Demo link: https://youtu.be/YQ1x5zTEKq8
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

enum SM_States {Begin, Off, OffPress, On, OnPress, Up, UpPress, Down, DownPress} SM_States;
double freq[8] = {261.63, 293.66, 329.63, 349.23, 392.00, 440.00, 493.88, 523.25};
int idx = 0;
void SM() {
		switch(SM_States) {
				case Begin:
					set_PWM(0);
					SM_States = Off;
					break;
				case OffPress:
					idx = 0;
					set_PWM(0);
					if((~PINA & 0x07) == 0x00)
						SM_States = Off;
					break;
				case Off:
					set_PWM(0);
					if((~PINA & 0x07) == 0x01)
						SM_States = OnPress;
					break;
				case OnPress:
					set_PWM(freq[idx]);
					if((~PINA & 0x07) == 0x00)
						SM_States = On;
					break;
				case On:
					set_PWM(freq[idx]);
					if((~PINA & 0x07) == 0x01)
						SM_States = OffPress;
					else if((~PINA & 0x07) == 0x02)
						SM_States = UpPress;
					else if((~PINA & 0x07) == 0x04)
						SM_States = DownPress;
					break;
				case UpPress:
					if((~PINA & 0x07) == 0x00)
						SM_States = Up;
					break;
				case Up:
					if(idx == 7);
					else
						idx++;
					SM_States = On;
					break;
				case DownPress:
					if((~PINA & 0x07) == 0x00)
						SM_States = Down;
					break;
				case Down:
					if(idx == 0);
					else
						idx--;
					SM_States = On;
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
