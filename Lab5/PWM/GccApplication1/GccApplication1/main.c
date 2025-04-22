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

#include "PWMA1/PWMA1.h"
#include "PWMB1/PWMB1.h"
#include "PWM_manual/PWM_manual.h"

// Prototypes
void setup();
void initADC();

// Varibales
uint16_t duty ;
uint8_t mux_adc; 


 uint8_t counter = 0;
 uint8_t duty_cycle_led;


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
	
	initADC();
	initPWMFastA1(no_invert, 8, 2500); //Los parámetros aqui deben ser iguales, pues se usa el mismo timer
	initPWMFastB1(no_invert, 8, 2500); 
	
	initPWMManualA(1);// Tiene que usar el mismo prescaler
 
	
	mux_adc = 0;
	sei();
}

void initADC(){
	ADMUX = 0; // LIMPIAR EL REGISTRO ADMUX
	ADMUX |= (1 << REFS0); // REFERENCIA = AVCC
	ADMUX |= (1 << ADLAR); // JUSTIFICACION A LA IZQUIERDA
	ADMUX |= (1 << MUX0) | (1 << MUX1) | (1 << MUX2); // EMPEZAMOS LEYENDO ADC7
	
	ADCSRA = 0;
	ADCSRA |= (1 << ADPS1) | (1 << ADPS0); // PRESCALER DE 8
	ADCSRA |= (1 << ADEN) | (1 << ADIE); // HABILITAR INTERRUPCIONES
	ADCSRA |= (1 << ADATE); // HABILITAR INTERRUPCIONES
	
	ADCSRA |= (1 << ADSC); // EMPIEZA A HACER LA CONVERSION DEL PIN
}


ISR(ADC_vect) {
	uint8_t adc_value = ADCH;

	// Calcular el duty para PWM de servos
	duty = 65 + ((uint32_t)adc_value * (300 - 65) / 255);

	if (mux_adc == 0) {
		updateDutyCycleA1(duty);  // ADC7 ? Servo A
		
		// Próximo: ADC6
		ADMUX = (1 << REFS0) | (1 << ADLAR) | (1 << MUX1) | (1 << MUX2);
		mux_adc = 1;
		_delay_us(10);

		} else if (mux_adc == 1) {
		updateDutyCycleB1(duty);  // ADC6 ? Servo B

		// Próximo: ADC5 (para PWM manual)
		ADMUX = (1 << REFS0) | (1 << ADLAR) | (1 << MUX0) | (1 << MUX2);
		mux_adc = 2;
		_delay_us(10);

		} else {
        duty_cycle_led = (adc_value * 25) / 255;
		ADMUX = (1 << REFS0) | (1 << ADLAR) | (1 << MUX0) | (1 << MUX1) | (1 << MUX2);
		mux_adc = 0;
		_delay_us(10);
	}

	ADCSRA |= (1 << ADSC); // Iniciar siguiente conversión
}


ISR(TIMER2_OVF_vect) {
	counter++;
	
	if (counter < duty_cycle_led) {
		PORTD |= (1 << PORTD2);  // Encender
		} else {
		PORTD &= ~(1 << PORTD2); // Apagar
	}

	if (counter == 25) {
		counter = 0;
	}
}

