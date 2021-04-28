/*	Author: afara017
 *  Partner(s) Name: 
 *	Lab Section:
 *	Assignment: Lab 6  Exercise 1
 *	Exercise Description: [optional - include for your own benefit]
 *
 *	I acknowledge all content contained herein, excluding template or example
 *	code, is my own original work.
 * 
 *  Demo Link: https://youtu.be/RlQ4i7eT2eE
 */
#include <avr/io.h>
#ifdef _SIMULATE_
#include "simAVRHeader.h"
#endif
#include "timer.h"

enum SM_States {Begin, First, Second, Third} SM_States;

void Toggle_Tick() {
		switch(SM_States) {
				case Begin: 
					PORTB = 0x00;
					SM_States = First;
					break;
				case First:
					PORTB = 0x01;
					SM_States = Second;
					break;
				case Second:
					PORTB = 0x02;
					SM_States = Third;
					break;
				case Third:
					PORTB = 0x04;
					SM_States = First;
					break;
				default:
					PORTB = 0x00;
					SM_States = Begin;
		}
}

int main(void) {
    /* Insert DDR and PORT initializations */
    DDRB = 0xFF; PORTB = 0x00;

    /* Insert your solution below */
    SM_States = Begin;
    TimerSet(1000);
    TimerOn();
    while (1) {
		
		Toggle_Tick();
		while(!TimerFlag);
		TimerFlag = 0;

    }
    return 1;
}
