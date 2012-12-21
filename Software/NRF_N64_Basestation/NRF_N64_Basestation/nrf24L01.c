/*******************************************************************
* nRF2401A interface code
*
* Contains functions to operate the nRFL01 in disabled auto-ACK mode.
*
* (c) 2005, Lieven Hollevoet, boostc compiler.
* adapted for the nRF24L01 by Martin Hug 2012 .
********************************************************************/

#ifndef F_CPU
#define F_CPU 8000000UL
#endif

#include "nrf24L01.h"
#include "spi.h"
#include <util/delay.h>
#include <avr/io.h>



//#include "serial.h"


// Global variable to this function, contains all the settings
nrf2401_t nrf_conf;
char aux_aux;



//////////////////////////////////////////////////////////////
// nRF init function
// 
// Configures the SPi interface and the basic settings for 
// the nRF
//////////////////////////////////////////////////////////////
void nrf_init() {

	  //delay_ms(3);
	  _delay_ms(3);

	  spi_init();                 // Init the SPI interface, enables INTCON, 6,7.
	
	  //nrf_ce = 0;                 // RC,Pin35: CE 0 = transmitter, CE 1 = receiver.
	  NRF_CE_low();
	  //nrf_cs = 1;                 // RC,Pin36: nRF24L01 desactivated.     
	  NRF_CS_high();

	  nrf_basic_config();
	
	return;
}
//
//

//////////////////////////////////////////////////////////////
// Basic configuration settings
// 
// Change parameters like channel and datawidth here
//////////////////////////////////////////////////////////////
void nrf_basic_config() {

	// Program the settings for the nrf
	nrf_conf.config_addr    = NRF_CONFIG;     // 0x00.
  nrf_conf.config         = CONFIG_DEF;     // 00001000b (default)
  nrf_conf.en_aa_addr     = NRF_EN_AA;      // 0x01.
  nrf_conf.en_aa          = EN_AA_DEF;      // 00111111b (default)
  nrf_conf.en_rxaddr_addr = NRF_EN_RXADDR;  // 0x02.
  nrf_conf.en_rxaddr      = EN_RXADDR_DEF;  // 00000011b (default)
  nrf_conf.setup_aw_addr  = NRF_SETUP_AW;   // 0x03.
  nrf_conf.setup_aw       = SETUP_AW_DEF;   // 00000011b (default)
  nrf_conf.setup_ptr_addr = NRF_SETUP_RTR ; // 0x04.
  nrf_conf.setup_ptr      = SETUP_RTR_DEF;  // 00000011b (default)
  nrf_conf.rf_channel_addr= NRF_CHANNEL;    // 0x05.
  nrf_conf.rf_channel     = CHANNEL_DEF;    // 00000010b (default)
  nrf_conf.rf_setup_addr  = NRF_RF_SETUP;   // 0x06.
  nrf_conf.rf_setup       = RF_SETUP_DEF;   // 00001111b (default)
 
  nrf_conf.rx_addr_p0_addr= NRF_RX_ADDR_P0; // 0x0A - 0xF  writes 6 times to the
  nrf_conf.rx_addr_p0     = RX_ADDR_P0_DEF; // same address to increase the pointer.  
  nrf_conf.rx_addr_p1_addr= NRF_RX_ADDR_P1; // 0x0B - 0xF  writes 6 times to the
  nrf_conf.rx_addr_p1     = RX_ADDR_P1_DEF; // same address to increase the pointer. 
                                            // 0x0C -0x0F not sent (no use in his project)
  nrf_conf.tx_addr_addr   = NRF_TX_ADDR;    // 0x10
  nrf_conf.tx_addr        = TX_DEF;         // 00000000b; (addr. 0x10)

  nrf_conf.rx_pw_p0_addr  = NRF_RX_PW_P0;   // 0x11     
  nrf_conf.rx_pw_p0       = RX_PW_P0_DEF;   // 00000010b (sets payload to 2 bytes, default = 0x00)
  nrf_conf.rx_pw_p1_addr  = NRF_RX_PW_P1;   // 0x12
  nrf_conf.rx_pw_p1       = RX_PW_P1_DEF;   // 00000010b (sets payload to 2 bytes, default = 0x00)
  nrf_conf.rx_pw_p2_addr  = NRF_RX_PW_P2;   // 0x13
  nrf_conf.rx_pw_p2       = RX_PW_P2_DEF;   // 00000000b (default)
  nrf_conf.rx_pw_p3_addr  = NRF_RX_PW_P3;   // 0x14
  nrf_conf.rx_pw_p3       = RX_PW_P3_DEF;   // 00000000b (default)
  nrf_conf.rx_pw_p4_addr  = NRF_RX_PW_P4;   // 0x15
  nrf_conf.rx_pw_p4       = RX_PW_P4_DEF;   // 00000000b (default)
  nrf_conf.rx_pw_p5_addr  = NRF_RX_PW_P5;   // 0x16
  nrf_conf.rx_pw_p5       = RX_PW_P5_DEF;   // 00000000b (default)

	nrf_conf.write_reg      = NRF_W_REG ;     // 00100000b  // 000A AAAA.

 	nrf_send_config(1);                       // configure the device.
return;
	
}
//
//

