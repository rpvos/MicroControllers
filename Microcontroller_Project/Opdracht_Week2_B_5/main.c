/*
 * main.c
 *
 * Created: 2/10/2021 3:18:58 PM
 *  Author: Rik Vos
 */ 

#include "lcd.h"

int main(void)
{
	init();
	
	char text[] = "hello";
	display_text(text);
	
	
    while(1)
    {
        
    }
}