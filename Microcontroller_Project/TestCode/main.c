#include <avr/sfr_defs.h>
#include <avr/io.h>
#include <util/delay.h>

#define F_CPU 8e6


#define setbit(port,bit)  ((port) |=  (1<<bit)) // set bit
#define clrbit(port,bit)  ((port) &= ~(1<<bit)) // clear bit

#define REF_AVCC (1<<REFS0)  // reference = AVCC = 5 V
#define REF_INT  (1<<REFS0)|(1<<REFS1) // internal reference 2.56 V

// global variables
uint16_t adc_value0, adc_value1, adc_value2, adc_value3, adc_value4, adc_value5;


//------------------------------------------------------
uint16_t adc_read(uint8_t channel)
{
	ADMUX = REF_AVCC | channel;  // set reference and channel
	ADCSRA |= (1<<ADSC);         // start conversion
	loop_until_bit_is_clear(ADCSRA,ADSC); // wait for conversion complete
	return ADC;
}

//======================================================

int main(void)
{
	DDRB = 0xff; // portB all outputs
	DDRF = 0x00;
	ADCSRA = (1<<ADEN)|(1<<ADPS2)|(1<<ADPS1)|(1<<ADPS0);
	
	while(1)
	{
		adc_value0 = adc_read(0);
		adc_value1 = adc_read(1);
		adc_value2 = adc_read(2);
		adc_value3 = adc_read(3);
		adc_value4 = adc_read(4);
		adc_value5 = adc_read(5);
		
		if(adc_value0 > 512)  // >2.5 V
		setbit(PORTB,0);
		else
		clrbit(PORTB,0);
		
		if(adc_value1 > 512)
		setbit(PORTB,1);
		else
		clrbit(PORTB,1);
		
		if(adc_value2 > 512)  // >2.5 V
		setbit(PORTB,2);
		else
		clrbit(PORTB,2);
		
		if(adc_value3 > 512)
		setbit(PORTB,3);
		else
		clrbit(PORTB,3);
		
		if(adc_value4 > 512)  // >2.5 V
		setbit(PORTB,4);
		else
		clrbit(PORTB,4);
		
		if(adc_value5 > 512)
		setbit(PORTB,5);
		else
		clrbit(PORTB,5);
	}
}