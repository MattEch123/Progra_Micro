/*
 * PWMB1.h
 *
 * Created: 4/9/2025 4:21:21 PM
 *  Author: admin
 */ 


#ifndef PWMB1_H_
#define PWMB1_H_

#include <avr/io.h>

#define invert 1
#define no_invert 0

void initPWMFastB1(uint8_t invertido, uint16_t prescaler, uint16_t periodo);
void updateDutyCycleB1(uint16_t dutycycle);



#endif /* PWMB1_H_ */