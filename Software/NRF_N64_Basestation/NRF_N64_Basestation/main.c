/*
 * NERF_AMMO_COUNT.c
 *
 * Created: 13-09-2012 21:20:05
 *  Author: Mark
 */ 

#ifndef F_CPU
#define F_CPU 12000000UL
#endif

//#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include "charlie.h"
#include "spi.h"
#include "nrf24L01.h"


int main(void)
	{
	
	
	sei(); //global interrupt enable
	
	
	spi_init();
	nrf_init();
	nrf_tx_mode();
	nrf_reset_status();

	
	setup_charlie();
	
	
	
	roll_charlie();
	
	
	while (1)
		{
			//spi_load_byte(load_byte = 'a'); //
			//spi_load_byte(load_byte = 'b'); //
			//spi_load_byte(load_byte = 'c'); //
		      nrf_put_byte();                       // send data wireless with nRF24L01.
		      if (nrf_irq_jack()) set_charlie(12);             // waits until data has been sent.
			  _delay_ms(1500);
		      nrf_reset_status();
			  if (nrf_irq_jack()) set_charlie(11);             // waits until data has been sent.
			  _delay_ms(1500);
			  //else 
			  //roll_charlie();
		}	
	}



