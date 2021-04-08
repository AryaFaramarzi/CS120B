/*	Author: afara017
 *  Partner(s) Name: 
 *	Lab Section: 023
 *	Assignment: Lab 2  Exercise 2
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
	
	unsigned char tmpC = 0x00;
    /* Insert your solution below */
    while (1) {
	tmpC = !(PINA & 0x01) + !((PINA & 0x02) >> 1)+ !((PINA & 0x04) >> 2) + !((PINA & 0x08) >> 3); 	
	PORTC = tmpC;
    }
    return 1;
}
