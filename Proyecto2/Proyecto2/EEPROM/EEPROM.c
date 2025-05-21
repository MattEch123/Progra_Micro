/*
 * EEPROM.c
 *
 * Created: 4/30/2025 9:11:10 AM
 *  Author: admin
 */

#include "EEPROM.h"


void writeEEPROM(uint8_t dato, uint16_t direccion){
	cli();
	while (EECR & (1 << EEPE)); //Verifica que se quede aqui hasta que termine de escribir

	// Empezar a escribir..., ya sabemos que la EEPROm esta desocupada
	// Paso 1: establecer en que direccion quiero escribir
	EEAR = direccion;
	// Paso 2: Estbalecer que quiero escribir
	EEDR = dato;
	// Paso 3: Master write enable
	EECR |= (1 << EEMPE);
	// Paso 4: write enable
	EECR |= (1 << EEPE);
	sei();
	// Es recomendable deshabilitar interrupciones para esta funcion
}

uint8_t readEEPROM(uint16_t direccion){
	while ((EECR & (1 << EEPE)) == 1);//Verifica que se quede aqui hasta que termine de escribir

	// Empezar a leer...
	// Paso 1: decir la direccion que se quiere leer
	EEAR = direccion;
	// Paso 2: empezar lectura
	EECR |= (1 << EERE);
	// Paso 3: regresar lo que se leyo
	return EEDR;
}


