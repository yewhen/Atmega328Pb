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

uint16_t rising, falling;
int counts;
float dist = 0.0;
uint16_t us_per_count;


ISR(TIMER1_COMPA_vect){

	//Generate a 12us pulse to trigger the HR-SR04
	PORTB ^= ( 1 << PORTB2);
	_delay_us(12);
	PORTB ^= ( 1 << PORTB2);
	LED_ON;
}

ISR(TIMER1_CAPT_vect){

	if(TCCR1B & (1 << ICES1)){

		TCCR1B &= ~( 1 << ICES1);
		rising = ICR1;
	}
	else{
		TCCR1B |= (1 << ICES1);
		falling = ICR1;
		counts = falling - rising;
		dist = (float)us_per_count * counts * 10 / 580;
	}
	LED_OFF;
}


void init_ultrasonic_sensor(){

	//PORTB2 as output (Arduino PIN 10)
	DDRB |= (1 << PORTB2);

	//****TIMER1 INIT****//

	//noice reduction enable
	TCCR1B |= (1<<ICNC1);

	//Prescale to 64
	TCCR1B |= (1<<CS10) | (1<<CS11);

	//select CTC mode
	TCCR1B |= (1<<WGM12);

	//read Int. at: rising edge, falling edge
	TIMSK1 |= (1<<ICIE1) | (1<<OCIE1A);

	//read Int. at rising edge
	TCCR1B |= (1<<ICES1);

	//calculate TOP (70ms runtime for one cycle): 16MHz/64 = 25000 count/sec. = 25000/1000 = 2500 count/us / 100*70 = 17500 count/70ms
	OCR1A = 17500;

	//CPU-speed / Prescale = cycles/sec. 1sec/ freq./sec. = 4 (t= 4us/cycle)
	us_per_count = 4;

	sei();

}

float read_dist(){
	return dist;
}

int main(void){
	
	init_ultrasonic_sensor();
    
	//DDRB |= (1<<DDB5);
	//DDRB &= ~(1<<DDB7);
	//sei();
	//char str[] = "Hello World!";
	//USART_Init(MYUBBR);

	

    /*while (1){
		
		if (!(PINB & (1<<PINB7))){
			LED_ON;
		}
		else{
			LED_OFF;
		}
	}*/
	while(1){
		//printf("dist: %f", dist);
 
		//USART_putstring(&dist);
		
		//if (dist == 0.0) LED_ON;
	}
	
	return 0;
}

