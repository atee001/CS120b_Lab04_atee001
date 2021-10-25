/*	Author: Andrew Tee
 *  Partner(s) Name:
 *	Lab Section:
 *	Assignment: Lab #6  Exercise #3
 *
 *	Exercise Description: [optional - include for your own benefit]
 *
 *	I acknowledge all content contained herein, excluding template or example
 *	code, is my own original work.
 *
 * Demo Link: https://www.youtube.com/watch?v=jawkLEnIr7I&ab_channel=AndrewTee
 *
 */
#include "../header/io.h"
#include <stdio.h>
#include <avr/io.h>
#ifdef _SIMULATE_
#include "simAVRHeader.h"
#define A0 (~PINA & 0x03)
#define A1 (~PINA & 0x03)
#define LSB2 (~PINA & 0x03) //0000 0011
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






enum butt_States {start, init, incOn, decOn, incOff, decOff, reset, wait} butt_State;

void Display(){

	if(PORTB == 0x00) LCD_WriteData(0 + '0');
	else if(PORTB == 0x01) LCD_WriteData(1 + '0');
	else if(PORTB == 0x02) LCD_WriteData(2 + '0');
	else if(PORTB == 0x03) LCD_WriteData(3 + '0');
	else if(PORTB == 0x04) LCD_WriteData(4 + '0');
	else if(PORTB == 0x05) LCD_WriteData(5 + '0');
	else if(PORTB == 0x06) LCD_WriteData(6 + '0');
	else if(PORTB == 0x07) LCD_WriteData(7 + '0');
	else if(PORTB == 0x08) LCD_WriteData(8 + '0');
	else LCD_WriteData(9 + '0');

}
void Tick()
{

  Display();

  switch(butt_State) {   // Transitions

    case start:
        PORTC = 0x07;
        butt_State = wait;

        break;

    case incOff:
        if(LSB2 == 0x03) butt_State = reset;
        else if(A0 == 0x01) butt_State = incOff;
        else butt_State = wait;
        break;

      case decOff:
        if(LSB2 == 0x03) butt_State = reset;
        else if(A1 == 0x02) butt_State = decOff;
        else butt_State = wait;
        break;

      case wait:
         if(LSB2 == 0x03) {
           // PORTC = 0x00;
            butt_State = reset;
          }
         else if(A0 == 0x01) {
           PORTC = (PORTC < 9) ? PORTC + 1 : PORTC;
           butt_State = incOff;
         }
         else if(A1 == 0x02){
           PORTC = (PORTC > 0) ? PORTC - 1 : PORTC;
           butt_State = decOff;
         }
         else butt_State = wait;
         break;

      case reset:
          PORTC = 0x00;
          if(LSB2 == 0x03) butt_State = reset;
          else butt_State = wait;
          break;

      default:
        printf("Error!\n");
        break;

    switch(butt_State) {   // State Actions

          case start:

            break;

           // case init:
           //    PORTC = 0x07 ;
           //    break;
           //
           // case incOn:
           //    if(PORTC < 9) PORTC+=1;
           //    break;
           //
           // case decOn:
           //    if(PORTC > 0) PORTC-=1;
           //    break;

           case incOff:
              break;

           case decOff:
              break;

            case wait:
              break;

            case reset:
              // PORTC = 0x00;
              break;

           default:
              printf("Error!\n");
              break;
    }
  }
}







int main(void){
	DDRB = 0xff; PORTB = 0x00;
	DDRA = 0x00; PORTA = 0xff;
	TimerSet(150);
	TimerOn();
	butt_State = init;
	while(1){
		Tick();
		while(!TimerFlag){}
		TimerFlag = 0;

	}




}
