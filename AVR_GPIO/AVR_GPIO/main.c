/*
 * AVR_GPIO.c
 *
 * Created: 2019/4/20 4:22:31
 * Author : User
 */ 

#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#define F_CPU 16000000UL // 16MHz

#define LED_ON				PORTB |= (1<<PORTB5)
#define LED_OFF				PORTB &= ~(1<<PORTB5)
#define LED_TOGGLE			PINB |= (1<<PINB5)

ISR(TIMER1_COMPA_vect){
	LED_ON;
}

ISR(TIMER1_COMPB_vect){
	LED_OFF;
}


void Timer_Frequency(uint8_t freq){
	TCCR1B |= (1<<CS12) | (1<<WGM12);
	TIMSK1 |= (1<<OCIE1A);
	// =F_CPU/frequency*2*N - 1
	OCR1A = (F_CPU / (freq * 2 * 256) - 1);	
}

void PWM_Init(void){
	TCCR1B |= (1<<CS10) | (1<<WGM12); // CTC mode
	TIMSK1 |= (1<<OCIE1A) | (1<<OCIE1B);
	
	OCR1A = 800; // (F_CPU / (10000*2*1) - 1)
	OCR1B = 400;
}

int main(void){
    /* Replace with your application code */
	DDRB |= (1<<DDB5);
	DDRB &= ~(1<<DDB7);
	
	//Timer_Frequency(1);
	PWM_Init();
	sei();
	
    while (1){
		
		//if (!(PINB & (1<<PINB7))){
		//	LED_ON;
		//}
		//else{
		//	LED_OFF;
		//}
	}
	return 0;
}