//////////////////////////////////////////////////////////////
// Program the nRF in receive mode 
//////////////////////////////////////////////////////////////
void nrf_rx_mode(){

  nrf_conf.config_addr = NRF_CONFIG;        // 0x00 (address).
	nrf_conf.write_reg = NRF_W_REG;
	nrf_conf.txrx = NRF_RX;
  nrf_conf.pw_up = NRF_PWR_UP;
	nrf_send_config(3);
	
	//nrf_ce = 1;                     	        // enable reception.
	NRF_CE_high();
//  delay_us(10);
_delay_us(10);
 return;	
}
//
//

//////////////////////////////////////////////////////////////
// Program the nRF in transmit mode
//////////////////////////////////////////////////////////////
void nrf_tx_mode(){

  nrf_conf.config_addr = NRF_CONFIG;        // 0x00 (address).
	nrf_conf.write_reg = NRF_W_REG;
	nrf_conf.txrx = NRF_TX;
  nrf_conf.pw_up = NRF_PWR_UP;
	nrf_send_config(2);

  //nrf_ce = 0;                               // enable transmition.
  NRF_CE_low();
  //delay_us(10);
  _delay_us(10);
 return;	
}
//
//

//////////////////////////////////////////////////////////////
// clear the status register of the nRF24L01
//////////////////////////////////////////////////////////////
void nrf_reset_status(){
  //char aux = nrf_ce;
 char aux = NRF_CE_PORT&(1<<NRF_CE_BV);
    //nrf_ce = 0;                     	        // to be sure the nRF  is a transmitter.
	NRF_CE_low();
    nrf_conf.status_addr = NRF_STATUS;        // 0x07 (address).
    nrf_conf.status      = NRF_STAT_CLEAR;    // 01110000b. 
	  nrf_send_config(4);
      if (aux);
      //nrf_ce = true;                          // returns as receiver.  
	  NRF_CE_high();
  return;	
}
//
//

//////////////////////////////////////////////////////////////
//  Send the configuration word to the nRF
//
//////////////////////////////////////////////////////////////
void nrf_send_config(char nr_of_bytes){

	char helper;

	switch (nr_of_bytes) {
		case 1:
			spi_load_byte(nrf_conf.config_addr | nrf_conf.write_reg);         // addr: 0x00.
			spi_load_byte(nrf_conf.config);                                   // default  00010000b.
			spi_load_byte(nrf_conf.en_aa_addr | nrf_conf.write_reg);          // addr: 0x01.
			spi_load_byte(nrf_conf.en_aa);                                    // default  00111111b. // DEBUG set all to 0x00.
			spi_load_byte(nrf_conf.en_rxaddr_addr | nrf_conf.write_reg);      // addr: 0x02.
			spi_load_byte(nrf_conf.en_rxaddr);                                // default  00000011b.
			spi_load_byte(nrf_conf.setup_aw_addr | nrf_conf.write_reg);       // addr: 0x03.
			spi_load_byte(nrf_conf.setup_aw);                                 // default  00000011b.
			spi_load_byte(nrf_conf.setup_ptr_addr | nrf_conf.write_reg);      // addr: 0x04.
			spi_load_byte(nrf_conf.setup_ptr);                                // default  00000011b.
			spi_load_byte(nrf_conf.rf_channel_addr | nrf_conf.write_reg);     // addr: 0x05.
			spi_load_byte(nrf_conf.rf_channel);                               // default  00000011b.
			spi_load_byte(nrf_conf.rf_setup_addr | nrf_conf.write_reg);       // addr: 0x06.
			spi_load_byte(nrf_conf.rf_setup);                                 // default  00001111b.

      spi_load_byte(nrf_conf.rx_addr_p0_addr | nrf_conf.write_reg);     // addr: 0x0A.
			spi_load_byte(nrf_conf.rx_addr_p0);                               // set to 00000010b.
      spi_load_byte(nrf_conf.rx_addr_p1_addr | nrf_conf.write_reg);     // addr: 0x0B.
			spi_load_byte(nrf_conf.rx_addr_p1);                               // set to 00000010b.
                                                                        // 0X0C -0X0F no use at the moment.

			spi_load_byte(nrf_conf.tx_addr_addr | nrf_conf.write_reg);        // addr: 0x010.
			spi_load_byte(nrf_conf.tx_addr);                                  // default  00000000b.

      spi_load_byte(nrf_conf.rx_pw_p0_addr | nrf_conf.write_reg);       // addr: 0x011.
      spi_load_byte(nrf_conf.rx_pw_p0);                                 // set to  00000010b.
      spi_load_byte(nrf_conf.rx_pw_p1_addr | nrf_conf.write_reg);       // addr: 0x012.
      spi_load_byte(nrf_conf.rx_pw_p1);                                 // set to  00000010b.
      spi_load_byte(nrf_conf.rx_pw_p2_addr | nrf_conf.write_reg);       // addr: 0x013.
      spi_load_byte(nrf_conf.rx_pw_p2);                                 // default  00000000b.
      spi_load_byte(nrf_conf.rx_pw_p3_addr | nrf_conf.write_reg);       // addr: 0x014.
      spi_load_byte(nrf_conf.rx_pw_p3);                                 // default  00000000b.
      spi_load_byte(nrf_conf.rx_pw_p4_addr | nrf_conf.write_reg);       // addr: 0x015.
      spi_load_byte(nrf_conf.rx_pw_p4);                                 // default  00000000b.
      spi_load_byte(nrf_conf.rx_pw_p5_addr | nrf_conf.write_reg);       // addr: 0x016.
      spi_load_byte(nrf_conf.rx_pw_p5);                                 // default  00000000b.
     break; 

		 case 2: // configure the nRF24L01 as transmitter & power_up.
			spi_load_byte(nrf_conf.config_addr | nrf_conf.write_reg);         // 00100000b.
			spi_load_byte(nrf_conf.config | nrf_conf.pw_up);                  // 00010010b.
     break; 

		 case 3: // configure the nRF24L01 as receiver & power_up.
			spi_load_byte(nrf_conf.config_addr | nrf_conf.write_reg);         // 00100000b.
			spi_load_byte(nrf_conf.config | nrf_conf.pw_up | nrf_conf.txrx);  // 00010011b.
     break;

		 case 4: // clear the nRF24L01 status register.
			spi_load_byte(nrf_conf.status_addr | nrf_conf.write_reg);         // 00100111b.
			spi_load_byte(nrf_conf.status);                                   // 01110000b.
	    }
	    spi_exchange(0);                                                   // send configuration bytes.
	  return;
  	 
}
//
//

