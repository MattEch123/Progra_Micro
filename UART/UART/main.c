/*
 * UART.c
 *
 * Created: 4/10/2025 3:21:48 PM
 * Author : admin
 */ 

#include <avr/io.h>
#include <avr/interrupt.h>

void setup();
void initUART();
void writeChar(char caracter);
void writeString(char* texto);

int main(void)
{
    setup();
	writeString("Hola mundo como estan");
    while (1) 
    {
    }
}


// Funciones
void setup(){
	cli();
	initUART();
	DDRB = 0xFF;
	PORTB = 0;
	
	DDRC = 0XFF;
	PORTC = 0;
	
	sei();
}

void initUART(){
	// Paso 1: estableceer PD1 (TX) como salida y PD0 como entrada (RX)
	DDRD |= (1 << DDD1);
	DDRD |= (1 << DDD0);
	
	// Paso 2: Configurar UCSR0A
	UCSR0A = 0;
	
	// Paso 3: Configurar UCSR0B
	UCSR0B = (1 << RXCIE0) | (1 << RXEN0) | (1 << TXEN0);
	
	// Paso 4: Configurar UCSR0C
	// Configurar UART como asincrono, sin polaridad, 1 stop bit y 8 bits de datos
	UCSR0C = (1 << UCSZ01) | (1 << UCSZ00);
	
	// Paso 5: Congiurar baudrate UBR0 = 103 -> 9600 @ 16 MHz
	UBRR0 = 103;
}

void writeChar(char caracter){
	while(( UCSR0A & (1 << UDRE0)) == 0){
	}
	UDR0 = caracter;
}

ISR(USART_RX_vect){
	uint8_t temporal = UDR0;
	PORTB = temporal;
	PORTC = temporal >> 6;
}

void writeString(char* texto){
	for(uint8_t i = 0; *(texto+i) != '\0' ; i++){
		writeChar(*(texto+i));
	}
}