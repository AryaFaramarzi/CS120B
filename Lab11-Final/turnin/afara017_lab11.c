/*	Author: afara017
 *  Partner(s) Name: 
 *	Lab Section: 023
 *	Assignment: Lab 11
 *	Exercise Description: 
 *
 *	I acknowledge all content contained herein, excluding template or example
 *	code, is my own original work.
 *  Demo Link: https://youtu.be/wBbQ0Hmr_qo
 */
 
 
 /* Read here for a quick description of the game :
  * This is the dinasour game that google chrome users have certainly played before.
  * We have two button PA0 for jumping up and PA2 for jumping down
  * holding the button will result in a higher jump. 
  * If the buttons are release, the dinasour will stay in the air for 0.5 seconds before going back down
  * to the original position.
  * In order to start the game press the up button. Note that high score is also kept, everytime a highsocre
  * is reached the led connected to PB0 is turned on. 
  * Once the dinasour hits the wall, the whole LED-matrix lights on to indicate that the player has lost.
  * The walls have upward and downward orientation and also their lenghts vary from 3 to 4 pixels. These 
  * are all generated randomly.
  * Enjoy.
  */
#include <avr/io.h>
#include <stdlib.h>
#include <time.h>
#ifdef _SIMULATE_
#include "simAVRHeader.h"
#include "scheduler.h"
#include "bit.h"
#include "timer.h"
#endif
// global vars
// led matrix
unsigned char mxRow, mxCol;

// Dinasour position
unsigned char dRow, dCol;

// Wall position
unsigned char wRow, wCol;
// Wall length
unsigned char wLen;
// Wall orientation (up / down)
unsigned char orient;
// Wall on led-matrix
unsigned char wall_present = 0;

// randoms
unsigned char r, rr;

// Game started/ended
unsigned char game;

// High-score
unsigned int score, high_score = 0;

// Buttons
unsigned char up, down, start;

// general 
unsigned char jump_count, duck_count;


enum Dinasour_States {Begin_Dinasour, Wait_Dinasour, Jump_Dinasour, Long_Jump, Duck_Dinasour, Long_Duck};
int Dinasour(int state) {
		up = ((~PINA & 0x01) == 0x01); // PA0
		down = ((~PINA & 0x04) == 0x04); // PA2
		switch(state) {
				case Begin_Dinasour:
					if(up && game)
						state = Wait_Dinasour;
					else
						state = Begin_Dinasour;
					break;
				case Wait_Dinasour:
					if(up && !down && game)
						state = Jump_Dinasour;
					else if(down && !up && game)
						state = Duck_Dinasour;
					else
						state = Wait_Dinasour;
					break;
				case Jump_Dinasour:
					if(up){
						jump_count = 0;
						state = Long_Jump;
					}
					else if(jump_count >= 5)
						state = Wait_Dinasour;
					else state = Jump_Dinasour;
					break;
				case Long_Jump:
					if(jump_count >= 5)
						state = Wait_Dinasour;
					else
						state = Long_Jump;
					break;
				case Duck_Dinasour:
					if(down){
						duck_count = 0;
						state = Long_Duck;
					}
					else if(duck_count >= 5)
						state = Wait_Dinasour;
					else
						state = Duck_Dinasour;
					break;
				case Long_Duck:
					if(duck_count >= 5)
						state = Wait_Dinasour;
					else
						state = Long_Duck;
					break;
				default:
					state = Begin_Dinasour;
					break;
		}
		switch(state) {
				case Begin_Dinasour:
					dRow = 4;
					dCol = 2;
					mxRow = dRow;
					mxCol = dCol;
					break;
				case Wait_Dinasour:
					jump_count = 0;
					duck_count = 0;
					dRow = 4;
					dCol= 2;
					mxRow = dRow;
					mxCol = dCol;
					break;
				case Jump_Dinasour:
					jump_count++;
					dRow = 8;
					dCol = 2;
					mxRow = dRow;
					mxCol = dCol;
					break;
				case Long_Jump:
					jump_count++;
					dRow = 0x10;
					dCol = 2;
					mxRow = dRow;
					mxCol = dCol;
					break;
				case Duck_Dinasour:
					duck_count++;
					dRow = 2;
					dCol = 2;
					mxRow = dRow;
					mxCol = dCol;
					break;
				case Long_Duck:
					duck_count++;
					dRow = 1;
					dCol = 2;
					mxRow = dRow;
					mxCol = dCol;
					break;
		}
		return state;
}

