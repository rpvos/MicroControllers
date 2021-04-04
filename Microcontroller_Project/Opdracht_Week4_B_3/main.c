/*
 * main.c
 *
 * Created: 3/10/2021 10:09:27 AM
 *  Author: rikvo
 */ 

#define F_CPU 8e6

#include <avr/io.h>
#include <xc.h>
#include "lcd.h"
#include <util/delay.h>
#include <stdio.h>

void adcInit( void )
{
	ADMUX = 0b01000001;			// AREF=VCC, result left adjusted, channel1 at pin PF1
	ADCSRA = 0b11100110;		// ADC-enable, no interrupt, start, free running, division by 64
}

void wait( int ms )
{
	for (int tms=0; tms<ms; tms++)
	{
		_delay_ms( 1 );			// library function (max 30 ms at 8MHz)
	}
}
int getADC(){
	int low = ADCL;
	int hight = ADCH;
	int value = (hight<<8)+(low);
	double percentage = value/1023.0;
	double temperature = 5.0*percentage*100;
	
	return temperature;
}

int main(void)
{
	DDRF = 0x00;				// set PORTF for input (ADC)
	DDRA = 0xFF;				// set PORTA for output
	DDRB = 0xFF;				// set PORTB for output
	
	adcInit();
	init();
	
	char text[10];
	
    while(1)
    {	
        sprintf(text,"Temp: %d",getADC());
        wait( 250 );
        display_text(text);
        wait( 250 );

        clear_display();
    }
}