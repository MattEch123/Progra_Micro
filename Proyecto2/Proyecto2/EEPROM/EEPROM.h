/*
 * EEPROM.h
 *
 * Created: 4/30/2025 9:11:20 AM
 *  Author: admin
 */ 


#ifndef EEPROM_H_
#define EEPROM_H_

#include <avr/io.h>
#include <avr/interrupt.h>

uint8_t readEEPROM(uint16_t direccion);
void writeEEPROM(uint8_t dato, uint16_t direccion);

uint16_t readEEPROM_16(uint16_t direccion);


#endif /* EEPROM_H_ */