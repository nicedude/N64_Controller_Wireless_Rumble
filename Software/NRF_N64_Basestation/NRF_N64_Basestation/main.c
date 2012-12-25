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
#include "spi.h"
#include "nrf24L01.h"


int main(void)
	{
	setup_charlie();
	
	nrf_init();
	
	nrf_tx_mode();
	
	nrf_reset_status();

	
	
	sei(); //global interrupt enable
	while (1)
		{
		roll_charlie();
		
		      nrf_put_byte();                       // send data wireless with nRF24L01.
		      while (nrf_irq_jack());             // waits until data has been sent.
		      nrf_reset_status();
		
		}	
	}



