/*	Author: afara017
 *  Partner(s) Name: 
 *	Lab Section: 023
 *	Assignment: Lab 6  Exercise 2
 *	Exercise Description: [optional - include for your own benefit]
 *
 *	I acknowledge all content contained herein, excluding template or example
 *	code, is my own original work.
 * 
 *  Demo Link: https://youtu.be/TptkQAEVAC4
 */
#include <avr/io.h>
#ifdef _SIMULATE_
#include "simAVRHeader.h"
#endif
#include "timer.h"

char seq[3] = {0x01, 0x02, 0x04};
short i = 0x00;
enum SM_States {Begin, LightSeq, Pause, PauseRelease, Resume} SM_States;

void Toggle_Tick() {
		switch(SM_States) {
				case Begin: 
					i = 0;
					PORTB = 0x00;
					SM_States = LightSeq;
					break;
				case LightSeq:
					PORTB = seq[i];
					i = (i + 1) % 3;
					if((~PINA & 0x01) == 0x01) {
							SM_States = Pause;
					}
					break;
				case Pause:
					if((~PINA & 0x01) == 0x01)
						SM_States = Pause;
					else
						SM_States = PauseRelease;
					break;
				case PauseRelease:
					if((~PINA & 0x01) == 0x01) {
						SM_States = Resume;
					}
					else
						SM_States = PauseRelease;
					break;
				case Resume:
					i = 0x00;
					if((~PINA & 0x01) == 0x00)
						SM_States = LightSeq;
					else
						SM_States = Resume;
					break;
				default:
					PORTB = 0x00;
					SM_States = Begin;
		}
}

int main(void) {
    /* Insert DDR and PORT initializations */
    DDRA = 0x00; PORTA = 0xFF;
    DDRB = 0xFF; PORTB = 0x00;

    /* Insert your solution below */
    SM_States = Begin;
    TimerSet(200);
    TimerOn();
    while (1) {
		
		Toggle_Tick();
		while(!TimerFlag);
		TimerFlag = 0;

    }
    return 1;
}
