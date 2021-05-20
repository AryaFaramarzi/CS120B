/*	Author: afara017
 *  Partner(s) Name: 
 *	Lab Section: 023
 *	Assignment: Lab 10  Exercise 1
 *	Exercise Description: [optional - include for your own benefit]
 *
 *	I acknowledge all content contained herein, excluding template or example
 *	code, is my own original work.
 * 
 *  Demo Link: No demo for part 1
 */
#include <avr/io.h>
#ifdef _SIMULATE_
#include "simAVRHeader.h"
#include "timer.h"
#include "bit.h"
#include "keypad.h"
#include "scheduler.h"
#endif

unsigned char led_output = 0x00;
unsigned char key_pressed = 0;



enum keypad_States {keypad_begin, keypad_wait, keypad_press};
enum led_States {led_begin, led_off, led_on};
enum display_States {display_display};

int keypadSMTick(int state) {
		unsigned char x = GetKeypadKey();
		switch(state) {
				case keypad_begin:
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
						state = keypad_press;
					break;
				default:
					state = keypad_begin;
					break;
		}
		
		switch(state) {
				case keypad_begin:
					break;
				case keypad_wait:
					key_pressed = 0;
					//PORTB = 1;
					break;
				case keypad_press:
					key_pressed = 1;
					//PORTB = 1;
					break;
		}
		
		return state;
}

int ledSMTick(int state) {
		switch(state) {
				case led_begin:
					state = led_off;
					break;
				case led_off:
					if(key_pressed)
						state = led_on;
					else
						state = led_off;
					break;
				case led_on:
					if(key_pressed)
						state = led_on;
					else
						state = led_off;
					break;
				default:
					state = led_begin;
					break;
		}
		switch(state) {
				case led_begin:
					led_output = 0;
					break;
				case led_off:
					led_output = 0;
					break;
				case led_on:
					led_output = 1;
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
					output = led_output << 7;
					break;
		}
		PORTB = output;
		return state;
}

int main(void) {
    /* Insert DDR and PORT initializations */
    DDRA = 0x00; PORTA = 0xff;
    DDRB = 0xff; PORTB = 0x00;
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
    task2.TickFct = &ledSMTick;
    
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
