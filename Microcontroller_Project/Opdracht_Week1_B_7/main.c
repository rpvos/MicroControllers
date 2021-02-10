/*
 * main.c
 *
 * Created: 2/10/2021 9:15:02 AM
 *  Author: Dave-
 */ 

#include <xc.h>

int maxCount = 7;
int count = 0;

void charlieLoop() {

	count++;

	for (int i=0; i<1000; i++) {
		for (int c=0; c<count; c++) {
			setCharliePlexingLed(c);
		}
	}
	wait(1000);
	if (count > maxCount) count = 0;
}

void setCharliePlexingLed (int lednr){

		// Make sure we don't feed random voltages to the LEDs
		// during the brief time we are changing pin modes and voltages.

		DDRD = 0b00000000;

		switch(lednr){

			case 0:
			
			pinMode(0, OUTPUT);
			pinMode(2, OUTPUT);
			digitalWrite(2, LOW);
			digitalWrite(0, HIGH);
			break;

			case 1:
			pinMode(3, OUTPUT);
			pinMode(2, OUTPUT);
			digitalWrite(2, LOW);
			digitalWrite(3, HIGH);
			break;

			case 2:
			pinMode(3, OUTPUT);
			pinMode(1, OUTPUT);
			digitalWrite(1, LOW);
			digitalWrite(3, HIGH);
			break;

			case 3:
			pinMode(1, OUTPUT);
			pinMode(0, OUTPUT);
			digitalWrite(0, LOW);
			digitalWrite(1, HIGH);
			break;

			case 4:
			pinMode(0, OUTPUT);
			pinMode(1, OUTPUT);
			digitalWrite(1, LOW);
			digitalWrite(0, HIGH);
			break;

			case 5:
			pinMode(2, OUTPUT);
			pinMode(0, OUTPUT);
			digitalWrite(0, LOW);
			digitalWrite(2, HIGH);
			break;

			case 6:
			pinMode(2, OUTPUT);
			pinMode(1, OUTPUT);
			digitalWrite(1, LOW);
			digitalWrite(2, HIGH);
			break;
		}
	}
}

int main(void)
{
    while(1)
    {
		charlieLoop();
        //TODO:: Please write your application code 
		
    }
}