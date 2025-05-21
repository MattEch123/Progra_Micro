/*
 * ADC.c
 *
 * Created: 4/30/2025 11:25:45 AM
 *  Author: admin
 */ 
#include "ADC.h"

void initADC(){
	
	 
	ADMUX = 0; // LIMPIAR EL REGISTRO ADMUX
	ADMUX |= (1 << REFS0); // REFERENCIA = AVCC
	ADMUX |= (1 << ADLAR); // JUSTIFICACION A LA IZQUIERDA
	ADMUX |= (1 << MUX0) | (1 << MUX1) | (1 << MUX2); // EMPEZAMOS LEYENDO ADC7
	
	ADCSRA = 0;
	ADCSRA |= (1 << ADPS1) | (1 << ADPS0); // PRESCALER DE 8
	ADCSRA |= (1 << ADEN) | (1 << ADIE); // HABILITAR INTERRUPCIONES

	ADCSRA |= (1 << ADSC); // EMPIEZA A HACER LA CONVERSION DEL PIN
}