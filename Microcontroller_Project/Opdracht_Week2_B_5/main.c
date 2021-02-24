/*
 * main.c
 *
 * Created: 2/10/2021 3:18:58 PM
 *  Author: Rik Vos
 */ 

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
	
	wait(10);
	
	clear_display();
	
	char text[] = "hello";
	display_text(text);
	
	
    while(1)
    {
        PORTC ^= (1<<0);	// Toggle PORTD.7
		wait( 250 );
    }
}