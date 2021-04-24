/*    Author: afara017
 *  Partner(s) Name:
 *    Lab Section: 023
 *    Assignment: Lab 5  Exercise 3
 *    Exercise Description: [optional - include for your own benefit]
 *
 *    I acknowledge all content contained herein, excluding template or example
 *    code, is my own original work.
 */
#include <avr/io.h>
#ifdef _SIMULATE_
#include "simAVRHeader.h"
#endif
enum SM_States {Begin, Wait, Next} SM_State;
char pattern[5] = {0x00, 0xFF, 0xF0, 0x0F, 0xCC};
short i = 0;
short patternSize = 5;
void Tick_Toggle() {
    switch(SM_State) {
   	 case Begin:
   		 i = 0;
   		 PORTB = 0x00;
   		 SM_State = Wait;
   		 break;
   	 case Wait:
   		 if((~PINA & 0x01) == 0x01){
   			 SM_State = Next;
   			 PORTB = pattern[i];
   			 ++i;
   			 i %= patternSize;
   		 }
   		 else {
   		 SM_State = Wait;
   		 }
   		 break;
   	 case Next:
   		 if((~PINA & 0x01) == 0x00) {
   			 SM_State = Wait;
   		 }
   		 else {
   		 SM_State = Next;
   		 }
   		 break;
   	 default:
   		 SM_State = Begin;
   		 break;
    }

}

int main(void) {
	/* Insert DDR and PORT initializations */
    DDRA = 0x00; PORTA = 0xFF;
    DDRB = 0xFF; PORTB = 0x00;
	/* Insert your solution below */
    SM_State = Begin;
	while (1) {
    	Tick_Toggle();
	}
	return 1;
}
