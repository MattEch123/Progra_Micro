/*
 * PWM2.h
 *
 * Created: 5/4/2025 5:03:42 PM
 *  Author: admin
 */ 


#ifndef PWM2_H_
#define PWM2_H_


#include <avr/io.h>

#define invert 1
#define no_invert 0


void initPWMFastA2(uint8_t invertido, uint16_t prescaler);
void updateDutyCycleA2(uint8_t dutycycle);


void initPWMFastB2(uint8_t invertido, uint16_t prescaler);
void updateDutyCycleB2(uint8_t dutycycle);



#endif /* PWM2_H_ */