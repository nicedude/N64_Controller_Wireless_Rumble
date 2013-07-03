/************************************************************************
* SPI interface code for PIC18F devices
*
*
* (c) 2005, Lieven Hollevoet, boostc compiler.
* 2012 adapted for the nRF24L01 by Martin Hug.
************************************************************************/

#ifndef F_CPU
#define F_CPU 12000000UL
#endif

#include "spi.h"

#include <util/delay.h>
#include <avr/io.h>



// Declaration of local variables.
unsigned char spi_data_buffer[34];
unsigned char data[34];               // storage for the readback data.   
char spi_pointer = 0x00;
char spi_counter = 0x00;
char statusReg;             
              


/////////////////////////////////////////////////////////////////////////
// Init of the hardware unit
/////////////////////////////////////////////////////////////////////////
void spi_init(){

  // DDR settings
	// outputs
	
	
	SPI_SS_DDR |= 1<<(SPI_SS_BV); //Set slave select to output so it doesn't interfere with SPI operation
	
	SPI_SCK_DDR|=(1<<SPI_SCK_BV);
	SPI_MOSI_DDR|=(1<<SPI_MOSI_BV);
	SPI_CS_DDR|=(1<<SPI_MISO_BV);
	SPI_CE_DDR|=(1<<SPI_CE_BV);
	
	// inputs
	SPI_MOSI_DDR &= ~(1<<SPI_MOSI_BV);
	SPI_IRQ_DDR &= ~(1<<SPI_IRQ_BV);
	
	

SPCR = (1<<SPE)|(1<<MSTR);			  //Enable SPI, Master, set clock rate fck/4
SPSR |= (1<<SPI2X);				      //double speed fck/2 (6MHz)


spi_pointer = 0;          // init the buffer pointers
spi_counter = 0;	
}
//
//

//////////////////////////////////////////////////////////////////////
// Init the databuffer of the SPI code
//////////////////////////////////////////////////////////////////////
void spi_init_buffer(){
	spi_pointer = 0;
	return;
}
//
//

//////////////////////////////////////////////////////////////////////
// Add a byte to the SPI TX buffer
//////////////////////////////////////////////////////////////////////
void spi_load_byte(unsigned char input){
	spi_data_buffer[spi_pointer] = input;
	spi_pointer++;
}
//
//

//////////////////////////////////////////////////////////////////////
// Initiate an SPI transaction
//////////////////////////////////////////////////////////////////////
void spi_exchange(char sw){

	uint8_t flag = 0x00; 
	spi_counter = spi_pointer;
	spi_pointer = 0x00;
  	
	while (spi_counter)
    {
    SPI_CS_low();
    _delay_us(10);

    data[spi_pointer] = spi_Read_Write(spi_data_buffer[spi_pointer]); // 1st address, 2nd data.
    
    if (!sw)
		{
		if (flag)
			{
			SPI_CS_high();
			flag = 0x00;
			_delay_us(10);
			}
		else flag = ~flag;
		}
    }
    sw = 0x00;
    SPI_CS_high();
    _delay_us(10);
 
   return;
}
//
//

unsigned char spi_Read_Write(unsigned char byte)
{
  //sspbuf = byte;
  SPDR = byte;
  while(!(SPSR & (1<<SPIF))); //wait for empty transmit buffer <- probably what causes the shitty halting. :/
  //while (!test_bit (sspstat,0));                                      // waits for byte sent.
  spi_pointer++;
  spi_counter--;
  //return sspbuf;
  return SPDR;
}
//
//

///////////////////////////////////////////////////////////////////////
// Get a byte from the spi data buffer
///////////////////////////////////////////////////////////////////////
unsigned char spi_get_byte(char count){
  spi_pointer = count;
	return data[spi_pointer];
}
//
//

/////////////////////////////////////////////////////////////////
// function for debugging read registers: The reg_addr variable
// containes the register address.
/////////////////////////////////////////////////////////////////
void spi_read_reg(char reg_addr){

  
  //spi_cs = 0;
  SPI_CS_low();
    //delay_us(10);                                          // aprox.10us.
	_delay_us(10);
    statusReg = spi_Read_Write(reg_addr);                  // change REGISTER.
    data[0]   = spi_Read_Write(0x00);
    data[1]   = spi_Read_Write(0x00);
    data[2]   = spi_Read_Write(0x00);
    data[3]   = spi_Read_Write(0x00);
    data[4]   = spi_Read_Write(0x00);
    data[5]   = spi_Read_Write(0x00);
 
    spi_init_buffer();                                      // set pointer 0.
    //spi_cs = 1; 
	SPI_CS_high();
}
//
//


