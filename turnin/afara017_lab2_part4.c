/*	Author: afara017
 *  Partner(s) Name: 
 *	Lab Section: 023
 *	Assignment: Lab 2  Exercise 4
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
	DDRB = 0x00; PORTB = 0x00;
	DDRC = 0x00; PORTC = 0x00;
	DDRD = 0xFF; PORTD = 0x00;
	
	unsigned char wA = 0x00, wB = 0x00, wC = 0x00;
	unsigned char total = 0x00;
    /* Insert your solution below */
    while (1) {
    	wA = PINA;	
    	wB = PINB;	
    	wC = PINC;

	total = wA + wB + wC;
	PORTD = total;	
	PORTD = PORTD & 0xFC;

	if(total > 140) {
		PORTD = PORTD | 0x01;
	}	
	if((wA - wC > 80) || (wC - wA > 80)) {
		PORTD = PORTD | 0x02;
	}
    }
    return 1;
}
