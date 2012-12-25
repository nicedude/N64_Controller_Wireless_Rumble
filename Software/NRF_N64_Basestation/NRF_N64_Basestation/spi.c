/************************************************************************
* SPI interface code for PIC18F devices
*
*
* (c) 2005, Lieven Hollevoet, boostc compiler.
* 2012 adapted for the nRF24L01 by Martin Hug.
************************************************************************/

#ifndef F_CPU
#define F_CPU 8000000UL
#endif

#include "spi.h"
//#include <PIC18f4420.h>

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

  // Tris settings
	// outputs
	/*
	spi_clk_tris = 0;
	spi_do_tris  = 0;
	spi_cs_tris  = 0;
	spi_ce_tris  = 0;
	*/

	SPI_SCK_DDR|=(1<<SPI_SCK_BV);
	SPI_MOSI_DDR|=(1<<SPI_MOSI_BV);
	SPI_CS_DDR|=(1<<SPI_MISO_BV);
	SPI_CE_DDR|=(1<<SPI_CE_BV);
	
	// inputs
	/*
	spi_di_tris  = 1;
  spi_irq_tris = 1;
	*/
	SPI_MOSI_DDR &= ~(1<<SPI_MOSI_BV);
	SPI_IRQ_DDR &= ~(1<<SPI_IRQ_BV);
	
	
SPSR |= (1<<SPI2X);				      //double speed
SPCR = (1<<SPE)|(1<<MSTR);			  //Enable SPI, Master, set clock rate fck/2

// Hardware registers (depends on the selected device in spi.h)
/*	
#ifdef PIC16_SPI	// for 16F series devices.
	clear_bit(sspcon, SSPEN); // disable spi interface.
	sspcon = 0x11;				    // SPI Master, Idle high, Fosc/16.
	
	set_bit(sspcon, SSPEN);		// enable spi interface.
#endif // PIC16_SPI

#ifdef PIC18_SPI1	          // for 18F series devices.
	sspcon1.SSPEN = 0;
	sspcon1 = 0x12;           // 00010010b; Fosc/64, idle clock state = Hi.
	sspcon1.CKP = 0;
	sspstat.CKE = 1;
	sspstat.SMP = 0;
	sspcon1.SSPEN = 1;
#endif // PIC18_SPI1

	clear_bit(pir1, SSPIF);		// clear interrupt flag.
	set_bit(pie1, SSPIE);		  // enable interrupt flag.
*/	
	spi_pointer = 0;          // init the buffer pointer.
	
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
void spi_load_byte(char input){
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
    //spi_cs = 0;          
	SPI_CS_low();
    //delay_us(10);                                                     // aprox.10us.
	_delay_us(10);

    data[spi_pointer] = spi_Read_Write(spi_data_buffer[spi_pointer]); // 1st address, 2nd data.
    
    if (!sw){
      if (flag){
        //spi_cs = 1;
		SPI_CS_high();
        flag = 0x00;
        //delay_10us(1);                                                  
		_delay_us(10);
        }
      else
        flag = ~flag;
      }
    };
    sw = 0x00;
    //spi_cs = 1; 
	SPI_CS_high();
    //delay_us(10);
	_delay_us(10);
   
    /*
	  //spi_cs = 0;
	  SPI_CS_low();
      //delay_us(10);                                                   // aprox.10us.
	  _delay_us(10);
      statusReg = spi_Read_Write(0x06);                               // change REGISTER.
      data[6]   = spi_Read_Write(0x00);
      data[7]   = spi_Read_Write(0x00);
      data[8]   = spi_Read_Write(0x00);
 
    spi_init_buffer();                                                // set pointer 0. 
    //spi_cs = 1; 
	SPI_CS_high();
    //delay_us(10);
	_delay_us(10);*/
 
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


