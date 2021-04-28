/*	Author: afara017
 *  Partner(s) Name: 
 *	Lab Section: 023
 *	Assignment: Lab 6  Exercise 3
 *	Exercise Description: [optional - include for your own benefit]
 *
 *	I acknowledge all content contained herein, excluding template or example
 *	code, is my own original work.
 * 
 *  Demo Link: 
 */
#include <avr/io.h>
#ifdef _SIMULATE_
#include "simAVRHeader.h"
#endif
#include "timer.h"

enum SM_STATES {Begin, Wait, Up, Down, Reset, UpHold, DownHold} SM_States;
short flag;
void Tick_Toggle() {
	switch(SM_States) {
		case Begin: 
			flag = 0;
			PORTB = 0x07;
			SM_States = Wait;
		break;
		case Wait: 
			flag = 0;
			if((~PINA & 0x03) == 0x03) {
				PORTB = 0;
				SM_States = Reset;
			}
			else if((~PINA & 0x01) == 0x01){
				if(PORTB < 0x09) {
					PORTB++;
				SM_States = Up;}}
			else if((~PINA & 0x02) == 0x02) {
				if(PORTB > 0x00) {
					PORTB--;
				SM_States = Down;}
			}
				
		break;
		case Up: 
			if((~PINA & 0x03) == 0x03)
				SM_States = Reset;
			else if((~PINA & 0x01) == 0x01)
				SM_States = UpHold;
			else 
				SM_States = Wait;
		break;
		case UpHold:
			if((~PINA & 0x03) == 0x03) {
				PORTB = 0;
				SM_States = Reset;
			}
			else if((~PINA & 0x01) == 0x01) {
					flag++;
					flag %= 10;
					if(flag == 0) {
						if(PORTB < 9)
							PORTB++;
					}
					SM_States = UpHold;
			}
			else 
				SM_States = Wait;
			break; 
		case Down:
			if((~PINA & 0x03) == 0x03) {
				PORTB = 0;
				SM_States = Reset;
			}
			else if((~PINA & 0x02) == 0x02)
				SM_States = DownHold;
			else 
				SM_States = Wait;
		break;
		case DownHold:
			if((~PINA & 0x02) == 0x02) {
					flag++;
					flag %= 10;
					if(flag == 0) {
						if(PORTB > 0)
							PORTB--;
					}
					SM_States = DownHold;
			}
			else 
				SM_States = Wait;
			break;
		case Reset:
			flag = 0;
			PORTB = 0x00;
			if((~PINA & 0x03) == 0x00)
				SM_States = Wait;
			else
				SM_States = Reset;
				
		break;
		default:
			SM_States = Begin;
		break;
	}


}

int main(void) {
    /* Insert DDR and PORT initializations */
	DDRA = 0x00;
	PORTA = 0xFF;
	DDRB = 0xFF;
	PORTB = 0x00;
    /* Insert your solution below */
	SM_States = Begin;
	TimerSet(100);
	TimerOn();
    while (1) {
		
		Tick_Toggle();
		while(!TimerFlag);
		TimerFlag = 0;

    }
    return 1;
}
