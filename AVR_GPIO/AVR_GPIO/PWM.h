/*
 * PWM.h
 *
 * Created: 4/21/2019 6:29:41 PM
 *  Author: Jiangtian Feng
 */ 


#ifndef PWM_H_
#define PWM_H_

#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

#define F_CPU				16000000UL // 16MHz
#define LED_ON				PORTB |= (1<<PORTB5)
#define LED_OFF				PORTB &= ~(1<<PORTB5)
#define LED_TOGGLE			PINB |= (1<<PINB5)

void PWM_Init_16();
void PWM_Init_8();
void PWM_Increase_duty();
void PWM_Decrease_duty();
void Timer_Frequency(uint8_t freq);
void milliS_Timer(uint8_t milliS);



#endif /* PWM_H_ */