/*
 * PWM1.h
 *
 * Created: 4/8/2025 3:10:03 PM
 *  Author: admin
 */ 


#ifndef PWMA1_H_
#define PWMA1_H_

#include <avr/io.h>

#define invert 1
#define no_invert 0


void initPWMFastA1(uint8_t invertido, uint16_t prescaler, uint16_t periodo); 
void updateDutyCycleA1(uint16_t dutycycle);



#endif /* PWMA1_H_ */