#include <avr/sfr_defs.h>
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

#define F_CPU 8e6


#define setbit(port,bit)  ((port) |=  (1<<bit)) // set bit
#define clrbit(port,bit)  ((port) &= ~(1<<bit)) // clear bit
#define BIT(x)			(1 << (x))

#define REF_AVCC (1<<REFS0)  // reference = AVCC = 5 V
#define REF_INT  (1<<REFS0)|(1<<REFS1) // internal reference 2.56 V

// global variables
uint16_t adc_value0, adc_value1, adc_value2, adc_value3, adc_value4, adc_value5;

void wait( int ms ) {
	for (int tms=0; tms<ms; tms++) {
		_delay_ms( 1 );			// library function (max 30 ms at 8MHz)
	}
}

uint16_t adc_read(uint8_t channel)
{
	ADMUX = REF_AVCC | channel;  // set reference and channel
	ADCSRA |= (1<<ADSC);         // start conversion
	loop_until_bit_is_clear(ADCSRA,ADSC); // wait for conversion complete
	return ADC;		
}

volatile int Frequency = 100;
volatile int DutyCycle = 50;
volatile int high = 0;

ISR( TIMER1_COMPA_vect ) {
	if (high)
	{
		int counter = Frequency*DutyCycle/100;
		if (counter<1)
		{
			counter = 1;
		}
	OCR1A = counter;		// Set the compare value
	
	}else{
		int counter =Frequency*(100-DutyCycle)/100;
		if (counter<1)
		{
			counter = 1;
		}
	OCR1A = counter;		// Set the compare value
		
	}
	TCNT0 = 0;
	high ^= 0x01;
	PORTB ^= 0x01;
}

void timer1Init( void ) {
	OCR1A = Frequency;	// 16-bits compare value of counter 1
	TIMSK |= BIT(4);		// T1 compare match A interrupt enable
	sei();					// turn_on intr all	TCCR1A = 0b00000000;  Initialize T1: timer, prescaler=256,
	TCCR1B = 0b00001100;	// compare output disconnected, CTC, RUN
}


int main(void)
{
	DDRB = 0xff; // portB all outputs
	DDRF = 0x00; // portF all inputs
	ADCSRA = (1<<ADEN)|(1<<ADPS2)|(1<<ADPS1)|(1<<ADPS0);
	timer1Init();
	sei();					// turn on all interrupts
	
	while(1)
	{
		adc_value0 = adc_read(0);
		double percentage = adc_value0/1024.0;
		double waitAmount = percentage*100;
		Frequency = waitAmount;
		
		
		adc_value1 = adc_read(1);
		percentage = adc_value1/1024.0 *100;
		DutyCycle = percentage;
		
		/*
		adc_value2 = adc_read(2);
		adc_value3 = adc_read(3);
		adc_value4 = adc_read(4);
		adc_value5 = adc_read(5);
		
		CompareValue = adc_value0;
		*/
	}
}