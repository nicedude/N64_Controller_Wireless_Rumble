/*
 * NRF_TEST.cpp
 *
 * Created: 27-11-2012 19:37:14
 *  Author: Mark
 */ 

#define F_CPU 8000000UL

#include <avr/io.h>
#include <avr/delay.h>
#include <avr/interrupt.h>


#include "mirf.h"

uint8_t buffer[12];

typedef struct
{
	uint8_t DDRS;
	uint8_t PORT;
} charlie_DDR_and_Pins;

typedef struct
{
	charlie_DDR_and_Pins LEDS[10];
} charlie_struct;

charlie_struct charlie;

uint16_t LED_PATTERN;
uint8_t ovf_count=0;
uint8_t charlie_counter=0;

ISR (TIMER0_OVF_vect) //runs at 1MHz/256 = 3906,25Hz
{
	ovf_count++;
	if (ovf_count<32) return;	//half the frq: 1953,125Hz - period= 512µS
	ovf_count=0;
	
	PORTC=0;
	if (LED_PATTERN & (1<<charlie_counter))
	{
		DDRC=charlie.LEDS[charlie_counter].DDRS;
		PORTC=charlie.LEDS[charlie_counter].PORT;
	}
	charlie_counter++;
	if (charlie_counter>9) charlie_counter=0;
}

int main(void)
{
	setup_charlie_struct();
	setup_hw();
	
	
	// Initialize AVR for use with mirf
	mirf_init();
	// Wait for mirf to come up
	_delay_ms(50);
	// Activate interrupts
	sei();
	// Configure mirf
	mirf_config();
	// Test communication
	int i = 0;
	
	sei(); //global interrupt enable
	
	roll_charlie();
	
    while(1)
    {
	/*
	for(uint8_t i=0;i<10;i++)
	{
	mirf_send(&i,1);	
	_delay_ms(1000);
	}
	*/
	
	while (!mirf_data_ready());
	
	mirf_get_data(buffer);
	
	//set_charlie(buffer[i]);
	i++;
    }
}

void setup_hw(void)
{
	
	//SET THE APPROPRIATE FREQUENCY:
	
	
	DDRC=0; //input all the things!
	PORTC=0;
	MCUCR&=~(1<<PUD);
	
	//timer0 set up to interrupt on overflow @ 1MHz/8/256 = 488,28125Hz
	TCCR0|=(1<<CS00); //prescaler to /1
	TIMSK|=(1<<TOIE0); //Overflow interrupt enabled
	
	//ADC, freerunning, interrupt on conversion complete:
	
	//	ADCSRA=0b10101100; //ADCEN=1 (ADC on) , ADSC=0 (don't start yet), ATADE=1 (auto trigger enable), ADIE=1 (interrupt enabled), ADPS[0:2]=111 (1MHz/16=62500Hz)
	
	//ADMUX=0b00000010; //REFS[0:2]=000 (VREF=VCC), ADLAR=0 (right adjusted result), ADMUX[0:4]=0100 (ADC channel 2 - PB4)
	
	//GIVES 26mS to do 128 conversions
	
	
	
	//ADCSRB Standard values are fine
	//	DIDR0|=(1<<ADC2D); //disable digital input on ADC ch2 (PB4)
	//	ADCSRA|=(1<<ADSC); //GO!
	
}
void setup_charlie_struct(void)
{
	//setup bit patterns for DDRs and output pins.
	
	//PB4 and PB5 are input.
	
	//HLZZ:
	charlie.LEDS[0].DDRS=0b001100;
	charlie.LEDS[0].PORT=0b1000;
	//ZHZL:
	charlie.LEDS[1].DDRS=0b000101;
	charlie.LEDS[1].PORT=0b0100;
	//ZHLZ:
	charlie.LEDS[2].DDRS=0b000110;
	charlie.LEDS[2].PORT=0b0100;
	//LHZZ:
	charlie.LEDS[3].DDRS=0b001100;
	charlie.LEDS[3].PORT=0b0100;
	//ZZHL:
	charlie.LEDS[4].DDRS=0b000011;
	charlie.LEDS[4].PORT=0b0010;
	//ZLHZ:
	charlie.LEDS[5].DDRS=0b000110;
	charlie.LEDS[5].PORT=0b0010;
	//LZHZ:
	charlie.LEDS[6].DDRS=0b001010;
	charlie.LEDS[6].PORT=0b0010;
	//ZZLH:
	charlie.LEDS[7].DDRS=0b000011;
	charlie.LEDS[7].PORT=0b000001;
	//ZLZH:
	charlie.LEDS[8].DDRS=0b000101;
	charlie.LEDS[8].PORT=0b0001;
	//LZZH:
	charlie.LEDS[9].DDRS=0b001001;
	charlie.LEDS[9].PORT=0b0001;
	
}

void set_charlie(uint8_t charlieVal)
{
	switch (charlieVal)
	{
		case 11: //one louder.
		LED_PATTERN = 0xAAA;
		break;
		case 10:
		LED_PATTERN = 0b1111111111;
		break;
		case 9:
		LED_PATTERN = 0b0111111111;
		break;
		case 8:
		LED_PATTERN = 0b0011111111;
		break;
		case 7:
		LED_PATTERN = 0b0001111111;
		break;
		case 6:
		LED_PATTERN = 0b0000111111;
		break;
		case 5:
		LED_PATTERN = 0b0000011111;
		break;
		case 4:
		LED_PATTERN = 0b0000001111;
		break;
		case 3:
		LED_PATTERN = 0b0000000111;
		break;
		case 2:
		LED_PATTERN = 0b0000000011;
		break;
		case 1:
		LED_PATTERN = 0b0000000001;
		break;
		case 0:
		LED_PATTERN = 0b0000000000;
		break;
		default:
		LED_PATTERN = 0b0101010101;
		break;
	}
}

void roll_charlie(void) //scroll the LED's (during reload)
{
	uint8_t i;
	for (i=0;i<=10;i++)
	{
		set_charlie(i);
		_delay_ms(100);
	}
}