/*	Author: afara017
 *  Partner(s) Name: 
 *	Lab Section: 023
 *	Assignment: Lab 10  Exercise 4
 *	Exercise Description: [optional - include for your own benefit]
 *
 *	I acknowledge all content contained herein, excluding template or example
 *	code, is my own original work.
 * 
 *  Demo Link: https://youtu.be/HBzFqVsIfpQ
 */
#include <avr/io.h>
#ifdef _SIMULATE_
#include "simAVRHeader.h"
#endif
#include "timer.h"
#include "bit.h"
#include "keypad.h"
#include "scheduler.h"
#include "pwm.h"

// The password will be set after button and * is pressed and released and then new pass will 
// be entered followed by a #. Only one pass change is allowed.   


unsigned char lock = 0;
unsigned char corretSeq = 0;
unsigned char sequence[20] = {'1', '2', '3', '4', '5'}; // Max length of password is 20 characters
unsigned char size = 5;
double melody[6] = {392.00, 0.00, 392.00, 440.00, 392.00, 261.63};
int rhythm[6] = {2, 3, 5, 10, 11, 15};
unsigned char idx = 0;
unsigned char i = 0, r = 0, cnt = 0;
unsigned char timeSinceChange = 0, seqIdx = 0, hasChanged = 0;

enum keypad_States {keypad_begin, keypad_wait, keypad_press, keypad_release};
enum lock_states {lock_begin, lock_off, lock_on};
enum doorbell_states {Begin, Ready, Playing, PlayingFinished};
enum change_pass_states {BeginChangePass, comboPress, comboRelease, changeZone, 
	poundPush, bufferZone, changeZonePress, changeZoneRelease};
enum display_States {display_display};
enum show_pass_states {show_pass};
unsigned char chtoi(char in) {
	return (int)in - '0';
}
unsigned char j = 0;
//Debugging, ignore :D
int showPassSMTick(int state) {
		//PORTA = 0;
		PORTA = chtoi(sequence[j++]);
		//PORTA = size;
		j %= size;
		return state;
}


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
					if((~PINB & 0x80) == 0x80){
						
						state = lock_off;
					}
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


int doorbellSMTick(int state) {
		switch(state) {
				case Begin:
					set_PWM(0);
					state = Ready;
					break;
				case Ready:
					i = 0;
					r = 0;
					cnt = 0;
					set_PWM(0);
					if((~PINA & 0x80) == 0x80){
						state = Playing;
					}
					break;
				case Playing:

					set_PWM(melody[i]);
					if(i == 6)
						state = PlayingFinished;
					else
						state = Playing;
					if(cnt == rhythm[r]){
						i++;
						r++;
					}
					cnt++;
					break;
				case PlayingFinished:
				
					set_PWM(0);
					if((~PINA & 0x80) == 0x00)
						state = Ready;
					break;
				default:
					state = Begin;
					break;
		}
		return state;
}

int changePassSMTick(int state) {
		unsigned char x = GetKeypadKey();
		unsigned char b = ((~PINB & 0x80) == 0x80);
		switch(state) {
				case BeginChangePass:
					state = comboPress;
					seqIdx = 0;
					timeSinceChange = 0;
					break;
				case comboPress:
					if(x == '*' && b && !hasChanged){
						state = comboRelease;
					}
					else
						state = comboPress;
					break;
				case comboRelease:
					if(x == '*' || b)
						state = comboRelease;
					else
						state = changeZone;
					break;
				case changeZone:
					if(x == '#' || seqIdx == 20) // end of new password
							state = poundPush;
					else if(x != '\0')
						state = changeZonePress;
					else
						state = changeZone;
					break;
				case changeZonePress:
					state = changeZoneRelease;
					break;
				case changeZoneRelease:
					if(x == '\0')
						state = changeZone;
					else
						state = changeZoneRelease;
					break;
				case poundPush:
					state = bufferZone;
					break;
				case bufferZone:
					if(timeSinceChange >= 20)
						state = comboPress;
					else
						state = bufferZone;
					break;

				default:
					state = BeginChangePass;
					break;
			}
		switch(state) {
				case BeginChangePass:
					break;
				case comboPress:
					break;
				case comboRelease:
					break;
				case changeZone:
					break;
				case changeZonePress:
					sequence[seqIdx++] = x;
					break;
				case changeZoneRelease:

					break;
				case poundPush:
					PORTB = SetBit(PORTB, 1, 1);
					size = seqIdx;
					break;
				case bufferZone:
					timeSinceChange++;
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
		PORTB = SetBit(PORTB, 0, output);
		return state;
}

int main(void) {
    /* Insert DDR and PORT initializations */
    DDRA = 0x00; PORTA = 0xff;
    DDRB = 0x7f; PORTB = 0x80;
    DDRC = 0xf0; PORTC = 0x0f;
    static task task1, task2, task3, task4, task5;
    task *tasks[] = {&task1, &task2, &task3, &task4, &task5}; // tasks is an array of task pointers
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
    task3.period = 200;
    task3.elapsedTime = task3.period;
    task3.TickFct = &doorbellSMTick;

	task4.state = start;
	task4.period = 100;
	task4.elapsedTime = task4.period;
	task4.TickFct = &changePassSMTick;

	task5.state = start;
    task5.period = 100;
    task5.elapsedTime = task5.period;
    task5.TickFct = &displaySMTick;
    
	unsigned short i;
    unsigned long GCD = tasks[0]->period;
	for(i = 1; i < numTasks; i++) {
			GCD = findGCD(GCD, tasks[i]->period);
	}
	PWM_on();

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
