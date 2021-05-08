/*	Author: Edward Segura
 *  Partner(s) Name: 
 *	Lab Section:
 *	Assignment: Lab 8  Exercise 1
 *	Exercise Description: [optional - include for your own benefit]
 *
 *	I acknowledge all content contained herein, excluding template or example
 *	code, is my own original work.
 *
 *  Demo Link: https://drive.google.com/file/d/1HnEyF3jxvobvmvH-iv9sZZgJmoEa_fP7/view?usp=sharing
 */
#include <avr/io.h>
#include "../header/timer.h"
#ifdef _SIMULATE_
#include "simAVRHeader.h"
#endif

void set_PWM(double frequency) {
    static double current_frequency;

    if (frequency != current_frequency) {
        if(!frequency)
            TCCR3B &= 0x08;
        else
            TCCR3B |= 0x03;

        if(frequency < 0.954)
            OCR3A = 0xFFFF;
        else if (frequency > 31250)
            OCR3A = 0x0000;
        else
            OCR3A = (short) (8000000 / (128 * frequency)) - 1;
        
        TCNT3 = 0;
        current_frequency = frequency;
    }
}

void PWM_on() {
    TCCR3A = (1 << COM3A0);
    TCCR3B = (1 << WGM32) | (1 << CS31) | (1 << CS30);
    set_PWM(0);
}

void PWM_off() {
    TCCR3A = 0x00;
    TCCR3B = 0x00;
}


int main(void) {
    /* Insert DDR and PORT initializations */
    DDRA = 0x00; PORTA = 0xFF;
    DDRB = 0xFF; PORTB = 0x00;
    unsigned char button0;
    unsigned char button1;
    unsigned char button2;

    enum SM_States {SM_Start, SM_Wait, SM_C, SM_D, SM_E} SM_State;
    void TickFct(){
    	switch(SM_State){
    		case SM_Start:
    		SM_State = SM_Wait;
    		break;
    		case SM_Wait:
    		if(button0 && !button1 && !button2){
    			SM_State = SM_C;
    		}
    		else if(!button0 && button1 && !button2){
    			SM_State = SM_D;
    		}
    		else if(!button0 && !button1 && button2){
    			SM_State = SM_E;
    		}
    		else{
    			SM_State = SM_Wait;
    		}
    		break;
    		case SM_C:
    		if(button0 && !button1 && !button2){
    			SM_State = SM_C;
    		}
    		else{
    			SM_State = SM_Wait;
    		}
    		break;
    		case SM_D:
    		if(!button0 && button1 && !button2){
    			SM_State = SM_D;
    		}
    		else{
    			SM_State = SM_Wait;
    		}
    		break;
    		case SM_E:
    		if(!button0 && !button1 && button2){
    			SM_State = SM_E;
    		}
    		else{
    			SM_State = SM_Wait;
    		}
    		break;
    		default:
    		break;
    	}
    	switch(SM_State){
    		case SM_Wait:
    		set_PWM(0);
    		break;
    		case SM_C:
    		set_PWM(261.63);
    		break;
    		case SM_D:
    		set_PWM(293.66);
    		break;
    		case SM_E:
    		set_PWM(329.63);
    		break;
    	}
    }
    TimerSet(100);
    TimerOn();
    SM_State = SM_Start;
    PWM_on();
    /* Insert your solution below */
    while (1) {
    	button0 = ~PINA & 0x01;
    	button1 = ~PINA & 0x02;
    	button2 = ~PINA & 0x04;
    	TickFct();
    	while(!TimerFlag);
    	TimerFlag = 0;
    }
    return 1;
}
