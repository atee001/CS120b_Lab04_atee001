/*	Author: atee001
 *  Partner(s) Name: 
 *	Lab Section:
 *	Assignment: Lab #  Exercise #
 *	Exercise Description: [optional - include for your own benefit]
 *
 *	I acknowledge all content contained herein, excluding template or example
 *	code, is my own original work.
 */
#include <stdio.h>
#include <avr/io.h>
#ifdef _SIMULATE_
#include "simAVRHeader.h"
#endif

enum LED_State { Start, Init, Press_off, Release_off, Press_on } LED_State;

void Tick()
{
  switch(LED_State) {   // Transitions
     case Start:  // Initial transition
        LED_State = Init;
        break;

     case Init:
        LED_State = (PINA & 0x01) ? Press_off : Init;
	break;

     case Press_off:
	LED_State = !(PINA & 0x01) ? Release_off : Press_off;
        break;

     case Release_off:
	LED_State = (PINA & 0x01) ? Press_on : Release_off;
	break;
     
     case Press_on:
	LED_State = !(PINA & 0x01) ? Init : Press_on;	
	break;

     default:
        printf("Error!\n");
        break;
  } // Transitions

  switch(LED_State) {   // Transitions
     case Start:  // Initial transition
        break;

     case Init:
        PORTB = 0x01;
        break;

     case Press_off:
	PORTB = 0x02;
        break;

     case Release_off:
	PORTB = 0x02;
        break;

     case Press_on:
	PORTB = 0x01;
        break;

     default:
        printf("Error!\n");
        break; 

  } // State actions
}

int main(void) {
   DDRB = 0xFF; PORTB = 0x00;
   DDRA = 0x00; PORTA = 0xFF;      
        // Initialize outputs
   LED_State = Start; // Indicates initial call

   while(1) {
      Tick();
   }

  return 1;
}
