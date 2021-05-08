/*	Author: afara017
 *  Partner(s) Name: 
 *	Lab Section:
 *	Assignment: Lab 8  Exercise 3
 *	Exercise Description: [optional - include for your own benefit]
 *
 *	I acknowledge all content contained herein, excluding template or example
 *	code, is my own original work.
 * 
 *  Demo link: https://youtu.be/Me-cf0xI-Xk
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

enum SM_States {Begin, Ready, Playing, PlayingFinished} SM_States;
//Notes {C4 = 261.63, D4 = 293.66, E4 = 329.63, F4 = 349.23, G4 = 392.00, A4 = 440.00, B4 = 493.88, C5 = 523.25}
double melody[12] = {392.00, 0.00, 392.00, 440.00, 392.00, 261.63, 493.88, 
	392.00, 0.00, 392.00, 440.00, 392.00, 293.66, 261.63};
int rhythm[14] = {5, 6, 11, 21, 26, 31, 41, 50, 55, 56, 61, 71, 76, 80};
int idx = 0, r = 0, cnt = 0;
void SM() {
		switch(SM_States) {
				case Begin:
					set_PWM(0);
					SM_States = Ready;
					break;
				case Ready:
					idx = 0;
					r = 0;
					cnt = 0;
					set_PWM(0);
					if((~PINA & 0x01) == 0x01)
						SM_States = Playing;
					break;
				case Playing:
					set_PWM(melody[idx]);
					if(idx == 14)
						SM_States = PlayingFinished;
					else
						SM_States = Playing;
					if(cnt == rhythm[r]){
						idx++;
						r++;
					}
					cnt++;
					break;
				case PlayingFinished:
					set_PWM(0);
					if((~PINA & 0x01) == 0x00)
						SM_States = Ready;
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
