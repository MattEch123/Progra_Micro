/*
 * Proyecto2.c
 *
 * Created: 4/30/2025 8:52:56 AM
 * Author: Pablo Echeverria
 * Description: Programación para hacer una garra robótica.
 */
/****************************************/
// Encabezado (Libraries)
#define F_CPU 16000000
#include <avr/io.h>
#include <avr./interrupt.h>
#include <util/delay.h>
#include <stdlib.h>  // Para atoi()
#include "ADC/ADC.h"
#include "PWM1/PWM1.h"
#include "PWM0/PWM0.h"
#include "PWM2/PWM2.h"
#include "EEPROM/EEPROM.h"
#include "UART/UART.h"
/****************************************/
// Variables
uint8_t mux_adc;
uint16_t duty ;
uint16_t EEPROM_guardar= 0;
uint16_t EEPROM_leer = 0;
uint8_t modo = 0;
uint8_t flag_guardar = 0;
uint8_t adc1_value;
uint8_t adc2_value;
uint8_t adc3_value;
uint8_t adc4_value;
char	in_adafruit[5];  // Buffer para almacenar los caracteres
uint8_t	index_buffer = 0;     // Índice para la posición actual en el buffer
/****************************************/
// Function prototypes
void setup();
/****************************************/
// Main Function
int main(void){
	setup();
	while (1)
	{
		if (modo == 0){
			PORTD |= (1 << PORTD7);
			PORTB &= ~(1 << PORTB0);
			//Mostrar en las leds en que array de posiciones quiero guardar  0-3
			switch(EEPROM_guardar){
				case (0):
					PORTD &= ~(1 << PORTD4);
					PORTD &= ~(1 << PORTD6);
					break;
				case(4):
					PORTD |= (1 << PORTD4);
					PORTD &= ~(1 << PORTD6);
					break;
				case(8):
					PORTD &= ~(1 << PORTD4);
					PORTD |= (1 << PORTD6);
					break;
				case(12):
					PORTD |= (1 << PORTD4) | (1 << PORTD6);
					break;
			}
			
			
		}else if(modo == 1){
			PORTD &= ~(1 << PORTD7);
			PORTB |= (1 << PORTB0);
			//Mostrar en las leds en que array de posiciones quiero descargar  0-3
			switch(EEPROM_leer){
				case (0):
					PORTD &= ~(1 << PORTD4);
					PORTD &= ~(1 << PORTD6);
					break;
				case(4):
					PORTD |= (1 << PORTD4);
					PORTD &= ~(1 << PORTD6);
					break;
				case(8):
					PORTD &= ~(1 << PORTD4);
					PORTD |= (1 << PORTD6);
					break;
				case(12):
					PORTD |= (1 << PORTD4) | (1 << PORTD6);
					break;
			}
		}else if(modo == 2){
			PORTD |= (1 << PORTD7);
			PORTB |= (1 << PORTB0);
		}
		
	}
}
/****************************************/
// NON-Interrupt subroutines
void setup(){
	cli();
	CLKPR = (1 << CLKPCE);
	CLKPR = (1 << CLKPS2); // FRECUENCIA DEL CLOCK PRINCIPAL DE 1 MHz
	// IMPORTANTE CAMBIAR EL BAUDRATE DEL UART
	
	initPWMFastA1(no_invert, 8, 2499); //Los parámetros aqui deben ser iguales, pues se usa el mismo timer
	initPWMFastB1(no_invert, 8, 2499);
	
	initPWMFastA2(no_invert, 64);
	initPWMFastB2(no_invert, 64);
	initADC();
	initUART();
	
	DDRC = 0; // PUERTO C COMO ENTRADA
	// Todo el puerto C ya esta configurado como entrada por la inicializacion del ADC
	PORTC |= (1 << PORTC1) | (1 << PORTC2) | (1 << PORTC3);
	// Habilitar interrpciones de pin_change PuertoC
	PCICR |= (1 << PCIE1);
	PCMSK1 |= (1 << PCINT9) | (1 << PCINT10) | (1 << PCINT11); 
	
	//HABILITAR PD7 Y PB0 COMO SALIDAS, SON LAS LEDS QUE MUESTRAN EN QUE MODO ESTA EL USUARIO
	DDRB |= (1 << PORTB0);
	DDRD |= (1 << PORTD7);
	
	//HABILITAR PD4 Y PD6 COMO SALIDAS, MUESTRAN EN QUE DIRECCIÓN DE LA EEPROM SE GUARDAN O LEEN COSAS
	DDRD |= (1 << PORTD4) | (1 << PORTD6);
	
	PORTD = 0;
	mux_adc = 0;
	sei();
}



