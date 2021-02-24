/*
 * main.c
 *
 * Created: 2/10/2021 3:18:58 PM
 *  Author: Rik Vos
 */ 
#define F_CPU 8e6

#include <stdlib.h>
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include "lcd.h"

void wait( int ms ) {
	for (int i=0; i<ms; i++) {
		_delay_ms( 1 );		// library function (max 30 ms at 8MHz)
	}
}

int main(void)
{
	init();
	
	//wait(10);
	_delay_ms(10);
	
	clear_display();
	
	_delay_ms(20);
	
	//char text[] = "";
	//display_text(text);
	int number = 0;
	char text[1];

	
	_delay_ms(20);

	
    while(1)
    {
        PORTC ^= (1<<0);	// Toggle PORTD.7
		set_cursor(2);
		itoa(number, text, 10);
		wait( 250 );
		display_text(text);
		wait( 250 );

		clear_display();

		number++;
	
	
    }
	
	return 1;
}