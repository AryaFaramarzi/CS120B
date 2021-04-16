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

enum SM_STATES {Begin, Wait, PreLock1, PreLock2, Unlock, Lock} SM_States;

void Tick_Toggle() {
	switch(SM_States) {
		case Begin: 
			SM_States = Wait;
		break;
		case Wait:
			if(PINA && 0x80) 
				SM_States = Lock; 
			else if(PINA == 0x04) 
				SM_States = PreLock1;
		break;
		case PreLock1: 
			if(PINA && 0x80)
				SM_States = Lock;
			else if(PINA == 0x00)
				SM_States = PreLock2;
			
			else if(PINA == 0x04)
				SM_States = PreLock1;
			else	
				SM_States = Lock;
			
		break;
		case PreLock2:
			if(PINA == 0x02)
				SM_States = Unlock;
			else
				SM_States = Lock;
		break;
		case Unlock:
			if(PINA == 0x80)
				SM_States = Lock;
			else
				SM_States = Unlock;
		break;
		case Lock:
			SM_States = Wait;	
		break;
		default:
			SM_States = Begin;
		break;
	}

	switch(SM_States) {
		case Begin:
			PORTB = 0x00;
		break;
		case Wait: 
			PORTB = 0x00;
		break;
		case PreLock1:
			PORTB = 0x00;
		break;
		case PreLock2:
			PORTB = 0x00;
		break;
		case Unlock:
			PORTB = 0x01;
		break;
		case Lock:
			PORTB = 0x00;
		break;
	}

}

int main(void) {
    /* Insert DDR and PORT initializations */
	DDRA = 0x00;
	PORTA = 0x00;
	DDRB = 0xFF;
	PORTB = 0x00;
    /* Insert your solution below */
    while (1) {
	Tick_Toggle();

    }
    return 1;
}
