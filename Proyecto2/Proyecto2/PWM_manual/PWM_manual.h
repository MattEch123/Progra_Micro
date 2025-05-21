/*
 * PWM_manual.h
 *
 * Created: 4/21/2025 9:47:54 PM
 *  Author: admin
 */ 


#ifndef PWM_MANUAL_H_
#define PWM_MANUAL_H_

#include <avr/io.h>




void initPWMManualA(uint16_t prescaler);
void updateDutyCycleA(uint16_t dutycycle);
void updateDutyCycleB(uint16_t dutycycle);


#endif /* PWM_MANUAL_H_ */