/*
 * PWM2.c
 *
 * Created: 5/4/2025 5:03:29 PM
 *  Author: admin
 */ 

#include "PWM2.h"

void initPWMFastA2(uint8_t invertido, uint16_t prescaler){
	DDRB |= (1 << PORTB3);  // OC2A es PB3
	
	TCCR2A &= ~((1 << COM2A1) | (1 << COM2A0));
	
	if (invertido){
		TCCR2A |= (1 << COM2A1) | (1 << COM2A0);
		} else {
		TCCR2A |= (1 << COM2A1);
	}
	
	TCCR2A |= (1 << WGM21) | (1 << WGM20); // MODO FAST PWM
	TCCR2B &= ~((1 << WGM22));             // solo WGM21 y WGM20 activos
	
	TCCR2B = 0;
	switch(prescaler){
		case 1:
		TCCR2B |= (1 << CS20);
		break;
		case 8:
		TCCR2B |= (1 << CS21);
		break;
		case 32:
		TCCR2B |= (1 << CS21) | (1 << CS20);
		break;
		case 64:
		TCCR2B |= (1 << CS22);
		break;
		case 128:
		TCCR2B |= (1 << CS22) | (1 << CS20);
		break;
		case 256:
		TCCR2B |= (1 << CS22) | (1 << CS21);
		break;
		case 1024:
		TCCR2B |= (1 << CS22) | (1 << CS21) | (1 << CS20);
		break;
		default:
		TCCR2B |= (1 << CS20);
		break;
	}
}

void initPWMFastB2(uint8_t invertido, uint16_t prescaler){
	DDRD |= (1 << PORTD3);  // OC2B es PD3
	
	TCCR2A &= ~((1 << COM2B1) | (1 << COM2B0));
	
	if (invertido){
		TCCR2A |= (1 << COM2B1) | (1 << COM2B0);
		} else {
		TCCR2A |= (1 << COM2B1);
	}
	
	TCCR2A |= (1 << WGM21) | (1 << WGM20); // MODO FAST PWM
	TCCR2B &= ~((1 << WGM22));
	
	TCCR2B = 0;
	switch(prescaler){
		case 1:
		TCCR2B |= (1 << CS20);
		break;
		case 8:
		TCCR2B |= (1 << CS21);
		break;
		case 32:
		TCCR2B |= (1 << CS21) | (1 << CS20);
		break;
		case 64:
		TCCR2B |= (1 << CS22);
		break;
		case 128:
		TCCR2B |= (1 << CS22) | (1 << CS20);
		break;
		case 256:
		TCCR2B |= (1 << CS22) | (1 << CS21);
		break;
		case 1024:
		TCCR2B |= (1 << CS22) | (1 << CS21) | (1 << CS20);
		break;
		default:
		TCCR2B |= (1 << CS20);
		break;
	}
}

void updateDutyCycleA2(uint8_t dutycycle){
	OCR2A = dutycycle;
}

void updateDutyCycleB2(uint8_t dutycycle){
	OCR2B = dutycycle;
}
