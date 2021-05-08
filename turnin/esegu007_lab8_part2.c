/*	Author: Edward Segura
 *  Partner(s) Name: 
 *	Lab Section:
 *	Assignment: Lab 8  Exercise 2
 *	Exercise Description: [optional - include for your own benefit]
 *
 *	I acknowledge all content contained herein, excluding template or example
 *	code, is my own original work.
 *
 *  Demo Link: https://drive.google.com/file/d/1ZiSl9iwXITgLwNCFt_chJJwJoB4LhEG9/view?usp=sharing
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
    unsigned char i = 0x00;
    unsigned OnOff = 0x00;
    double notes[8] = {261.63, 293.66, 329.63, 349.23, 392.0, 440.0, 493.88, 523.25};
    enum SM_States {SM_Start, SM_Press, SM_Wait, SM_OnOff, SM_Inc, SM_Dec} SM_State;
    void TickFct(){
    	switch(SM_State){
    		case SM_Start:
    		SM_State = SM_Wait;
    		break;
    		case SM_Wait:
    		if(button0 && !button1 && !button2){
    			SM_State = SM_OnOff;
    		}
    		else if(!button0 && !button1 && button2){
    			SM_State = SM_Inc;
    		}
    		else if(!button0 && button1 && !button2){
    			SM_State = SM_Dec;
    		}
    		break;
    		case SM_OnOff:
    		SM_State = SM_Press;
    		break;
    		case SM_Press: 
    		if(!button0 && !button1 && !button2){
    			SM_State = SM_Wait;
    		}
    		break;
    		case SM_Inc:
    		SM_State = SM_Press;
    		break;
    		case SM_Dec:
    		SM_State = SM_Press;
    		break;
    	}
    	switch(SM_State){
        case SM_Start:
        break;
        case SM_Press:
        break;
        case SM_Wait:
        break;
    	case SM_Inc:
    	if(i < 7){	
    		i++;
    	}
    	if(OnOff == 1){
    		set_PWM(notes[i]);
    	}
    	break;
    	case SM_Dec:
    	if(i > 0){	
    		i--;
    	}
    	if(OnOff == 1){
    		set_PWM(notes[i]);
    	}
    	break;
    	case SM_OnOff:
    	if(OnOff == 0){
    		OnOff = 1;
    		set_PWM(notes[i]);
    	}
    	else if(OnOff == 1){
    		OnOff = 0;
    		set_PWM(0);
    	}
    	break;
    }
}
	TimerOn();
	TimerSet(50);
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
