/*
 * UART.c
 *
 * Created: 4/10/2025 3:21:48 PM
 * Author : admin
 */ 

#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdlib.h> // para itoa()

void setup();
void initUART();
void initADC();
void writeChar(char caracter);
void writeString(char* texto);
void menu();
void writeNumber8(uint8_t number);

char opcion;
uint8_t flag;
uint8_t modo;
uint8_t adc_value;

int main(void)
{
	setup();
	menu();
	
	while (1)
	{
		if (flag) {
			switch(opcion) {
				case '1':
				writeString("Leyendo potenciometro...\n");
				writeNumber8(adc_value);
				writeString("\n\n");
				menu();
				modo = 0;
				break;

				case '2':
				writeString("Ingrese su caracter ASCII: \n");
				// No mostrar menú aquí. Cambiamos el modo:
				modo = 1;
				break;

				default:
				writeString("Opción no válida. Intente de nuevo.\n");
				menu();
				modo = 0;
				break;
			}
			opcion = 0;
			flag = 0;
		}
	}
}




// Funciones
void setup(){
	cli();
	initUART();
	initADC();
	DDRB = 0xFF;
	PORTB = 0;
	
	DDRC = 0X0F;
	DDRC &= ~(1<< DDC6);   // PC7 como entrada
	PORTC = 0;
	
	opcion = 0;
	flag = 0;
	modo = 0;
	
	CLKPR |= (1 << CLKPCE) | (1 << CLKPS2); // PRESCALER PRINCIPLAR EN 16, 1MHZ
	
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

void writeNumber8(uint8_t number){
	char buffer[4]; // Máx: "255\0"
	itoa(number, buffer, 10); // Convierte número a string en base 10
	writeString(buffer);
}

void writeChar(char caracter){
	while(( UCSR0A & (1 << UDRE0)) == 0){
	}
	UDR0 = caracter;
}

void writeString(char* texto){
	for(uint8_t i = 0; *(texto+i) != '\0' ; i++){
		writeChar(*(texto+i));
	}
}

void menu(){
	writeString("Seleccione una de las dos opciones \n");
	writeString("Leer potenciometro (1) \n");
	writeString("Enviar ASCI (2)\n");
}

ISR(USART_RX_vect){
	char recibido = UDR0;

	if (modo == 0) {
		opcion = recibido;
		flag = 1;
	}
	else if (modo == 1) {
		writeString("Caracter recibido: ");
		writeChar(recibido);
		PORTB = recibido;
		PORTC = recibido >> 6;
		writeString("\n");
		menu();
		modo = 0; // volver al menú
	}
}

ISR(ADC_vect) {
	adc_value = ADCH;
	ADCSRA |= (1 << ADSC); // Inicia la conversión
}