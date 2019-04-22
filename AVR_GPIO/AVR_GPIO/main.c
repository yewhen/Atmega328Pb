/*
 * AVR_GPIO.c
 *
 * Created: 2019/4/20 4:22:31
 * Author : User
 */ 
#include "PWM.h"
#include "sensor.h"


#define BAUD				9600
#define F_CPU				16000000UL // 16MHz
#define MYUBBR				F_CPU/16/BAUD-1

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

static volatile int pulse = 0;
static volatile int i = 0;

ISR(INT0_vect){
	LED_TOGGLE;
	if (i == 1){
		//LED_TOGGLE;
		TCCR1B = 0;
		pulse = TCNT1;
		TCNT1 = 0;
		i = 0;
	}
	if (i == 0){
		//LED_TOGGLE;
		TCCR1B |= (1<<CS10);
		i = 1;
	}
}

int main(void){
	Sensor_Init();
    
	//DDRB |= (1<<DDB5);
	//DDRB &= ~(1<<DDB7);
	//char str[] = "Hello World!";
	//USART_Init(MYUBBR);

    while (1){
		//LED_TOGGLE;
		int c = 0;

		// Trig Signal
		PORTB |= (1<<PINB3); 
		_delay_us(15); 
		PORTB &= ~(1<<PINB3);

		//for (idx = 0; idx < NUM_SENSOR; idx++){
			//c = (pulse / 58);
			if (pulse != 0){
				//LED_TOGGLE;
				_delay_ms(500);
			}
		//}
		/*if (!(PINB & (1<<PINB7))){
			LED_ON;
		}
		else{
			LED_OFF;
		}*/
	}
	
	return 0;
}

