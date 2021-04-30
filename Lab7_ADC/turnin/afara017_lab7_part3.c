/*	Author: lab
 *  Partner(s) Name: afara017
 *	Lab Section:
 *	Assignment: Lab 7  Exercise 3
 *	Exercise Description: [optional - include for your own benefit]
 *
 *	I acknowledge all content contained herein, excluding template or example
 *	code, is my own original work.
 * 
 *  Max : 110111000 = 440
 *  Min : 0000110000 = 48
 * 
 *  Demo link: https://youtu.be/v9b-Ca3Pezw
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
	ADC_init();
	
    /* Insert your solution below */
    while (1) {
		unsigned short x = ADC;
		if(x > (max / 2) )
			PORTB = 0x01;
		else
			PORTB = 0x00;
    }
    return 1;
}
