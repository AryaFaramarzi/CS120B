/*	Author: afara017
 *  Partner(s) Name: 
 *	Lab Section: 023
 *	Assignment: Lab 3  Exercise 1
 *	Exercise Description: [optional - include for your own benefit]
 *
 *	I acknowledge all content contained herein, excluding template or example
 *	code, is my own original work.
 */

#include <avr/io.h>
#ifdef _SIMULATE_
#include "simAVRHeader.h"
#endif

int main(void) {
    /* Insert DDR and PORT initializations */
	DDRA = 0x00; PORTA = 0x00;
	DDRC = 0xFF; PORTC = 0x00;	

	unsigned char tmpC = 0x00, fuel = 0, keyin = 0, seat = 0, seatbelt = 0;
    /* Insert your solution below */
    while (1) {
	tmpC = 0x00;
	keyin = ((PINA & 0x10) >> 4);
	seat = ((PINA & 0x20) >> 5);
	seatbelt = ((PINA & 0x40) >> 6);
	fuel = PINA & 0x0F;

	if(fuel == 0) {
		tmpC = 0x40;
	}
	else if (fuel <= 2) {
		tmpC = 0x60;
	}
	else if(fuel <= 4) {
		tmpC = 0x70;
	}
	else if (fuel <= 6) {
		tmpC = 0x38;
	}
	else if(fuel <= 9) {
		tmpC = 0x3C;;
	}
	else if(fuel <= 12) {
		tmpC = 0x3E;
	}
	else if(fuel <= 15) {
		tmpC = 0x3F;
	}
	if(keyin && seat && !seatbelt) {
		tmpC = tmpC | 0x80;
	}
	PORTC = tmpC;
    }
    return 1;}
