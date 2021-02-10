/*
 * main.c
 *
 * Created: 2/10/2021 11:07:07 AM
 *  Author: Dave-
 */ 

#define F_CPU 8e6
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

int counter;

const unsigned char Numbers [16] = {
	0b00111111, // 0
	0b00000110, // 1
	0b01011011, // 2
	0b01001111, // 3
	0b01100110, // 4
	0b01101101, // 5
	0b01111101, // 6
	0b00000111, // 7
	0b01111111, // 8
	0b01101111, // 9
	0b01110111, //10 a
	0b01111100, //11 b
	0b00111001, //12 c
	0b01011110, //13 d
	0b01111001, //14 e
	0b01110001, //15 f
};

void wait( int ms ) {
	for (int i=0; i<ms; i++) {
		_delay_ms( 1 );		// library function (max 30 ms at 8MHz)
	}
}

void display(int digit ){
	// Calculate the size of the array
	int size = sizeof(Numbers)/ sizeof(Numbers[0]);
	// If the digit wanted is bigger then the stored numbers we display E (Error)
	if (size<digit||digit<0)
	{
		PORTC = Numbers[14];
	}else{
		PORTC = Numbers[digit];
	}
}

int main(void)
{
	// Set the first 2 buttons from D to input
	DDRD = 0xF0;
	// Set the C register as output
	DDRC = 0xFF;
	
	
	// INT1 falling edge, INT0 rising edge
	EICRA |= 0x0B;			
	// Enable INT1 & INT0
	EIMSK |= 0x03;			
	
	// Enable the interupts
	sei();
	
	display(counter);
	
    while(1)
    {
    }
	
	return 1;
}

ISR( INT0_vect ) {
	// Increment the number
	counter++;
	
	// Check if the other button is pressed
	if ((PIND & (1<<1))>>1 == 1)
	{
		counter = 0;
	}
	
	// Display the new number
	display(counter);
}

ISR( INT1_vect ) {
	// Decrement the number
	counter--;
	
	// Check if the other button is pressed
	if (PIND & 0x01 == 1)
	{
		counter = 0;
	}
	
	// Display the new number
	display(counter);
}