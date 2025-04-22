/*
 * PWMB1.c
 *
 * Created: 4/9/2025 4:21:02 PM
 *  Author: admin
 */ 
#include "PWMB1.h"

void initPWMFastB1(uint8_t invertido, uint16_t prescaler, uint16_t periodo){
	DDRB |= (1 << DDB2);
	ICR1 = periodo;
	
	TCCR1A &= ~((1 << COM1B1) | (1 << COM1B0));
	
	if (invertido){
		TCCR1A |= (1 << COM1B1) | (1 << COM1B0);
		}else{
		TCCR1A |= (1 << COM1B1);
	}
	
	TCCR1A |= (1 << WGM11); // MODO FAST PWM
	TCCR1B = 0;
	TCCR1B |= (1 << WGM13) | (1 << WGM12);
	
	switch(prescaler){
		case 1:
		TCCR1B |= (1 << CS10);
		break;
		case 8:
		TCCR1B |= (1 << CS11);
		break;
		case 64:
		TCCR1B |= (1 << CS11) | (1 << CS10);
		break;
		case 256:
		TCCR1B |= (1 << CS12);
		break;
		case 1024:
		TCCR1B |= (1 << CS12) | (1 << CS10);
		break;
		default:
		TCCR1B |= (1 << CS10);
		break;
	}
}

void updateDutyCycleB1(uint16_t dutycycle){
	OCR1B = dutycycle;
}