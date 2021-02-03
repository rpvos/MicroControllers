/*
 * main.c
 *
 * Created: 2/3/2021 11:34:12 AM
 *  Author: rikvo
 */ 

#define F_CPU 8e6

#include <avr/io.h>
#include <util/delay.h>

void wait( int ms )
/* 
short:			Busy wait number of millisecs
inputs:			int ms (Number of millisecs to busy wait)
outputs:	
notes:			Busy wait, not very accurate. Make sure (external)
				clock value is set. This is used by _delay_ms inside
				util/delay.h
Version :    	DMK, Initial code
*******************************************************************/
{
	for (int i=0; i<ms; i++)
	{
		_delay_ms( 1 );		// library function (max 30 ms at 8MHz)
	}
}

int main(void)
{
    
	    
	    DDRD = 0b11111111;			// All pins PORTD are set to output
	    
	    int var = 0;
	    int statement = 1;
	    int lights[];
	    
	    for (int i = 0; i < 0xff; i++)
	    {
		    lights[i] = var;
		    if (statement)
		    {
			    var= var<<1;
			    var+=1;
			    
			    if (var==0xff){
				    statement= !statement;
			    }
			    
			    }else{
			    var= var<<1;
			    if (var==0)
			    {
				    statement= !statement;
			    }
		    }
		    
		    
	    }
	    var = 0;
	    
	    while (1)
	    {
		    wait(100);
			
		    PORTD = lights[var];
		    var++;
		    
		    if (var>=0xff)
		    {
			    var=0;
		    }
		    
		    
	    }

	    return 1;
    
}

