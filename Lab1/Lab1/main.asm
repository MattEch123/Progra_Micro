/*******************************************************/
; Universidad del Valle de Guatemala
; IE2023: Programación de Microcontroladores;
; Lab1.asm
;
; Created: 1/30/2025 3:43:07 PM
; Author : Matheo
; Hardware ATMega 328P


.include "M328PDEF.inc" // Incluir definiciones de ATmega328P

.cseg
.org 0x0000

// Configuración de la pila
	LDI     R16, LOW(RAMEND)
	OUT     SPL, R16
	LDI     R16, HIGH(RAMEND)
	OUT     SPH, R16

// Configuración de MCU
SETUP:
	//	CONFIGURA EL CLOCK GENERAL CON UN PRESCALER DE 16 (1MHz)
	LDI R16, (1 << CLKPCE)
	STS CLKPR, R16 // Habilitar cambio de PRESCALER
	LDI R16, 0b00000100
	STS CLKPR, R16 // Configurar Prescaler a 16 F_cpu = 1MHz

    // Configurar PORTD como salida (para mostrar el contador en LEDs)
    LDI     R16, 0xFF
    OUT     DDRD, R16  // PORTD como salida
	LDI		R16, 0x00
    OUT     PORTD, R16 // Inicializar PORTD con el contador en 0

	// Configurar PORTC como salida (para mostrar el contador en LEDs)
    LDI     R16, 0xFF
    OUT     DDRC, R16  // PORTC como salida
	LDI		R16, 0x00
    OUT     PORTC, R16 // Inicializar PORTD con el contador en 0

    // Configurar PORTB como entrada con pull-ups activados
    LDI     R16, 0x00
    OUT     DDRB, R16	// PORTB como entrada
    LDI     R16, 0xFF
    OUT     PORTB, R16	// Activar pull-ups

    LDI     R17, 0xFF	// R17 almacena el estado anterior de los botones
    LDI     R20, 0x00	// Inicializar contador 1 en 0
	LDI		R21, 0x00	// INICIALIZAR CONTADOR 2 EN 0
	

// Bucle principal

LOOP:
//------------------- Anti-Rebote ----------------------------
    IN      R16, PINB   // Leer botones
    CP      R17, R16
    BREQ    LOOP        // Si no hay cambios, seguir esperando

    CALL    DELAY       // Antirebote
    IN      R16, PINB   // Leer botones otra vez
    CP      R17, R16
    BREQ    LOOP        // Si el estado no cambió después del delay, ignorar
//-----------------------------------------------------------------------------

    MOV     R17, R16    // Guardar nuevo estado de botones

    SBRS    R16, 0      // Si PB0 fue presionado (LOW)
    CALL    SUMAR1

    SBRS    R16, 1      // Si PB1 fue presionado (LOW)
    CALL    RESTAR1

	SBRS    R16, 2      // Si PB2 fue presionado (LOW)
    CALL    SUMAR2

	SBRS    R16, 3      // Si PB3 fue presionado (LOW)
    CALL    RESTAR2

	SBRS	R16, 4		// Si PB4 fue presionado (LOW)
	CALL	SUMARC		//Llamar a la funcion que suma los dos contadores

	MOV		R24, R21	//MOVER EL CONTADOR 2 A R24
	
	LSL		R24
	LSL		R24
	LSL		R24
	LSL		R24			//MOVER 4 VECES A LA IZQUIERDA R24 

	ADD		R24, R20	//SUMAR R24 AL CONTADOR 1, R24 SON LOS DOS CONTADORES

	OUT		PORTD, R24

    RJMP    LOOP



// Subrutinas
DELAY:
    LDI     R18, 255
SUBDELAY1:
    DEC     R18
    BRNE    SUBDELAY1
	RET
//    LDI     R18, 255
//SUBDELAY2:
//    DEC     R18
//    BRNE    SUBDELAY2
//    RET

// Sumar al contador 1
SUMAR1:
    CPI     R20, 0b00001111
    BREQ    OVERFLOW1
    INC     R20
    RET
OVERFLOW1:
    LDI     R20, 0x00
    RET    
//----------------------------------------

// Restar al contador 1
RESTAR1:
    CPI     R20, 0x00
    BREQ    UNDERFLOW1
    DEC     R20
    RET
UNDERFLOW1:
    LDI     R20, 0b00001111
	RET

//------------------- Contador 2 ---------------------
// Sumar al contador 2
SUMAR2:
    CPI     R21, 0b00001111
    BREQ    OVERFLOW2
    INC     R21
    RET
OVERFLOW2:
    LDI     R21, 0x00
    RET    
//----------------------------------------

// Restar al contador 2
RESTAR2:
    CPI     R21, 0x00
    BREQ    UNDERFLOW2
    DEC     R21
    RET

UNDERFLOW2:
    LDI     R21, 0b00001111
	RET

//----------------------------------------


//-------------- Sumar Contadores -----------------------
SUMARC:
	MOV		R25, R21	// MOVEMOS EL CONTADOR 2 AL REGISTRO R25, DE ESTA FORMA NOS ASEGURAMOS DE NO ALTERAR EL CONTADOR
	ADD		R25, R20	// SUMAMOS LOS CONTADORES
	SBRC	R25, 4		// SI EL BIT ESTA ENCENDIDO 4 SIGNIFICA QUE HUBO UN OVERFLOW
	RJMP	OVERFLOWC
	CBI		PORTC, 4	// APAGA EL LED DE OVERFLOW
	OUT		PORTC, R25	// MUESTRA EL RESULTADO DE LA SUMA EN LAS LEDS
	RET

OVERFLOWC:
	SBI		PORTC, 4	// ENCIENDE EL LED DE OVERFLOW
	OUT		PORTC, R25	// MUESTRA EL RESULTADO DE LA SUMA EN LAS LEDS
	RET



//Subrutinas de interrupción
