/*
 * lcd.c
 *
 * Created: 10/02/2021 15:20:03
 *  Author: Rik Vos
 */ 

#define F_CPU 8e6

#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

	// A6 = E
#define  E 0x20;
	// A4 = RS
#define  RS 0x08;
	// D4/7 = data pins
	

void lcd_flush(void) {
	PORTA |= E;				// pin high but keep all other values
	_delay_ms(1);			
	PORTA &= ~E;  			// pin low but keep all other values
	_delay_ms(1);			
}

void lcd_write_char(unsigned char byte) {
	// First part
	PORTC = byte;
	PORTA = RS;
	lcd_flush();

	// Second part
	PORTC = (byte<<4);
	PORTA = RS;
	lcd_flush();
}

void init(void) {
	// Set all the needed pins to output and set them low
	DDRD = 0xFF;
	DDRA = 0xFF;
	PORTD = 0x00;
	PORTA = 0x00;

	// Step 2 set the lcd to (4-bit interface, 1 line, 5*7 Pixels)
	PORTD = 0x20;
	lcd_flush();

	// Step 3 set display mode to 0x06
	PORTC = 0x00;   
	lcd_flush();
	PORTC = 0x60;
	lcd_flush();

	// Step 4 Set cursor and display on
	PORTC = 0x00;   
	lcd_flush();
	PORTC = 0xF0;
	lcd_flush();	
}

void display_text(char *str){
	for(;*str; str++){
		lcd_write_char(*str);
	}
}

void set_cursor(int position){
	
}

