#define F_CPU 8e6

#include <avr/io.h>
#include <util/delay.h>

void wait( int ms )
{
	for (int i=0; i<ms; i++)
	{
		_delay_ms( 1 );		// library function (max 30 ms at 8MHz)
	}
}

int main( void )
{
	
	DDRD = 0b11111111;			// All pins PORTD are set to output 
	
	while (1)
	{
		if (PINC & 0x01) {
		PORTD = 0x80;			// Write 10000000b PORTD
		}else{
		PORTD = 0x00;			// Turn all lights off		
		}			
		
	}

	return 1;
}