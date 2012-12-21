/*************************************************************
* nRF2401A interface code
* (c) 2005, Lieven Hollevoet, boostc compiler.
*************************************************************/

#ifndef _NRF24L01_H_
#define _NRF24L01_H_

//#include <system.h>
#include <avr/io.h>

#ifndef bool
#define bool unsigned char
#endif
#ifndef false
#define false 0
#endif
#ifndef true
#define true !false
#endif

// Define the nRF connections here
/*
#define NRF_CE_PORT			PORTC
#define NRF_CS_PORT			PORTC
#define NRF_IRQ_PORT		PORTD

#define NRF_CE_TRIS			TRISC
#define NRF_CS_TRIS			TRISC
#define NRF_IRQ_TRIS		TRISD

#define NRF_CE_PIN			1
#define NRF_CS_PIN			2
#define NRF_IRQ_PIN			3
*/

	#define NRF_CE_PORT PORTD
	#define NRF_CE_DDR DDRD
	#define NRF_CE_BV PD5
	
	#define NRF_CS_PORT PORTD
	#define NRF_CS_DDR DDRD
	#define NRF_CS_BV PD4
	
	//#define NRF_IRQ_PORT PORTD - only used for output
	#define NRF_IRQ_PORT PIND
	#define NRF_IRQ_DDR DDRD
	#define NRF_IRQ_BV PD3

///////////////////////////////////////////////////////////////////
//pin write defines:

//CS:
//volatile bit nrf_cs@ NRF_CS_PORT.NRF_CS_PIN;
#define NRF_CS_low() NRF_CS_PORT &= ~(1<<NRF_CS_BV);
#define NRF_CS_high() NRF_CS_PORT |= (1<<NRF_CS_BV);

//CE:
//volatile bit nrf_ce@ NRF_CE_PORT.NRF_CE_PIN;
#define NRF_CE_low() NRF_CE_PORT &= ~(1<<NRF_CE_BV);
#define NRF_CE_high() NRF_CE_PORT |= (1<<NRF_CE_BV);

//pin read defines:
//IRQ:
#define NRF_IRQ_read NRF_IRQ_PORT&(1<<NRF_IRQ_BV)
//volatile bit nrf_irq@NRF_IRQ_PORT.NRF_IRQ_PIN;



/*
bit	nrf_cs_tris@ NRF_CS_TRIS.NRF_CS_PIN;
bit nrf_ce_tris@ NRF_CE_TRIS.NRF_CE_PIN;
bit	nrf_irq_tris@ NRF_IRQ_TRIS.NRF_IRQ_PIN;
*/
// Constants and data types (credit: Andy Dewilde)
/* ===============================================================
					   CONFIGURATION PARAMETERS (default values)
   =============================================================== */

//.spi instruction set
#define NRF_PWR_UP      0b00000010  // CONFIG REG.1
#define NRF_PWR_DOWN    0b00000000
#define NRF_TX          0b00000000  // 0:PTX
#define NRF_RX          0b00000001  // 1:PRX
#define NRF_W_REG       0b00100000  // 000A AAAA   
#define R_RX_PAYLOAD    0b01100001
#define W_TX_PAYLOAD    0b10100000	
#define FLUSH_TX        0b11100001
#define FLUSH_RX        0b11100010
#define REUSE_TX_PL     0b11100011
#define NOP             0b11111111	
#define NRF_STAT_CLEAR  0b01110000  // resets status register values.   	


//.config
#define NRF_CONFIG      0x00      // (addr. 0x00)
#define NRF_EN_AA       0x01      // (addr. 0x01) 
#define NRF_EN_RXADDR   0x02      // (addr. 0x02)
#define NRF_SETUP_AW    0x03      // (addr. 0x03)
#define NRF_SETUP_RTR   0x04      // (addr. 0x04)
#define NRF_CHANNEL     0x05      // (addr. 0x05)
#define NRF_RF_SETUP    0x06      // (addr. 0x06)
#define NRF_STATUS      0x07      // (addr. 0x07)
#define NRF_OBSERVE_TX  0x08      // (addr. 0x08)
#define NRF_CD          0x09      // (addr. 0x09)

#define NRF_RX_ADDR_P0  0x0A      // (addr. 0x0A -0x0F reg addredss)
#define NRF_RX_ADDR_P1  0x0B
#define NRF_RX_ADDR_P2  0x0C
#define NRF_RX_ADDR_P3  0x0D
#define NRF_RX_ADDR_P4  0x0E
#define NRF_RX_ADDR_P5  0x0F
                              
#define NRF_TX_ADDR     0x10      // (addr. 0x10) 
                                                                
#define NRF_RX_PW_P0    0x11      // (addr. 0x11, payload with > 0)
#define NRF_RX_PW_P1		0x12      // (addr. 0x12, payload with > 0)
#define NRF_RX_PW_P2		0x13      // (addr. 0x13, payload with > 0)
#define NRF_RX_PW_P3		0x14      // (addr. 0x14, payload with > 0)
#define NRF_RX_PW_P4		0x15      // (addr. 0x15, payload with > 0)
#define NRF_RX_PW_P5		0x16      // (addr. 0x16, payload with > 0))

#define NRF_FIFO_STAT   0x17      // (addr. 0x17)

