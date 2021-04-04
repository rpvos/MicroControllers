#define F_CPU 8e6

#include <avr/sfr_defs.h>
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <stddef.h>

#define setbit(port,bit)  ((port) |=  (1<<bit)) // set bit
#define clrbit(port,bit)  ((port) &= ~(1<<bit)) // clear bit
#define BIT(x)			(1 << (x))
#define  BASELENGTH 300
#define END_WAIT_LENGTH -1

#define REF_AVCC (1<<REFS0)  // reference = AVCC = 5 V
#define REF_INT  (1<<REFS0)|(1<<REFS1) // internal reference 2.56 V

// global variables
uint16_t adc_value0, adc_value1, adc_value2, adc_value3, adc_value4, adc_value5;
int keyboardMode = 0;
int buzzerOn = 1;
volatile int Frequency = 100;
volatile int DutyCycle = 50;
int high = 0;

typedef struct tone_t{
	int frequency;
	int waitLength;
} tone;


 tone A ={.frequency = 440, .waitLength = BASELENGTH};
 tone B ={.frequency = 495,.waitLength = BASELENGTH};
 tone C ={.frequency = 528,.waitLength = BASELENGTH};
 tone D ={.frequency = 594,.waitLength = BASELENGTH};
 tone E ={.frequency = 660,.waitLength = BASELENGTH};
 tone F ={.frequency = 704,.waitLength = BASELENGTH};
 tone G = {.frequency = 792,.waitLength = BASELENGTH};
 tone X = {.frequency = -1,.waitLength = BASELENGTH};
 tone ending = {.frequency = -1,.waitLength = END_WAIT_LENGTH};

void wait( int ms ) {
	for (int tms=0; tms<ms; tms++) {
		_delay_ms( 1 );			// library function (max 30 ms at 8MHz)
	}
}

void playSong(tone Song[],int timeBetweenTones){
	DutyCycle = 20;
	int size = 50;
	
	for (int i = 0;i<size;i++)
	{
		if (Song[i].waitLength == END_WAIT_LENGTH)
		{
			return;
		}
		Frequency = Song[i].frequency;
		wait( Song[i].waitLength);
		
		Frequency = -1;
		wait(timeBetweenTones);
	}
}

uint16_t adc_read(uint8_t channel)
{
	ADMUX = REF_AVCC | channel;  // set reference and channel
	ADCSRA |= (1<<ADSC);         // start conversion
	loop_until_bit_is_clear(ADCSRA,ADSC); // wait for conversion complete
	return ADC;		
}

ISR( TIMER1_COMPA_vect ) {
	if(!buzzerOn || Frequency == -1)
		return;
		
	
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

ISR( INT0_vect ) {
	keyboardMode = !keyboardMode;
}

void initInterrupt(){
	// Init I/O
	DDRD = 0xFE;

	// Init Interrupt hardware
	EICRA |= 0x03;			// INT1 falling edge, INT0 rising edge
	EIMSK |= 0x01;			// Enable INT1 & INT0
	
}

void timer1Init( void ) {
	OCR1A = Frequency;	// 16-bits compare value of counter 1
	TIMSK |= BIT(4);		// T1 compare match A interrupt enable
	sei();					// turn_on intr all	TCCR1A = 0b00000000;  Initialize T1: timer, prescaler=256,
	TCCR1B = 0b00001100;	// compare output disconnected, CTC, RUN
}


int main(void)
{
	DDRA = 0x00; // PortA all inputs
	DDRB = 0xff; // portB all outputs
	DDRF = 0x00; // portF all inputs
	ADCSRA = (1<<ADEN)|(1<<ADPS2)|(1<<ADPS1)|(1<<ADPS0);
	timer1Init();
	initInterrupt();
	sei();					// turn on all interrupts
	
	tone electronicSong[] = {C,D,E,C,C,D,E,C,E,F,G,X,E,F,G,X,G,A,G,F,E,C,G,A,G,F,E,C,C,G,C,X,C,G,C,ending};
	
	while(1)
	{
		if (!keyboardMode)
		{
			buzzerOn = 1;
			adc_value0 = adc_read(0);
			double percentage = adc_value0/1024.0;
			double waitAmount = percentage*100;
			Frequency = waitAmount;
			
			
			adc_value1 = adc_read(1);
			percentage = adc_value1/1024.0 *100;
			DutyCycle = percentage;
		}
		else if (keyboardMode)
		{
			buzzerOn = 0;

			if ((PINA & (1<<PA0)))
			{
				buzzerOn  =1;
				Frequency = 440;		
		}else if ((PINA & (1<<PA1)))
		{
			buzzerOn  =1;
			Frequency = 495;
		}
		else if ((PINA & (1<<PA2)))
		{
			buzzerOn  =1;
			Frequency = 528;
		}
		else if ((PINA & (1<<PA3)))
		{
			buzzerOn  =1;
			Frequency = 594;
		}
		else if ((PINA & (1<<PA4)))
		{
			buzzerOn  =1;
			Frequency = 660;
		}
			else if ((PINA & (1<<PA5)))
			{
				buzzerOn  = 1;
				Frequency = 704;
			}
				else if ((PINA & (1<<PA6)))
				{
					buzzerOn  =1;
					Frequency = 792;
				}
				else if ((PINA & (1<<PA7)))
				{
					buzzerOn = 1;
					playSong(electronicSong,0);
				}
			_delay_ms(10);
		}
	}
}