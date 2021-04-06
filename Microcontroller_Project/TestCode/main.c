#define F_CPU 8e6

#include <avr/sfr_defs.h>
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <stddef.h>

#define setbit(port,bit)  ((port) |=  (1<<bit)) // Set bit
#define clrbit(port,bit)  ((port) &= ~(1<<bit)) // Clear bit
#define BIT(x)	(1 << (x)) // Method to shift a 1 to the given bit
#define BASELENGTH 300 // Base length of a tone
#define END_WAIT_LENGTH -1 // Value to end a song
#define REF_AVCC (1<<REFS0) // Value of the maximum pot meter value (5 volt)

// Global variables for the pot meters
uint16_t adc_value0, adc_value1;

// Other global variables
int keyboardMode = 0;
int buzzerOn = 1;
int Frequency = 100;
int DutyCycle = 50;
int high = 0;

// Struct for a tone in a song
typedef struct tone_t{
	int frequency;
	int waitLength;
} tone;

// Base tones with corresponding frequency
tone A ={.frequency = 440, .waitLength = BASELENGTH};
tone B ={.frequency = 495,.waitLength = BASELENGTH};
tone C ={.frequency = 528,.waitLength = BASELENGTH};
tone D ={.frequency = 594,.waitLength = BASELENGTH};
tone E ={.frequency = 660,.waitLength = BASELENGTH};
tone F ={.frequency = 704,.waitLength = BASELENGTH};
tone G = {.frequency = 792,.waitLength = BASELENGTH};
// Tone to make a pause in music
tone X = {.frequency = -1,.waitLength = BASELENGTH};
// Tone to signal the ending of a song
tone ending = {.frequency = -1,.waitLength = END_WAIT_LENGTH};

// Function to make the CPU wait
void wait( int ms ) {
	for (int tms=0; tms<ms; tms++) {
		_delay_ms( 1 );			// library function (max 30 ms at 8MHz)
	}
}

// Function to play song
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

// Interrupt for timer 1 that controls the buzzer signal
ISR( TIMER1_COMPA_vect ) {
	// Check if the buzzer should be on and if the frequency is not -1
	if(!buzzerOn || Frequency == -1)
	return;
	
	// Check if the buzzer signal needs to be set to high
	if (high)
	{
		// calculate the duration of the signal by multiplying the frequency and duty cycle
		// It is all divided by 100 because we need the percentage duty cycle
		int compareValue = Frequency*DutyCycle/100;
		
		// Check if the duration is not smaller then 1 because we cant wait a shorter period then 1
		if (compareValue<1)
		{
			compareValue = 1;
		}
		
		// Set the compare value of the timer to generate an interrupt when compare value time has passed
		OCR1A = compareValue;
		
		}else{
		// This is the part to calculate the duration of the buzzer signal being low
		// We subtract duty cycle from 100 to get the length of the low signal
		// We divide it again to make duty cycle an percentage
		int compareValue =Frequency*(100-DutyCycle)/100;
		
		// make sure the duration is not smaller then 1
		if (compareValue<1)
		{
			compareValue = 1;
		}
		
		// Set the compare value of timer 1
		OCR1A = compareValue;
		
	}
	// set the counter to 0
	TCNT0 = 0;
	// Set high to not high so we alternate high and low
	high ^= 0x01;
	// Set bit 1 of port C to high or low
	// This is the port the buzzer natively uses
	PORTC ^= 0x02;
}

// Interrupt to switch the modus
ISR( INT0_vect ) {
	keyboardMode = !keyboardMode;
}

// Method that initializes the interrupt requirements
void initInterrupt(){
	// Initialize I/O
	DDRD = 0xFE;

	// Initialize interrupt hardware
	// INT0 rising edge
	EICRA |= 0x03;
	// Enable INT0
	EIMSK |= 0x01;
}

void timer1Init( void ) {
	// 16-bits compare value of counter 1
	OCR1A = Frequency;
	// T1 compare match A interrupt enable
	TIMSK |= BIT(4);
	// Initialize T1: timer with pre scaler=256
	TCCR1A = 0b00000000;
	// Compare output disconnected, CTC, RUN
	TCCR1B = 0b00001100;
	// Turn on interrupt system
	sei();
}


int main(void)
{
	DDRA = 0x00; // PortA all inputs
	DDRB = 0xff; // portB all outputs
	DDRC = 0x02; // portC only port 1 as output
	DDRF = 0x00; // portF all inputs
	// ADC Control and Status Register A, ADEN = adc enable, ADPS2:0 = division factor = 128
	ADCSRA = (1<<ADEN)|(1<<ADPS2)|(1<<ADPS1)|(1<<ADPS0);
	
	// Initialize components
	timer1Init();
	initInterrupt();
	
	// Preprogrammed song we play on button A7 when it is in keyboard mode
	tone electronicSong[] = {C,D,E,C,C,D,E,C,E,F,G,X,E,F,G,X,G,A,G,F,E,C,G,A,G,F,E,C,C,G,C,X,C,G,C,ending};
	
	
	while(1)
	{
		// Actively pull the pot meter values
		if (!keyboardMode)
		{
			// Make sure the buzzer is on
			buzzerOn = 1;
			// Read the value of port 0
			adc_value0 = adc_read(0);
			// Calculate the percentage
			double percentage = adc_value0/1024.0;
			// Calculate the wait amount
			double waitAmount = percentage*100;
			// Set the frequency to it
			Frequency = waitAmount;
			
			// Same goes for the pot meter on port 1
			adc_value1 = adc_read(1);
			percentage = adc_value1/1024.0 *100;
			DutyCycle = percentage;
		}
		else if (keyboardMode)
		{
			// Turn the buzzer off
			buzzerOn = 0;

			// Actively pull if a button is pressed
			// If a button is pressed we turn the buzzer on and set the frequncy to the value of the corresponding letter
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
				// Turn the buzzer on and play a pre programmed song
				buzzerOn = 1;
				playSong(electronicSong,0);
			}
			_delay_ms(10);
		}
	}
}