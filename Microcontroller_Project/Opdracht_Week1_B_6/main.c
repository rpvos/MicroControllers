#define F_CPU 8e6

#include <avr/io.h>
#include <util/delay.h>

void wait( int ms ){
	for (int i=0; i<ms; i++)
	{
		_delay_ms( 1 );		// library function (max 30 ms at 8MHz)
	}
}

int main( void ){
	
	DDRD = 0b11111111;				// All pins PORTD are set to output 
	int speedToggle = 0;			
	
	while (1)
	{
 		if (PINC & 0x01) {
 			speedToggle = !speedToggle;			// Reverse speed's state
 		}
		if (speedToggle)
		{
			//#define F_CPU 4e6????
			PORTD = 0x80;			// Turn light 7 on.
			wait(250);
			PORTD = 0x00;			// Turn all lights off
			wait(250);
		}else{
			//#define F_CPU 8 e6????
			PORTD = 0x80;			// Turn light 7 on.
			PORTD = 0x00;			// Turn all lights off
	}
	return 1;
}