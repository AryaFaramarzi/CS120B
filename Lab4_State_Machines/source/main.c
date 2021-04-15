/*	Author: afara0174 Exercise 1
 *  Partner(s) Name: 
 *	Lab Section: 023
 *	Assignment: Lab #  Exercise #
 *	Exercise Description: [optional - include for your own benefit]
 *
 *	I acknowledge all content contained herein, excluding template or example
 *	code, is my own original work.
 */
#include <avr/io.h>
#ifdef _SIMULATE_
#include "simAVRHeader.h"
#endif

enum LED_STATES {Begin, First, Second} LED_States;

void Tick_Toggle() {
	switch(LED_States) {
		case Begin: 
			if(PINA == 0x01)
				LED_States = First;
		break;
		case First: 
			if(PINA == 0x01)
				LED_States = Second;
			else
				LED_States = First;
		break;
		case Begin: 
			if(PINA == 0x01)
				LED_States = First;
			else
				LED_States = Second;
		break;
		default:
			LED_States = Begin;
		break;
	}

	switch(LED_States) {
		case Begin:
			PORTB = 0x01;
		break;
		case First:
			PORTB = 0x01;
		break;
		case Second:
			PORTB = 0x02;
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
