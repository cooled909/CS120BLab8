/*	Author: Edward Segura
 *  Partner(s) Name: 
 *	Lab Section:
 *	Assignment: Lab 8  Exercise 3
 *	Exercise Description: [optional - include for your own benefit]
 *
 *	I acknowledge all content contained herein, excluding template or example
 *	code, is my own original work.
 *
 *  Demo Link: https://drive.google.com/file/d/1woE4aWqnFmINYVD3NPEC530jqYqeqCAi/view?usp=sharing
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
    unsigned char currnote = 0x01;
    unsigned char i = 0x00;
    double notes[42] = {261.63, 0, 261.63, 349.23, 392.0, 415.30, 466.16, 415.30, 261.63, 0, 261.63, 349.23, 392.0, 415.3, 261.63, 415.3, 349.23, 523.25, 466.16, 261.63, 0, 261.63, 349.23, 392.0, 415.30, 349.23, 523.25, 415.30, 698.46, 349.23, 415.3, 392.0, 349.23, 523.25, 415.3, 349.23, 261.63, 0, 261.63, 0, 261.3, 349.23};
    int length[42] = {8, 1, 3, 24, 18, 3, 3, 24, 12, 6, 6, 18, 6, 6, 6, 4, 4, 4, 36, 8, 1, 3, 21, 3, 9, 3, 9, 3, 24, 12, 4, 4, 4, 16, 4, 4, 11, 1, 8, 1, 3, 36};
    enum SM_States {SM_Start, SM_Wait, SM_Play, SM_Press} SM_State;
    void TickFct(){
    	switch(SM_State){
    		case SM_Start:
    		SM_State = SM_Wait;
    		break;
    		case SM_Wait:
    		if(button0){
    			SM_State = SM_Play;
    		}
    		break;
    		case SM_Play:
    		if(i > 0x2A){
    			i = 0x00;
    			if(button0){
    				SM_State = SM_Press;
    			}
    			else{
    				SM_State = SM_Wait;
    			}     		
    		}
    		break;
    		case SM_Press:
    		if(!button0){
    			SM_State = SM_Wait;
    		}
    	}
    	switch(SM_State){
    	case SM_Start:
    	break;
    	case SM_Press:
    	set_PWM(0);
    	break;
    	case SM_Wait:
    	set_PWM(0);
    	break;
    	case SM_Play:
    	if(currnote  == 1){	
    		set_PWM(notes[i]);
    	}
    	if(currnote > length[i]){
    		i++;
    		currnote = 0x01;
    	}
    	else{
    		currnote++;
    	}
    	break;
    }
}
	TimerOn();
	TimerSet(40);
    SM_State = SM_Start;
    PWM_on();
    /* Insert your solution below */
    while (1) {
    	button0 = ~PINA & 0x01;
    	TickFct();
    	while(!TimerFlag);
    	TimerFlag = 0;
    }
    return 1;
}
