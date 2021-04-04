#define F_CPU 8e6
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include "lcd.h"

#define BIT(x)			(1 << (x))

// wait(): busy waiting for 'ms' millisecond
// Used library: util/delay.h
void wait( int ms ) {
	for (int tms=0; tms<ms; tms++) {
		_delay_ms( 1 );			// library function (max 30 ms at 8MHz)
	}
}

volatile int tenthValue = 0;
volatile int CompareValue = 100;

ISR( TIMER2_COMP_vect ) {
	TCNT2 = 0;			// Preset value
	tenthValue++;		// Increment counter
	}

// Initialize timer2
void timer2Init( void ) {
	OCR2 = CompareValue;		// Compare value of counter 2
	TIMSK |= BIT(7);			// T2 compare match interrupt enable
	sei();					// turn_on intr all
	TCCR2 = 0b00000111;		// Initialize T2: ext.counting, rising edge, compare output mode, CTC, RUN

}

void setupLCD(){
	init();
	
	//wait(10);
	_delay_ms(10);
	
	clear_display();
	
	_delay_ms(20);
	
	//char text[] = "";
	//display_text(text);

	
	_delay_ms(20);
}

int main(void) {
	
	int number = 0;
	char text[1];
	setupLCD();
	
	DDRD &= ~BIT(7);			// set PORTD.7 for input
	DDRA = 0xFF;				// set PORTB for output (shows countregister)
	DDRB = 0xFF;				// set PORTC for output (shows tenth value)
	timer2Init();

	while (1) {
		//PORTA = TCNT2;		// show value counter 2
		if (number != TCNT2)
		{
		number = TCNT2;	// show value tenth counter
		wait(10);			// every 10 ms
		
			clear_display();

			itoa(number, text, 10);
			wait( 250 );
			display_text(text);
			wait( 250 );

	}
	}

}