/*
 * AVR_GPIO.c
 *
 * Created: 2019/4/20 4:22:31
 * Author : User
 */ 
#include "PWM.h"
#include "sensor.h"
#include <stdio.h>

#define BAUD				9600

#define MYUBBR				F_CPU/16/BAUD-1


ISR(TIMER0_COMPA_vect){
	PORTC |= (1<<PORTC0);

}

ISR(TIMER0_COMPB_vect){
	PORTC &= ~(1<<PORTC0);
}

void USART_Init(unsigned int ubrr){
	/*Set baud rate */
	UBRR0H = (unsigned char)(ubrr>>8);
	UBRR0L = (unsigned char)ubrr;
	/* Enable receiver and transmitter */
	UCSR0B = (1<<RXEN0)|(1<<TXEN0);
	/* Set frame format: 8data, 2stop bit */
	UCSR0C = (3<<UCSZ00);
}
unsigned char USART_Receive(){
	/* Wait for data to be received */
	while (!(UCSR0A & (1<<RXC0)));
	/* Get and return received data from buffer */
	return UDR0;
}
void USART_Transmit(unsigned char data){
	/* Wait for empty transmit buffer */
	while (!(UCSR0A & (1<<UDRE0)));
	/* Put data into buffer, sends the data */
	UDR0 = data;
}
void USART_putstring(char* strptr){
	while (*strptr != 0x00){
		USART_Transmit(*strptr);
		strptr++;
	}
}

ISR(INT0_vect){
	//LED_TOGGLE;
	if (sensors[0].flag == 1){
		TCCR1B = 0;
		sensors[0].pulse = TCNT1;
		TCNT1 = 0;
		sensors[0].flag = 0;
	}
	if (sensors[0].flag == 0){
		TCCR1B |= (1<<CS10);
		sensors[0].flag = 1;
	}
}

int main(void){
	Sensor_Init();
	USART_Init(MYUBBR);
	char s[100];
	int i = 500;
	sprintf(s, "%d", i);
	USART_putstring(s);

    //PWM_Init_8();
	sei();
	//DDRC |= (1<<DDC0);

	//DDRC |= (1<<DDC0);
	int prev = 0, cur = 0;
    while (1){
		int idx;
		// Trig Signal
		PORTB |= (1<<PINB3); 
		_delay_us(15); 
		PORTB &= ~(1<<PINB3);

		/*for (idx = 0; idx < NUM_SENSOR; idx++){
		
		
		}*/
		cur = sensors[0].pulse/58;
		//if (sensors[0].pulse/58 > 50){
		//	PWM_Increase_duty();
		//	_delay_ms(500);
		//}
		//prev = cur;
		/*if (!(PINB & (1<<PINB7))){
			LED_ON;
		}
		else{
			LED_OFF;
		}*/
	}
	
	return 0;
}

