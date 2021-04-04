/*
 * main.c
 *
 * Created: 2/10/2021 2:14:10 PM
 *  Author: rikvo
 */ 

#define F_CPU 8e6
#include <avr/io.h>
#include <util/delay.h>


void wait( int ms ) {
	for (int i=0; i<ms; i++) {
		_delay_ms( 1 );
	}
}


typedef enum {
	a = 0b00000001,
	b = 0b00000010,
	c = 0b00000100,
	d = 0b00001000,
	e = 0b00010000,
	f = 0b00100000,
	g = 0b01000000,
	h = 0b10000000,
	} LetterToSegment;
	
	LetterToSegment LightShow[] = {
		a,b,g,e,d,c,g,f
		};

int main( void ) {
	DDRC = 0b11111111;
	
	while (1==1) {
		int index = 0;
		int n = sizeof(LightShow)/sizeof(LightShow[0]);
		
		while( index < n ) {
			char lights = 0x00;
			LetterToSegment light = LightShow[index];
				lights ^= light;
			
			PORTC = lights;
			wait(250);
			index++;
		}
	}

	return 1;
}