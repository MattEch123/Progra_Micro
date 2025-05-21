/*
 * PWM_manual.c
 *
 * Created: 4/21/2025 9:47:18 PM
 *  Author: admin
 */ 

#include "PWM_manual.h"

void initPWMManualA(uint16_t prescaler)
{
    DDRD  |= (1 << DDD2);   // PD2 como salida
    PORTD &= ~(1 << PORTD2);// LED apagado

    TCCR2A = 0;             // Modo Normal
    TCCR2B = 0;             // Limpia prescaler

    switch (prescaler)
    {
        case 1:    TCCR2B |= (1 << CS20);                                   break; // clk/1
        case 8:    TCCR2B |= (1 << CS21);                                   break; // clk/8
        case 32:   TCCR2B |= (1 << CS21) | (1 << CS20);                     break; // clk/32
        case 64:   TCCR2B |= (1 << CS22);                                   break; // clk/64
        case 128:  TCCR2B |= (1 << CS22) | (1 << CS20);                     break; // clk/128
        case 256:  TCCR2B |= (1 << CS22) | (1 << CS21);                     break; // clk/256
        case 1024: TCCR2B |= (1 << CS22) | (1 << CS21) | (1 << CS20);       break; // clk/1024
        default:   TCCR2B |= (1 << CS21);                                   break; // clk/8 por defecto
    }

    TCNT2  = 0;                                   // Contador a 0
    TIMSK2 = (1 << TOIE2);        // Habilita OVF y COMPA
                    
}


void updateDutyCycleA(uint16_t dutycycle)
{
    OCR2A = dutycycle;      // evita 0 para que el match ocurra
}