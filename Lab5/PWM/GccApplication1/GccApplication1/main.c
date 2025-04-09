/*
 * GccApplication1.c
 *
 * Created: 4/3/2025 3:05:06 PM
 * Author : admin
 */ 
#define F_CPU 16000000
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include "PWM0/PWM0.h"
#include "PWM1/PWM1.h"

// Prototypes
void setup();
void initADC();
uint16_t duty ;
int main(void)
{
	setup();
	
    while (1) 
    {
    }
}

void setup(){
	cli();
	
	CLKPR = (1 << CLKPCE);
	CLKPR = (1 << CLKPS2); // FRECUENCIA DEL CLOCK PRINCIPAL DE 1 MHz
	
	initPWMFastA1(no_invert, 8, 2500);
	
	initADC();
	
	sei();
}

void initADC(){
	ADMUX = 0; // LIMPIAR EL REGISTRO ADMUX
	ADMUX |= (1 << REFS0); // REFERENCIA = AVCC
	ADMUX |= (1 << ADLAR); // JUSTIFICACION A LA IZQUIERDA
	ADMUX |= (1 << MUX0) | (1 << MUX1) | (1 << MUX2);
	
	ADCSRA = 0;
	ADCSRA |= (1 << ADPS1) | (1 << ADPS0); // PRESCALER DE 8
	ADCSRA |= (1 << ADEN) | (1 << ADIE); // HABILITAR INTERRUPCIONES
	
	ADCSRA |= (1 << ADSC); // EMPIEZA A HACER LA CONVERSION DEL PIN
}


ISR(ADC_vect){
	uint32_t adc_value = ADCH;	
	duty = 63 + (adc_value*(300-63) /255);
	updateDutyCycleA1(duty);
	
	ADCSRA |= (1 << ADSC); // Para que empice a leer otra vez
}