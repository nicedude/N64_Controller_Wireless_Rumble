/*
 * NERF_AMMO_COUNT.c
 *
 * Created: 13-09-2012 21:20:05
 *  Author: Mark
 */ 

#ifndef F_CPU
#define F_CPU 8000000UL
#endif

//#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include "charlie.h"


int main(void)
	{
	setup_charlie();
	sei(); //global interrupt enable
	while (1)
		{
		roll_charlie();
		}	
	}



