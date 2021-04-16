/*	Author: afara0174 Exercise 1
 *  Partner(s) Name: 
 *	Lab Section: 023
 *	Assignment: Lab 4  Exercise 2
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
			SM_States = Wait;
		break;
		case Wait: 
			if(PINA == 0x01)
				SM_States = Up;
			else if(PINA == 0x02)
				SM_States = Down;
			else if(PINA == 0x03)
				SM_States = Reset;
		break;
		case Up: 
			SM_States = Wait;
		break;
		case Down:
			SM_States = Wait;
		break;
		case Reset:
			SM_States = Wait;
		break;
		default:
			SM_States = Begin;
		break;
	}

	switch(SM_States) {
		case Begin:
			PORTC = 0x07;
		break;
		case Wait: 
			
		break;
		case Up:
			if(PORTC < 0x07)
				PORTC++;
		break;
		case Down:
			if(PORTC > 0)
				PORTC--;
		break;	
		case Reset:
			PORTC = 0x00;
		break;
	}

}

int main(void) {
    /* Insert DDR and PORT initializations */
	DDRA = 0x00;
	PORTA = 0x00;
	DDRC = 0xFF;
	PORTC = 0x00;
    /* Insert your solution below */
    while (1) {
	Tick_Toggle();

    }
    return 1;
}
