/*	Author: lab
 *  Partner(s) Name: 
 *	Lab Section:
 *	Assignment: Lab #  Exercise #
 *	Exercise Description: [optional - include for your own benefit]
 *
 *	I acknowledge all content contained herein, excluding template or example
 *	code, is my own original work.
 */
#include <avr/io.h>
#include <stdlib.h>
#ifdef _SIMULATE_
#include "simAVRHeader.h"
#include "scheduler.h"
#include "bit.h"
#include "timer.h"
#endif
// global vars
// Dinasour position
unsigned char dRow, dCol;

// Wall position
unsigned char wRow, wCol;
// Wall orientation (up / down)
unsigned char orient;
// Wall on led-matrix
unsigned char wall_present;

// Game started/ended
unsigned char game;

// High-score
unsigned int high_score;

// Buttons
unsigned char up, down;


enum Demo_States {shift};
int Demo_Tick(int state) {

	// Local Variables
	static unsigned char pattern = 0x80;	// LED pattern - 0: LED off; 1: LED on
	static unsigned char row = 0xFE;  	// Row(s) displaying pattern. 
							// 0: display pattern on row
							// 1: do NOT display pattern on row
	// Transitions
	switch (state) {
		case shift:	
			break;
		default:	
			state = shift;
			break;
	}	
	// Actions
	switch (state) {
		case shift:	
			if (row == 0xEF && pattern == 0x01) { // Reset demo 
				pattern = 0x80;		    
				row = 0xFE;
			} else if (pattern == 0x01) { // Move LED to start of next row
				pattern = 0x80;
				row = (row << 1) | 0x01;
			} else { // Shift LED one spot to the right on current row
				pattern >>= 1;
			}
			break;
		default:
			break;
	}
	PORTC = pattern;	// Pattern to display
	PORTD = row;		// Row(s) displaying pattern	
	return state;
}


enum Dinasour_States {Begin_Dinasour, Wait_Dinasour, Jump_Dinasour, Duck_Dinasour};
int Dinasour(int state) {
		up = ((~PINA & 0x01) == 0x01); // PA0
		down = ((~PINA & 0x04) == 0x04); // PA2
		switch(state) {
				case Begin_Dinasour:
					if(up)
						state = Wait_Dinasour;
					else
						state = Begin_Dinasour;
					break;
				case Wait_Dinasour:
					break;
				case Jump_Dinasour:
					state = Wait_Dinasour;
					break;
				case Duck_Dinasour:
					state = Wait_Dinasour;
					break;
				default:
					state = Begin_Dinasour;
					break;
		}
		switch(state) {
				case Begin_Dinasour:
					dRow = 3;
					dCol = 2;
					game = 0;
					break;
				case Wait_Dinasour:
					game = 1;
					dRow = 3;
					dCol= 2;
					break;
				case Jump_Dinasour:
					dRow = 4;
					dCol = 2;
					break;
				case Duck_Dinasour:
					dRow = 2;
					dCol = 2;
					break;
		}
		return state;
}

enum Wall_States {Begin_Wall, Create_Wall, Move_Wall, };
int Wall(int state)
{
	return state;
}

//enum Score_States {};
int Score(int state)
{
	return state;
}

//enum Display_states {};
int Display(int state)
{
	return state;	
}



int main(void) {
	// seed the rand function
	srand(time(0));
    /* Insert DDR and PORT initializations */
	DDRC = 0xff; PORTC = 0x00;
	DDRD = 0xff; PORTD = 0x00;
    /* Insert your solution below */
    static task task1;
    task *tasks[] = {&task1};
    const unsigned short numTasks = sizeof(tasks) / sizeof(task*);
    
    const char start = -1;
    
    task1.state = start;
    task1.period = 100;
    task1.elapsedTime = task1.period;
    task1.TickFct = &Display;
    
    unsigned short i;
    unsigned long GCD = tasks[0]->period;
	for(i = 1; i < numTasks; i++) {
			GCD = findGCD(GCD, tasks[i]->period);
	}

    TimerSet(GCD);
    TimerOn();
    
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
    
    return 1;
}