/////////////////////////////////////////////////////////////////////////
// Read a byte from nrf receive buffer
//
////////////////////////////////////////////////////////////////////////

void nrf_read_fifo(){
   //nrf_ce = 0; 
   NRF_CE_low();
   //delay_us(10);
   _delay_us(10);
   char flag = 1;

    nrf_conf.w_rx_payload = R_RX_PAYLOAD;           // loads 0x61, payload address.   
    nrf_conf.incoming = 0x00;                       // send 0x00 te get byte from rxpayload.  
    spi_load_byte(nrf_conf.w_rx_payload);           // addr: 0xA0.
	  spi_load_byte(nrf_conf.incoming);               // dummy bytes.
    spi_load_byte(nrf_conf.incoming);               // dummy bytes.
    spi_load_byte(nrf_conf.incoming);               // dummy bytes.   

    spi_exchange(flag);                             // send configuration bytes.
    flag = 0;
    //nrf_ce = 1;
	NRF_CE_high();
 }
//
//

///////////////////////////////////////////////////////////////////////
// reads the data[] array to send afterwards the data to the serial port
//
///////////////////////////////////////////////////////////////////////
unsigned char nrf_get_byte(char count){

   unsigned char data_ret;
   data_ret = spi_get_byte(count);

  return data_ret;
}
//
//

////////////////////////////////////////////////////////////////////////
// Write data to the nrf and transmit
//
////////////////////////////////////////////////////////////////////////
void nrf_put_byte(){

    unsigned char load_byte;
    char flag = 1;

    nrf_conf.w_tx_payload = W_TX_PAYLOAD;       // loads 0xA0, payload address.
      
	  spi_init_buffer();                          // pointer to 0;

      spi_load_byte(nrf_conf.w_tx_payload);     // addr: 0xA0.
	  spi_load_byte(load_byte = 'A'); // 
      spi_load_byte(load_byte = 'B'); // 
      spi_load_byte(load_byte = 'C'); // 

    spi_exchange(flag);                         // send configuration bytes.
    flag = 0;
 	  //nrf_ce = 1;                                 // toggle CE for 10us to send the data.
	   NRF_CE_high();
    //delay_ms(1);                                // should work with 10us.
	_delay_us(10);                                
  	//nrf_ce = 0; 
	  NRF_CE_low();
}
//
//
void send_byte(){

 	  //nrf_ce = 1;                                 // toggle CE for 10us to send the data.
	   NRF_CE_high();
    //delay_ms(1);                                // should work with 10us.
	_delay_us(10);
  	//nrf_ce = 0; 
	  NRF_CE_low();
 }
//
//

char nrf_irq_jack()                             // something happend?
  {
    if (NRF_IRQ_read)
    return true;
    else
    return false;                               // IRQ active(low), data has been sent/received.
  }
//
//

