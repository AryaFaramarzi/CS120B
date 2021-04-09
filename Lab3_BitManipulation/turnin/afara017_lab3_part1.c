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
	DDRB = 0x00; PORTB = 0x00;
	DDRC = 0xFF; PORTC = 0x00;	

	unsigned char tmpC = 0x00;
    /* Insert your solution below */
    while (1) {
	unsigned char i = 0x00;
	unsigned char mask = 0x01;
	tmpC = 0x00;
	for(; i < 0x08; i++) {
		tmpC += (mask && PINA) + (mask && PINB);
	}	

	PORTC = tmpC;
    }
    return 1;
}
