/*	Author: afara017
 *  Partner(s) Name: 
 *	Lab Section: 023
 *	Assignment: Lab 2  Exercise 1
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
	DDRB = 0xFF; PORTB = 0x00;
	
	unsigned char tmpB = 0x00, door = 0x00, light = 0x00;
    /* Insert your solution below */
    while (1) {
	door = PINA & 0x01;	
	light = PINA & 0x02;
	
	if(door == 0x00 && light == 0x00)
		tmpB = 0x00;

	else if(door == 0x01 && light == 0x00)
		tmpB = 0x01;
	
	else if(door == 0x00 && light == 0x02)
		tmpB = 0x00;

	else if(door == 0x01 && light == 0x02)
		tmpB = 0x00;
	
	PORTB = tmpB;
    }
    return 1;
}
