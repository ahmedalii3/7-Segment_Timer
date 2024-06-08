/*
 * main.c
 *
 *  Created on: May 11, 2024
 *      Author: DELL
 */

#include<avr/io.h>
#include <avr/interrupt.h>
#include<util/delay.h>
unsigned char first_digit_second  =0;
unsigned char second_digit_second =0;
unsigned char first_digit_minute  =0;
unsigned char second_digit_minute =0;
unsigned char first_digit_hour    =0;
unsigned char second_digit_hour   =0;
unsigned char seconds_counter     =0;
unsigned char minutes_counter     =0;
unsigned char hours_counter       =0;

ISR(TIMER1_COMPA_vect){
	seconds_counter++;
	if(seconds_counter == 60){
		seconds_counter =0;
		first_digit_second = 0;
		second_digit_second = 0;
		minutes_counter++;
	}else if(seconds_counter < 60){
		first_digit_second = seconds_counter%10;
		second_digit_second = seconds_counter/10;
	}
	if(minutes_counter == 60){
		minutes_counter = 0;
		first_digit_minute =  0;
		second_digit_minute = 0;
		hours_counter++;
	}else if(minutes_counter < 60){
		first_digit_minute = minutes_counter%10;
		second_digit_minute = minutes_counter/10;
	}if(hours_counter == 24){
		hours_counter = 0;
		seconds_counter =0;
		minutes_counter =0;
		first_digit_minute = 0;
		second_digit_minute = 0;
		first_digit_second = 0;
		second_digit_second = 0;
		first_digit_hour =0;
		second_digit_hour =0 ;
	}else if(hours_counter<24){
		first_digit_hour  = hours_counter%10;
		second_digit_hour = hours_counter/10;
	}
}

ISR(INT0_vect)
{
	hours_counter = 0;
	seconds_counter =0;
	minutes_counter =0;
	first_digit_minute = 0;
	second_digit_minute = 0;
	first_digit_second = 0;
	second_digit_second = 0;
	first_digit_hour =0;
	second_digit_hour =0 ;
}
ISR(INT1_vect)
{
	TCCR1B &= 0xf8;
}
ISR(INT2_vect)
{
	TCCR1B |= (1<<CS10) | (1<<CS12);
}
void Timer_1_init(void){
	TCNT1   = 0;
	OCR1A = 977;
	TIMSK  |= (1<<OCIE1A);
	TCCR1A |= (1<<FOC1A);
	TCCR1B |= (1<<WGM12) | (1<<CS10) | (1<<CS12);


}

void INT0_Init(void)
{
	DDRD  &= (~(1<<PD2));               // Configure INT0/PD2 as input pin
	MCUCR |= (1<<ISC01);   				// Trigger INT0 with the falling edge
	GICR  |= (1<<INT0);                 // Enable external interrupt pin INT0
}

void INT1_Init(void)
{
	DDRD  &= (~(1<<PD3));               // Configure INT0/PD2 as input pin
	MCUCR |= (1<<ISC10) | (1<<ISC11);   // Trigger INT1 with the raising edge
	GICR  |= (1<<INT1);                 // Enable external interrupt pin INT0
}
void INT2_Init(void)
{
	DDRB  &= (~(1<<PB2));               // Configure INT0/PD2 as input pin
	GICR  |= (1<<INT2);                 // Enable external interrupt pin INT0
}
int main(void){
	SREG |= (1<<7);
	INT0_Init();
	INT1_Init();
	INT2_Init();
	DDRC |= (0x0f);
	DDRA |= 0x01;


//	PORTA |= ( 1<<0);
	PORTA |= (0x3f);
	PORTC &= 0xf0;

	Timer_1_init();
	while(1){
		PORTA = 0xC0;
		PORTA |= (1<<0);
		PORTC = (PORTC & 0xF0) | (first_digit_second & 0x0F);
		_delay_ms(5);
		PORTA = 0xC0;
		PORTA |= (1<<1);
		PORTC = (PORTC & 0xF0) | (second_digit_second & 0x0F);
		_delay_ms(5);
		PORTA = 0xC0;
		PORTA |= (1<<2);
		PORTC = (PORTC & 0xF0) | (first_digit_minute & 0x0F);
		_delay_ms(5);
		PORTA = 0xC0;
		PORTA |= (1<<3);
		PORTC = (PORTC & 0xF0) | (second_digit_minute & 0x0F);
		_delay_ms(5);
		PORTA = 0xC0;
		PORTA |= (1<<4);
		PORTC = (PORTC & 0xF0) | (first_digit_hour & 0x0F);
		_delay_ms(5);
		PORTA = 0xC0;
		PORTA |= (1<<5);
		PORTC = (PORTC & 0xF0) | (second_digit_hour & 0x0F);
		_delay_ms(5);
	}




	return 0;
}
