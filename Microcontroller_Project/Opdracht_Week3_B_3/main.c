/*
 * main.c
 *
 * Created: 3/7/2021 1:59:37 PM
 *  Author: Dave-
 */ 
#define F_CPU 8e6
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <xc.h>

#define BIT(x)(1 << (x))

int msCount;
int turnOff = 0;

ISR( TIMER2_COMP_vect ) {
	msCount++;
	if (msCount == 15 && turnOff == 0)
	{
		PORTD ^= BIT(7);
		msCount = 0;
		turnOff = !turnOff;
	}else if (msCount == 25 && turnOff == 1)
	{
		PORTD ^= BIT(7);
		msCount = 0;
		turnOff = !turnOff;
	}
}

void wait( int ms ) {
	for (int tms=0; tms<ms; tms++) {
		_delay_ms( 1 );			// library function (max 30 ms at 8MHz)
	}
}

void timer2Init( void ) {
	//OCR2 = CompareValue;			// Compare value of counter 2
	TIMSK |= BIT(7);				// T2 compare match interrupt enable
	sei();							// turn_on intr all
	TCCR2 = (1<<CS10) | (1<<CS12);			// Initialize T2: ext.counting, rising edge, compare output mode, CTC, RUN

}

int main(void)
{
	
	DDRD |= BIT(7);
	//DDRB |= (1 << 0); // Set  LED as  output
	timer2Init();
	
	//TCCR1B |= (1 << CS10);
	//TCCR1B  |= (1 << CS12); // Set up  timer
	
    while(1)
    {
        //TODO:: Please write your application code
		wait(10);
	
    }
}