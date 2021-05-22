/*	Author: afara017
 *  Partner(s) Name: 
 *	Lab Section: 023
 *	Assignment: Lab 10  Exercise 2
 *	Exercise Description: [optional - include for your own benefit]
 *
 *	I acknowledge all content contained herein, excluding template or example
 *	code, is my own original work.
 * 
 *  Demo Link: https://youtu.be/fzfxFAa0ZnE
 */
#include <avr/io.h>
#ifdef _SIMULATE_
#include "simAVRHeader.h"
#include "timer.h"
#include "bit.h"
#include "keypad.h"
#include "scheduler.h"
#endif

unsigned char lock = 0;
unsigned char corretSeq = 0;
unsigned char sequence[5] = {'1', '2', '3', '4', '5'};
unsigned char size = sizeof(sequence) / sizeof(char);
unsigned char idx = 0;

enum keypad_States {keypad_begin, keypad_wait, keypad_press, keypad_release};
enum lock_states {lock_begin, lock_off, lock_on};
enum display_States {display_display};

int keypadSMTick(int state) {
		unsigned char x = GetKeypadKey();
		switch(state) {
				case keypad_begin:
					idx = 0;
					corretSeq = 0;
					state = keypad_wait;
					break;
				case keypad_wait:
					if(x == '\0')
						state = keypad_wait;
					else
						state = keypad_press;
					break;
				case keypad_press:
					if(x == '\0')
						state = keypad_wait;
					else
						state = keypad_release;
					break;
				case keypad_release:
					if(x == '\0')
						state = keypad_wait;
					else
						state = keypad_release;
					break;
				default:
					state = keypad_begin;
					break;
		}
		switch(state) {
				case keypad_begin:
					break;
				case keypad_wait:	
					break;
				case keypad_press:
					if(x == '#' || sequence[idx] != x) {
						
						idx = 0;
					}
					else if(idx == size - 1) {
						idx = 0;
						corretSeq = 1;
					}
					else {
						
						idx++;
					}
					break;
				case keypad_release:
					break;
		}	
		return state;
}

int lockSMTick(int state) {
		switch(state) {
				case lock_begin:
					lock = 0;
					state = lock_off;
					break;
				case lock_off:
					if(corretSeq)
						state = lock_on;
					else	
						state = lock_off;
					break;
				case lock_on:
					if((~PINB & 0x80) == 0x80)
						state = lock_off;
					else
						state = lock_on;
					break;
				default:
					state = lock_begin;
					break;
		}
		switch(state) {
				case lock_begin:
					lock = 0;
					break;
				case lock_off:
					corretSeq = 0;
					lock = 0;
					break;
				case lock_on:
					lock = 1;
					break;
		}
		return state;
}

int displaySMTick(int state) {
		unsigned char output;
		switch(state) {
				case display_display: 
					state = display_display;
					break;
				default:
					state = display_display;
					break;
		}
		switch(state) {
				case display_display:
					output = lock;
					break;
		}
		PORTB = output;
		return state;
}

int main(void) {
    /* Insert DDR and PORT initializations */
    DDRA = 0x00; PORTA = 0xff;
    DDRB = 0x0f; PORTB = 0xf0;
    DDRC = 0xf0; PORTC = 0x0f;
    static task task1, task2, task3;
    task *tasks[] = {&task1, &task2, &task3}; // tasks is an array of task pointers
    const unsigned short numTasks = sizeof(tasks) / sizeof(task*);
    
    
    
    
    const char start = -1;
    
    task1.state = start;
    task1.period = 100;
    task1.elapsedTime = task1.period;
    task1.TickFct = &keypadSMTick;
    
    task2.state = start;
    task2.period = 100;
    task2.elapsedTime = task2.period;
    task2.TickFct = &lockSMTick;
    
    task3.state = start;
    task3.period = 100;
    task3.elapsedTime = task3.period;
    task3.TickFct = &displaySMTick;

	unsigned short i;
    unsigned long GCD = tasks[0]->period;
	for(i = 1; i < numTasks; i++) {
			GCD = findGCD(GCD, tasks[i]->period);
	}

    TimerSet(GCD);
    TimerOn();
    /* Insert your solution below */

    while (1) {
			for (i = 0; i < numTasks; i++) {
					if(tasks[i]->elapsedTime == tasks[i]->period) {
							tasks[i]->state = tasks[i]->TickFct(tasks[i]->state);
							tasks[i]->elapsedTime = 0;
					}
					tasks[i]->elapsedTime += GCD;
			}
			while(!TimerFlag);
			TimerFlag = 0;
		}
    
    return 0;
}