/****************************************/
// Interrupt routines
ISR(PCINT1_vect){
	// Leer el estado actual de los pines si necesitas detectar flanco o nivel
	uint8_t pin_state = PINC;
	if (!(pin_state & (1 << PINC1)))
	{
		if (modo == 2){
			modo = 0;
		}else{
			modo++;
		}
	
	}else if (!(pin_state & (1 << PINC2))){
		// PC2 está en LOW
		if (modo == 0){
			writeEEPROM(adc1_value, 0 + EEPROM_guardar);
			
			writeEEPROM(adc2_value, 1 + EEPROM_guardar);
			
			writeEEPROM(adc3_value, 2 + EEPROM_guardar);
			
			writeEEPROM(adc4_value, 3 + EEPROM_guardar);
			
		}else if (modo == 1){
			uint8_t temp;
			
			temp = readEEPROM(0 + EEPROM_leer);
			updateDutyCycleA1(183 + ((uint32_t)temp * (300 - 183) / 255));
			writeChar('A');
			writeNumber8(temp);
			writeChar('\n');
			
			temp = readEEPROM(1 + EEPROM_leer);
			updateDutyCycleB1(60 + ((uint32_t)temp * (182 - 60) / 255));
			writeChar('B');
			writeNumber8(readEEPROM(temp));
			writeChar('\n');
			
			temp = readEEPROM(2 + EEPROM_leer);
			updateDutyCycleA2(7 + ((uint32_t)temp* (35 - 7) / 255));
			writeChar('C');
			writeNumber8(temp);
			writeChar('\n');
			
			temp = readEEPROM(3 + EEPROM_leer);
			updateDutyCycleB2(32 + ((uint32_t)temp * (36 - 32) / 255));
			writeChar('D');
			writeNumber8(temp);
			writeChar('\n');
			
		}
		
		
	}else if (!(pin_state & (1 << PINC3))){
		//PC3 está en LOW
		if (modo == 0){
			if (EEPROM_guardar == 12){
				EEPROM_guardar = 0;
			}else{
				EEPROM_guardar += 4;
			}
		}else if (modo == 1){
			if (EEPROM_leer == 12){
				EEPROM_leer = 0;
			}else{
				EEPROM_leer += 4;
			}
		}
	}
}



ISR(ADC_vect) {
	if (mux_adc == 0) {
		// Calcular el duty para PWM de servos (Timer1)
		adc1_value = ADCH;
		duty = 183 + ((uint32_t)adc1_value * (300 - 183) / 255);
		
		if (modo == 0){updateDutyCycleA1(duty);}  // ADC7 ? Servo A
		
		// Próximo: ADC6
		ADMUX = (1 << REFS0) | (1 << ADLAR) | (1 << MUX1) | (1 << MUX2);
		mux_adc = 1;
		_delay_us(10);

		} else if (mux_adc == 1) {
		// Calcular el duty para PWM de servos (Timer1)
		adc2_value = ADCH;
		duty = 60 + ((uint32_t)adc2_value * (182 - 60) / 255);
		
		if (modo == 0){updateDutyCycleB1(duty);}  // ADC6 ? Servo B

		// Próximo: ADC5
		ADMUX = (1 << REFS0) | (1 << ADLAR) | (1 << MUX0) | (1 << MUX2);
		mux_adc = 2;
		_delay_us(10);
		
		}else if (mux_adc == 2) {
		// Calcular el duty para PWM de servos (Timer2)
		adc3_value = ADCH;
		duty = 7 + ((uint32_t)adc3_value * (35 - 7) / 255);
		
		if (modo == 0){updateDutyCycleA2(duty);}  // ADC5 ? Servo C

		// Próximo: ADC4
		ADMUX = (1 << REFS0) | (1 << ADLAR) | (1 << MUX2);
		mux_adc = 3;
		_delay_us(10);
		
		}else if (mux_adc == 3) {
		// Calcular el duty para PWM de servos (Timer2)
		adc4_value = ADCH;
		duty = 32 + ((uint32_t)adc4_value * (36 - 32) / 255);
		
		if (modo == 0){updateDutyCycleB2(duty);}  // ADC4 ? Servo D

		// Próximo: ADC7
		ADMUX = (1 << REFS0) | (1 << ADLAR) | (1 << MUX0) | (1 << MUX1) | (1 << MUX2);
		mux_adc = 0;
		_delay_us(10); 
		}

	ADCSRA |= (1 << ADSC); // Iniciar siguiente conversión
}

ISR(USART_RX_vect){
	
	char recibido = UDR0; // Recibe el caracter por serial, uno a la vez porque es serial.
		
	if (recibido == '\n'){
		in_adafruit[index_buffer] = '\0';
		index_buffer = 0;
			
		char* temp = &in_adafruit[1]; // Esta varible guarda la ubicacion de la posicion 1 del array in_adafruit
		uint8_t adc_temp = atoi(temp); // Va a llegar hasta \0 porque de esa forma termine yo mi array, termina ir por ser un valor no numeric
			
		switch(in_adafruit[0]){
			case('A'):
			if (modo == 2) updateDutyCycleA1(183 + (adc_temp * (300 - 183) / 255));
			break;
			case('B'):
			if (modo == 2) updateDutyCycleB1(60 + (adc_temp * (182 - 60) / 255));
			break;
			case('C'):
			if (modo == 2) updateDutyCycleA2(7 + (adc_temp * (35 - 7) / 255));
			break;
			case('D'):
			if (modo == 2) updateDutyCycleB2(32 + (adc_temp * (36 - 32) / 255));
			break;
			case('E'):
				switch (adc_temp){
					case 1:
						modo = 0;
						break;
					case 2:
						modo = 1;
						break;
					case 3:
						modo = 2;
				}
				
			
			
			
			default:
				
			break;
				
		}
			
		}else{
		in_adafruit[index_buffer] = recibido;
		index_buffer++;

	}
	
	
}





