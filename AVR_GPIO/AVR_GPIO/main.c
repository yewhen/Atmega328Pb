/*
 * AVR_GPIO.c
 *
 * Created: 2019/4/20 4:22:31
 * Author : User
 */ 
#include <math.h>
#include <avr/io.h>
//#include <USART_irq.h>
#include <util/delay.h>
#include <avr/interrupt.h>

#define BAUD				9600
#define F_CPU				16000000UL // 16MHz
#define MYUBBR				F_CPU/16/BAUD-1

#define LED_ON				PORTB |= (1<<PORTB5)
#define LED_OFF				PORTB &= ~(1<<PORTB5)
#define LED_TOGGLE			PINB |= (1<<PINB5)

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

void PWM_Increse_duty(){
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

int main(void){
    
	//DDRB |= (1<<DDB5);
	//DDRB &= ~(1<<DDB7);
	sei();
	char str[] = "Hello World!";
	USART_Init(MYUBBR);

	USART_putstring(str);

    while (1){
		
		/*if (!(PINB & (1<<PINB7))){
			LED_ON;
		}
		else{
			LED_OFF;
		}*/
	}
	return 0;
}

