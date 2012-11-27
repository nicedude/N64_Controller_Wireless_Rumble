
#include <avr/interrupt.h>
#include <avr/io.h> //basic IO definitions

void setup_ADC(void)
{
	ADCSRA |= (1<<ADEN); //ADC enable
	
	ADMUX |=(1<<REFS0);  //VREF = AVCC pin
	ADMUX &=~(1<<ADLAR); //right adjusted ADC results (ADLAR = 0)
	ADCSRA |= (1<<ADIE); //ADC interrupt enable - for waking up from adc sleep mode
	
	ADCSRA &= ~((1<<ADEN)); //ADC disable
	//PRR	|= (1<<PRADC); //ADC clk OFF,
}