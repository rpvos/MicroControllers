/*
 * main.c
 *
 * Created: 2/3/2021 11:01:35 AM
 *  Author: Nathalie
 */ 

#define F_CPU 8e6

#include <avr/io.h>
#include <util/delay.h>


void wait( int ms )
/* 
short:			Busy wait number of millisecs
inputs:			int ms (Number of millisecs to busy wait)
outputs:	
notes:			Busy wait, not very accurate. Make sure (external)
				clock value is set. This is used by _delay_ms inside
				util/delay.h
Version :    	DMK, Initial code
*******************************************************************/
{
	for (int i=0; i<ms; i++)
	{
		_delay_ms( 1 );		// library function (max 30 ms at 8MHz)
	}
}
int main(void)
{
	DDRD = 0b11111111;
    while(1)
    {
		wait(50);
        PORTD = PORTD << 1;
		if(PORTD == 0){
			PORTD = 1;
		}
    }
}