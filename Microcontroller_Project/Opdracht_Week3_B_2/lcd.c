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


#define LCD_E 	6  // RA6 UNI-6
#define LCD_RS	4  // RA4 UNI-6
	
void lcd_flush(void);
void clear_display(void);
void lcd_write_command(unsigned char byte);
void lcd_write_data(unsigned char byte);
void set_cursor(int position);
void display_text(char *str);
void init(void);


void lcd_flush(void) {
	PORTA |= (1<<LCD_E);	// E high
	_delay_ms(1);			
	PORTA &= ~(1<<LCD_E);  	// E low
	_delay_ms(1);					
}

void clear_display() {
	lcd_write_command (0x01);						//Leeg display
	//lcd_flush();
	_delay_ms(2);
	lcd_write_command (0x80);	//Cursor terug naar start
	//lcd_flush();
}

void init() {
	DDRC = 0xFF;			// PORTD(7) output, PORTD(6:0) input
	PORTC = 0xFF;
	// PORTC output mode and all low (also E and RS pin)
	DDRD = 0xFF;
	DDRA = 0xFF;
	PORTC = 0x00;
	PORTA = 0x00;
	//PORTA = 0xFF;

	// Step 2 (table 12)
	PORTC = 0x20;	// function set
	lcd_flush();

	// Step 3 (table 12)
	PORTC = 0x20;   // function set
	lcd_flush();
	PORTC = 0x80;
	lcd_flush();

	// Step 4 (table 12)
	PORTC = 0x00;   // Display on/off control
	lcd_flush();
	PORTC = 0xF0;
	lcd_flush();

	// Step 4 (table 12)
	PORTC = 0x00;   // Entry mode set
	lcd_flush();
	PORTC = 0x60;
	lcd_flush();
}



void display_text(char *str){
	for(;*str; str++){
		lcd_write_data(*str);
	}
}

void lcd_write_command(unsigned char byte) {
	// First nibble.
	PORTC = byte;
	PORTA &= ~(1<<LCD_RS);
	lcd_flush();

	// Second nibble
	PORTC = (byte<<4);
	PORTA &= ~(1<<LCD_RS);
	lcd_flush();
}

void lcd_write_data(unsigned char byte) {
	// First nibble.
	PORTC = byte;
	PORTA |= (1<<LCD_RS);
	lcd_flush();

	// Second nibble
	PORTC = (byte<<4);
	PORTA |= (1<<LCD_RS);
	lcd_flush();
}

void set_cursor(int position){
	lcd_write_command(128+position);
}

