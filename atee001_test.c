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
#include <avr/interrupt.h>
#ifdef SIMULATE
#include "simAVRHeader.h"
#endif


volatile unsigned char TimerFlag = 0;

void TimerISR() {
    TimerFlag = 1;
    
}


unsigned long _avr_timer_M = 1;
unsigned long _avr_timer_cntcurr = 0;



void TimerOn() {
    
    TCCR1B = 0x0B;
    OCR1A = 125;
    TIMSK1 = 0x02;
    TCNT1 = 0;
    _avr_timer_cntcurr = _avr_timer_M;
    SREG |= 0x80;
    
}



void TimerOff() {
    
    TCCR1B = 0x00;
    
}





ISR(TIMER1_COMPA_vect) {
    
    _avr_timer_cntcurr--;
    if (_avr_timer_cntcurr == 0) {
        TimerISR();
        _avr_timer_cntcurr = _avr_timer_M;
    }
}





void TimerSet (unsigned long M) {
    _avr_timer_M = M;
    _avr_timer_cntcurr = _avr_timer_M;
}

enum LED_States {init, LED_One, LED_Two, LED_Three} Led_State;

void Tick(){

	switch(LED_State){


		case init:

			LED_State = init;
			break;

		case LED_One:
	
			LED_State = LED_Two;
			break;

		case LED_Two:

			LED_State = LED_Three
			break;

		case LED_Three:
			
			LED_State = LED_One;
			break;

		default: 
			break;


	}


	switch(LED_State){

		case LED_One:
			
			PORTB = 0x01;
			break;
		
		case LED_Two:

			PORTB = 0x02;
			break;

		case LED_Three:

			PORTB = 0x04;
			break;

		default: 
			break;

	}


}
void main(){

	TimerSet(1000);
	TimerOn();
	LED_State = init;
	while(1){
		Tick();
		while(!TimerFlag){}
		TimerFlag = 0;

	}




}
