/*
 * main.c
 *
 * Created: 3/10/2021 9:56:16 AM
 *  Author: rik vos
 */ 

#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

#define BIT(x)	(1 << (x))
int enabled = 0;

ISR(INT0_vect){
	enabled = !enabled;
}

// wait(): busy waiting for 'ms' millisecond
// Used library: util/delay.h
void wait( int ms )
{
	for (int tms=0; tms<ms; tms++)
	{
		_delay_ms( 1 );			// library function (max 30 ms at 8MHz)
	}
}


// Initialize ADC: 10-bits (left justified), free running
void adcInit( void )
{
	ADMUX = 0b01100001;			// AREF=VCC, result left adjusted, channel1 at pin PF1
	ADCSRA = 0b11100110;		// ADC-enable, no interrupt, start, free running, division by 64
}


// Main program: ADC at PF1
int main( void )
{
	DDRD = 0x00;				// set PORTD for input
	DDRF = 0x00;				// set PORTF for input (ADC)
	DDRA = 0xFF;				// set PORTA for output
	DDRB = 0xFF;				// set PORTB for output
	adcInit();					// initialize ADC
	
	EICRA |= 0x0B;			// INT1 falling edge, INT0 rising edge
	EIMSK |= 0x03;			// Enable INT1 & INT0
	
	// Enable global interrupt system
	//SREG = 0x80;			// Of direct via SREG of via wrapper
	sei();

	while (1)
	{
		if(enabled){
			PORTB = ADCL;			// Show MSB/LSB (bit 10:0) of ADC
			PORTA = ADCH;
			enabled = !enabled;
		}
		wait(100);				// every 100 ms (busy waiting)
	}
}
