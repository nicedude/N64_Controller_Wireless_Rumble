/*
 * SPI.c
 *
 * Created: 10-10-2012 10:12:40
 *  Author: Robotto
 *
 * Contains low level functions for initializing the SPI interface, and for RX and TX.
 */ 

#include <avr/interrupt.h>
#include <avr/io.h> //basic IO definitions
//#include <avr/>

void setup_spi(void) //enables the SPI module of the µC
{
	
}


ISR (PCINT1_vect)
{
	
}

unsigned char spi_trx(unsigned char tx_byte)
{
SPDR=tx_byte;
SPI_tx_wait();
return SPDR;
}

void setup_SPI(void)
{
	DDRB = (1<<DDB2)|(1<<DDB3)|(1<<DDB5); //Set CS, MOSI and SCK output, all others input
	SPSR |= (1<<SPI2X);				      //double speed
	SPCR = (1<<SPE)|(1<<MSTR);			  //Enable SPI, Master, set clock rate fck/2
}