//.set default values
#define CONFIG_DEF      0b00001000 // (addr. 0x00)
#define EN_AA_DEF	    0b00000000 // (addr. 0x01)  // DEBUG default 00111111. 
#define EN_RXADDR_DEF   0b00000011 // (addr. 0x02)
#define SETUP_AW_DEF	0b00000011 // (addr. 0x03)
#define SETUP_RTR_DEF 	0b00000011 // (addr. 0x04)
#define CHANNEL_DEF   	0b00001100 // (addr. 0x05)  // change channel here.
#define RF_SETUP_DEF	0b00000111 // (addr. 0x06)  // DEBUG before 00001111b.
#define STATUS_DEF		0b00001110 // (addr. 0x07)
#define OBSERVE_TX_DEF	0b00000000 // (addr. 0x08)
#define CD_DEF			0b00000000 // (addr. 0x09)

#define RX_ADDR_P0_DEF	0xE7      // (addr. 0x0A-OX0F) (pipe 0 -5)
#define RX_ADDR_P1_DEF	0xC2      // (addr. 0x0A-OX0F)
#define RX_ADDR_P2_DEF	0xC3      // (addr. 0x0A-OX0F)
#define RX_ADDR_P3_DEF	0xC4      // (addr. 0x0A-OX0F)
#define RX_ADDR_P4_DEF	0xC5      // (addr. 0x0A-OX0F)
#define RX_ADDR_P5_DEF	0xC6      // (addr. 0x0A-OX0F)


#define TX_DEF   		    0xE7    // (addr. 0x10) (5 bytes)

#define RX_PW_P0_DEF		0b00000011 // (addr. 0x11) // payload 3 bytes (default 0x00).
#define RX_PW_P1_DEF		0b00000000 // (addr. 0x12)
#define RX_PW_P2_DEF		0b00000000 // (addr. 0x13)
#define RX_PW_P3_DEF		0b00000000 // (addr. 0x14)
#define RX_PW_P4_DEF		0b00000000 // (addr. 0x15)
#define RX_PW_P5_DEF		0b00000000 // (addr. 0x16)
                                  
#define FIFO_STAT_DEF 		0b00010001// (addr. 0x17)	



// ------------------------------------------------------------------
/** \brief nRF2401 configuration settings
  
    All configuration bits of the Nordic nRF24L01 chip are mapped into
    this structure.  
*/// ----------------------------------------------------------------
struct _nrf2401_t{
  char  config_addr;      // 0x00.
  char  config;           // default  00001000b.
  char  en_aa_addr;       // 0x01.
  char  en_aa;            // default  00111111b.
  char  en_rxaddr_addr;   // 0x02.
  char  en_rxaddr;        // default  00000011b.
  char  setup_aw_addr;    // 0x03.
  char  setup_aw;         // default  00000011b.
  char  setup_ptr_addr;   // 0x04. 
  char  setup_ptr;        // default  00000011b.
  char  rf_channel_addr;  // 0x05.
  char  rf_channel;       // default  00000010b.
  char  rf_setup_addr;    // 0x06.
  char  rf_setup;         // default  00001111b.
  char  status_addr;      // 0x07.
  char  status;           // default  00001110b.
  char  observe_tx_addr;  // 0x08.
  char  observe_tx;       // default  00000000b.
  char  cd_addr;          // 0x09.
  char  cd;               // default  00000000b.

  char  rx_addr_p0_addr;  // 0x0A 
  char  rx_addr_p0;       // default  0xE7.
  char  rx_addr_p1_addr;  // 0x0B
  char  rx_addr_p1;       // default  0xC2.
  char  rx_addr_p2_addr;  // 0x0C
  char  rx_addr_p2;       // default  0xC3.
  char  rx_addr_p3_addr;  // 0x0D
  char  rx_addr_p3;       // default  0xC4.
  char  rx_addr_p4_addr;  // 0x0E
  char  rx_addr_p4;       // default  0xC5. 
  char  rx_addr_p5_addr;  // 0x0F
  char  rx_addr_p5;       // default  0xC6.

  char  tx_addr_addr;     // 0x10.
  char  tx_addr;          // default  00000000b.

  char  rx_pw_p0_addr;    // 0x11.
  char  rx_pw_p0;         // 0x01; default  00000000b.
  char  rx_pw_p1_addr;    // 0x12.
  char  rx_pw_p1;         // default  00000000b
  char  rx_pw_p2_addr;    // 0x13.
  char  rx_pw_p2;         // default  00000000b
  char  rx_pw_p3_addr;    // 0x14.
  char  rx_pw_p3;         // default  00000000b
  char  rx_pw_p4_addr;    // 0x15.
  char  rx_pw_p4;         // default  00000000b
  char  rx_pw_p5_addr;    // 0x16.
  char  rx_pw_p5;         // default  00000000b
 
  char  fifo_stat_addr;   // 0x17.
  char  fifo_stat;        // default  000010001b.

  char  txrx;             // transmit or receive mode 1.
  char  write_reg;        // write to register (instruction).
  char  pw_up;            // CONFIG REG.1 (set 1 for PW_UP)
  char  pw_down;          // CONFIG REG.1

  char  w_tx_payload;     // 0xA0.
  char  w_rx_payload;     // 0x61.
  unsigned char  outgoing;// data to be sent to txpayload.
  unsigned char  incoming;// data to get from rxpayload.

  unsigned char  aux1;    // DEBUG only.
  unsigned char  aux2;    // DEBUG only.
};



typedef struct _nrf2401_t nrf2401_t;

///////////////////////////////////////////////////////////
// Function declarations: the functions are listed written  
// in the code (spi.c).
///////////////////////////////////////////////////////////
void nrf_init();
void nrf_basic_config();
void nrf_rx_mode();
void nrf_tx_mode();
void nrf_reset_status();
void nrf_send_config(char nr_of_bytes);
void nrf_read_fifo();
unsigned char nrf_get_byte(char count);
void nrf_put_byte();
unsigned char get_analog(char len);
char nrf_irq_jack();


#endif // _NRF24L01_H_
