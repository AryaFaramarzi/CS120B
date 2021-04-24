/*	Author: afara0174 Exercise 1
 *  Partner(s) Name: 
 *	Lab Section: 023
 *	Assignment: Lab 5  Exercise 2
 *	Exercise Description: [optional - include for your own benefit]
 *
 *	I acknowledge all content contained herein, excluding template or example
 *	code, is my own original work.
 */
#include <avr/io.h>
#ifdef _SIMULATE_
#include "simAVRHeader.h"
#endif

enum SM_STATES {Begin, Wait, Up, Down, Reset} SM_States;

void Tick_Toggle() {
	switch(SM_States) {
		case Begin: 
			PORTC = 0x07;
			SM_States = Wait;
		break;
		case Wait: 
			if((~PINA & 0x03) == 0x03) {
				PORTC = 0;
				SM_States = Reset;
			}
			else if((~PINA & 0x01) == 0x01){
				if(PORTC < 0x09)
					PORTC++;
				SM_States = Up;}
			else if((~PINA & 0x02) == 0x02) {
				if(PORTC > 0x00)
					PORTC--;
				SM_States = Down;
			}
				
		break;
		case Up: 
			if((~PINA & 0x03) == 0x03)
				SM_States = Reset;
			else if((~PINA & 0x01) == 0x01)
				SM_States = Up;
			else 
				SM_States = Wait;
		break;
		case Down:
			if((~PINA & 0x03) == 0x03)
				SM_States = Reset;
			else if((~PINA & 0x02) == 0x02)
				SM_States = Down;
			else 
				SM_States = Wait;
		break;
		case Reset:
			PORTC = 0x00;
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
	DDRC = 0xFF;
	PORTC = 0x00;
    /* Insert your solution below */
	SM_States = Begin;
    while (1) {
	Tick_Toggle();

    }
    return 1;
}