enum Wall_States {Begin_Wall, Create_Wall, Move_Wall};
int Wall(int state)
{
	switch(state){
		case Begin_Wall:
			if(game)
				state = Create_Wall;
			break;
		case Create_Wall:
			state = Move_Wall;
			break;
		case Move_Wall:
			if(wall_present)
				state = Move_Wall;
			else
				state = Begin_Wall;
			break;
		default:
			state = Begin_Wall;
			break;
	}
	switch(state){
		case Begin_Wall:
			r = rand() % 100;
			orient = r > 50? 1 : -1;
			rr = rand() % 100;
			wLen = rr > 50? 3 : 4;
			break;
		case Create_Wall:
			wall_present = 1;
			wCol = 0x80;
			if(wLen == 3){
				if(orient == 1) // up
					wRow = 0x1c;
				else
					wRow = 0x07;}
			else if(wLen == 4){
				if(orient == 1)
					wRow = 0x1e;
				else
					wRow = 0x0F;}
			mxRow = wRow;
			mxCol = wCol;
			break;
		case Move_Wall:
			if(wCol != 1)
				wCol = wCol >> 1;
			else
				wall_present = 0;
			mxRow = wRow;
			mxCol = wCol;
			break;
		
	}
	return state;
}

enum Game_States {Menu, Play, Lose};
int Game(int state) {
		start = ((~PINA & 0x01) == 0x01);
		switch(state){
				case Menu:
					if(start)
						state = Play;
					else
						state = Menu;
					break;
				case Play:
					if(dCol == wCol && (dRow & wRow) != 0)
						state = Lose;
					else
						state = Play;
					break;
				case Lose:
					if(start)
						state = Menu;
					else
						state = Lose;
					break;
				default:
					state = Menu;
					break;
		}
		switch(state){
				case Menu:
					mxRow = 0x00;
					mxCol = 0x00;
					game = 0;
					break;
				case Play:
					game = 1;
					break;
				case Lose:
					game = 0;
					mxRow = 0xff;
					mxCol = 0xff;
					break;
		}
		return state;
}

enum Score_States {Begin_Score, Play_Score, Lose_Score};
int Score(int state)
{	switch(state){
		case Begin_Score:
			if(game)
				state = Play_Score;
			else
				state = Begin_Score;
			break;
		case Play_Score:
			if(game)
				state = Play_Score;
			else
				state = Lose_Score;
			break;
		case Lose_Score:
			if(game)
				state = Begin_Score;
			else
				state = Lose_Score;
			break;
		default:
			state = Begin_Score;
			break;
	}
	switch(state){
		case Begin_Score:
			PORTB = SetBit(PORTB, 0, 0);
			score = 0;
			break;
		case Play_Score:
			score++;
			break;
		case Lose_Score:
			if(score > high_score && score != 0){
				PORTB = SetBit(PORTB, 0, 1);
				high_score = score;
			}
			break;
	}
	return state;
}

//enum Display_states {};
enum Display_States {Display_Display};
int Display(int state)
{
	switch(Display_Display) {
			case Display_Display:
				state = Display_Display;
				break;
			default:
				state = Display_Display;
				break;
	}
	switch(Display_Display) {
			case Display_Display:
				for(int r = 0; r < 5; r++){
					for(int c = 0; c < 8; c++)
					{
						PORTC = mxCol & (1 << c);
						PORTD = ~(mxRow & (1 << r));
					}
				}
				break;
	}
	return state;	
}



int main(void) {
	// seed the rand function
	srand(time(NULL));
    /* Insert DDR and PORT initializations */
    DDRA = 0x00; PORTA = 0x0f;
    DDRB = 0xff; PORTB = 0x00;
	DDRC = 0xff; PORTC = 0x00;
	DDRD = 0xff; PORTD = 0x00;
    /* Insert your solution below */
    static task task1, task2, task3, task4, task6;
    task *tasks[] = {&task1, &task2, &task3, &task4, &task6};
    const unsigned short numTasks = sizeof(tasks) / sizeof(task*);
    
    const char start = -1;
    
    task1.state = start;
    task1.period = 200;
    task1.elapsedTime = task1.period;
    task1.TickFct = &Dinasour;
    
    task2.state = start;
    task2.period = 500;
    task2.elapsedTime = task2.period;
    task2.TickFct = &Wall;
    
    task3.state = start;
    task3.period = 100;
    task3.elapsedTime = task3.period;
    task3.TickFct = &Game;
    
    task4.state = start;
    task4.period = 100;
    task4.elapsedTime = task4.period;
    task4.TickFct = &Score;
    
    task6.state = start;
    task6.period = 1;
    task6.elapsedTime = task6.period;
    task6.TickFct = &Display;
    
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
