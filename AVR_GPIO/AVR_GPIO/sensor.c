/*
 * sensor.c
 *
 * Created: 4/21/2019 6:22:43 PM
 *  Author: Jiangtian Feng
 */ 

#include "sensor.h"
#define LED_TOGGLE			PINB |= (1<<PINB5)

/*ISR(INT0_vect){
	if (sensors[0].flag == 1){
		LED_TOGGLE;
		TCCR1B = 0;
		sensors[0].pulse = TCNT1;
		TCNT1 = 0;
		sensors[0].flag = 0;
	}
	if (sensors[0].flag == 1){
		LED_TOGGLE;
		TCCR1B |= (1<<CS10);
		sensors[0].flag = 1;
	}
}*/

/*ISR(INT1_vect){
	if (sensors[1].flag == 1){
		//LED_TOGGLE;
		TCCR3B = 0;
		sensors[1].pulse = TCNT3;
		TCNT3 = 0;
		sensors[1].flag = 0;
	}
	if (sensors[1].flag == 1){
		//LED_TOGGLE;
		TCCR3B |= (1<<CS10);
		sensors[1].flag = 1;
	}
}

ISR(PCINT0_vect){
	if (sensors[2].flag == 1){
		//LED_TOGGLE;
		TCCR4B = 0;
		sensors[2].pulse = TCNT4;
		TCNT4 = 0;
		sensors[2].flag = 0;
	}
	if (sensors[2].flag == 1){
		//LED_TOGGLE;
		TCCR4B |= (1<<CS10);
		sensors[2].flag = 1;
	}
}*/


void Sensor_Init(){
	//input(0), output(1), others: Don't care
	int idx;
	// Trig Pin Setup: PB3
	DDRB |= (1<<DDB3);
	
	// Echo Pin Setup: PD2(EXTINT0), PD3(EXTINT1), PB5(PCINT0), PE0(PCINT3) 
	//DDRD &= ~(1<<DDD3); 
	DDRD &= ~(1<<DDD2);
	//DDRB &= ~(1<<DDB4);
	//DDRE &= ~(1<<DDE0);

	// Turn on the pull-up
	PORTD |= (1<<PORTD2)/* | (1<<PORTD3)*/;
	//PORTB |= (1<<PORTB4);
	//PORTE |= (1<<PORTE0);

	_delay_ms(50);

	// Enable the Interrupt
	sei();
	EICRA |= (1<<ISC00)/* | (1<<ISC10)*/; // Set INT0 & INT1 to trigger on ANY logic change
	EIMSK |= (1<<INT0)/* | (1<<INT1)*/;  // Turns on INT0 & INT1
	
	//PCICR |= (1<<PCIE3) | (1<<PCIE0); 
	//PCMSK0 |= (1<<PCINT4);
	//PCMSK3 |= (1<<PCINT24);		

	for (idx = 0; idx < NUM_SENSOR; idx++){
		sensors[idx].pulse = 0;
		sensors[idx].flag = 0;
		sensors[idx].dist = 0;
	}

} 