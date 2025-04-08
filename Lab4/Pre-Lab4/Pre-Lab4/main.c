/*
 * Pre-Lab4.c
 *
 * Created: 4/2/2025 9:50:14 AM
 * Author: Matheo 
 * Description: Contador de 8 bits con 8 leds
 */
/****************************************/


// Encabezado (Libraries)
#define F_CPU 16000000 // Frecuencia de cpu en 16 M
#include <avr/io.h>
#include <avr/interrupt.h>

// Variables
uint8_t leds;

uint8_t display1;
uint8_t display2;

uint8_t timer0_ini = 178;

uint8_t tabla7seg[] = {
	0x81, 0xCF, 0x92, 0x86,
	0xCC, 0xA4, 0xA0, 0x8F,
	0x80, 0x84, 0x88, 0xE0,
	0xB1, 0xC2, 0xB0, 0xB8
};

uint8_t transistores;
	



/****************************************/
// Function prototypes
void setup();
void initADC();

/****************************************/
// Main Function

int main(void){
	setup();
	while(1){
		
		
	} 
}
/****************************************/
// NON-Interrupt subroutines
void setup(){
	cli();
	DDRD = 0xFF; //PINES DEL PUERTO D COMO SALIDA
	DDRC = 0x0F; //PINES DEL PUERTO C COMO SALIDA
	PORTC = 0x01; // ENCENDER EL TRANSISTOR EN PC0
	DDRB = 0x00; //PINES DEL PUERTO B COMO ENTRADA
	PORTB = 0xFF; //ACTICAR PULL-UPS EN PUERTO B
	
	
	CLKPR |= (1 << CLKPCE) | (1 << CLKPS2); // PRESCALER PRINCIPLAR EN 16, 1MHZ

	
	initADC();
	
	//INTERRUPCIONES
	//INTERRUPCIONES PUERTO B
	PCICR |= (1<<PCIE0); // HABILITAR LAS INTERRUPCIONES EN EL PUERTO B
	PCMSK0 |= (1<<PCINT0) | (1<<PCINT1);
	
	//INTERRUPCION TIMER 0
	TCCR0B |= (1 << CS01) | (1 << CS00);
	TCNT0 = timer0_ini;
	TIMSK0 = (1 << TOIE0);
	
	display1 = tabla7seg[0];
	display2 = tabla7seg[0];
	
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

/****************************************/

// Interrupt routines
ISR(PCINT0_vect){
	if (!(PINB & (1 << PB0))) {  // Si PB0 está en 0 (presionado)
		leds++;  // Incrementa contador
	}
	if (!(PINB & (1 << PB1))) {  // Si PB1 está en 0 (presionado)
		leds--;  // Decrementa contador
	}
}

ISR(ADC_vect){
	uint8_t adc_value = ADCH;
	display1 = tabla7seg[adc_value & 0x0F];
	display2 = tabla7seg[(adc_value & 0xF0) >> 4];
	if (ADCH > leds){
		PORTC |= (1 << PORTC3);
		}else{
		PORTC &= ~(1 << PORTC3);
	}
	
	ADCSRA |= (1 << ADSC); // Para que empice a leer otra vez
	
}

ISR(TIMER0_OVF_vect){
	TCNT0 = timer0_ini;
	switch(transistores)
	{
		case 0:
		PORTC = (PORTC & 0b00001000) | 0x01 ; // Enciende PC0, mantiene PC3-PC7
		transistores++;
		PORTD = leds;
		break;
		case 1:
		PORTC = (PORTC & 0b00001000) | 0x02; // Enciende PC1, mantiene PC3-PC7
		transistores++;
		PORTD = display1;
		break;
		case 2:
		PORTC = (PORTC & 0b00001000) | 0x04; // Enciende PC2, mantiene PC3-PC7
		transistores = 0;
		PORTD = display2;
		break;
		default:
		break;
	}
}
