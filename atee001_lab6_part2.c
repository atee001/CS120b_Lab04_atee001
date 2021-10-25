/*	Author: lab
 *  Partner(s) Name: 
 *	Lab Section:
 *	Assignment: Lab #  Exercise #
 *	Exercise Description: [optional - include for your own benefit]
 *
 *	I acknowledge all content contained herein, excluding template or example
 *	code, is my own original work.
 */

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
#define PA0 ~(PINA x 0x01)
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

enum LED_States {init, forward, backward, wait_on, wait_off} LED_State;

void Tick(){

	switch(LED_State){


		case init:
			LED_State = forward;
			break;
		case forward:
			if(PA0) LED_State = wait_on;
			else if(PORTB = 0x04){
				LED_State = backward;
			}
			else{
				PORTB = PORTB << 1;
			}
			break;

		case backward:
			if(PA0) LED_State = wait_on;
			else if(PORTB = 0x01){
				LED_State = forward;
			}
			else{
				PORTB = PORTB >> 1;
			}
			break;

		case wait_on:
			LED_State = (PAO) ? wait_on : wait_off;
			break;
		case wait_off:
			LED_State = (PAO) ? init : wait_off;
		default:
			break;

	}
	switch(LED_State){
		case init:
			PORTB = 0x01;
			break;
		default:
			break;
	}


}
void main(){
	DDRB = 0xff; PORTB = 0x00;
	TimerSet(1000);
	TimerOn();
	LED_State = init;
	while(1){
		Tick();
		while(!TimerFlag){}
		TimerFlag = 0;

	}




}
