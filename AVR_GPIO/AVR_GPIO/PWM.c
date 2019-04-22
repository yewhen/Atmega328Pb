/*
 * PWM.c
 *
 * Created: 4/21/2019 6:29:23 PM
 *  Author: Jiangtian Feng
 */ 

#include "PWM.h"
 
/*ISR(TIMER1_COMPA_vect){
	LED_ON;
}

ISR(TIMER1_COMPB_vect){
	LED_OFF;
}

ISR(TIMER0_COMPA_vect){
	uint16_t period = OCR1A;
	uint16_t duty = OCR1B;

	if (duty < period) duty++;
	else duty = 0;

	OCR1B = duty;

}*/

void PWM_Increase_duty(){
	uint16_t period = OCR1A;
	uint16_t duty = OCR1B;

	if (duty < period) duty++;
	else duty = 0;

	OCR1B = duty;
}
void PWM_Decrease_duty(){
	uint16_t period = OCR1A;
	uint16_t duty = OCR1B;

	if (duty > 0) duty--;
	else duty = 0;

	OCR1B = duty;
}

void Timer_Frequency(uint8_t freq){
	TCCR1B |= (1<<CS12) | (1<<WGM12);
	TIMSK1 |= (1<<OCIE1A);
	// =F_CPU/frequency*2*N - 1
	OCR1A = (F_CPU / (freq * 2 * 256) - 1);
}

void milliS_Timer(uint8_t milliS){
	TCCR0A |= (1<<WGM01); // CTC mode
	TCCR0B |= (1<<CS02) | (1<<CS00); //prescalar of 1024
	OCR0A = milliS*7,8125-1;
	TIMSK0 |= (1<<OCIE0A);
}

void PWM_Init(void){
	TCCR1B |= (1<<CS12) | (1<<WGM12); // CTC mode
	TIMSK1 |= (1<<OCIE1A) | (1<<OCIE1B);
	 
	OCR1A = (F_CPU / (40 * 2 * 256) - 1); // 16000000 / (20480)
	OCR1B = 800*.5;
}