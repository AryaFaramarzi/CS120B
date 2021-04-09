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
	DDRB = 0xFE; PORTB = 0x00;
	DDRD = 0x00; PORTD = 0x00;

	unsigned int w = 0;
    /* Insert your solution below */
    while (1) {
    	w = PIND;
	w = w << 1;
	w += (PINB & 0x01);

	if(w >= 70) {
		PORTB = 0x02;
	}
    }
    return 1;
}
