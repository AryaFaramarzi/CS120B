/*	Author: lab
 *  Partner(s) Name: afara017
 *	Lab Section:
 *	Assignment: Lab 7  Exercise 4
 *	Exercise Description: [optional - include for your own benefit]
 *
 *	I acknowledge all content contained herein, excluding template or example
 *	code, is my own original work.
 * 
 *  Max : 110111000 = 440
 *  Min : 0000110000 = 48
 * 
 *  Demo link: https://youtu.be/lZldwKLNuhI
 */
#include <avr/io.h>
#ifdef _SIMULATE_
#include "simAVRHeader.h"
#endif
unsigned short max = 440;
unsigned short min = 48;
void ADC_init() {
	ADCSRA |= (1 << ADEN) | (1 << ADSC) | (1 << ADATE);
}

int main(void) {
    /* Insert DDR and PORT initializations */
	DDRA = 0x00; PORTA = 0xFF;
	DDRB = 0xFF; PORTB = 0x00;
	unsigned short step = max / 8;
	ADC_init();
	
    /* Insert your solution below */
    while (1) {
		unsigned short x = ADC;
		if(x <= step)
			PORTB = 0x01;
		else if(x > step && x <= 2 * step)
			PORTB = 0x03;
		else if(x > 2 * step && x <= 3 * step)
			PORTB = 0x07;
		else if(x > 3 * step && x <= 4 * step)
			PORTB = 0x0F;
		else if(x > 4 * step && x <= 5 * step)
			PORTB = 0x1F;
		else if(x > 5 * step && x <= 6 * step)
			PORTB = 0x3F;
		else if(x > 6 * step && x <= 7 * step)
			PORTB = 0x7F;
		else if(x > 7 * step && x <= 8 * step)
			PORTB = 0xFF;
    }
    return 1;
}
