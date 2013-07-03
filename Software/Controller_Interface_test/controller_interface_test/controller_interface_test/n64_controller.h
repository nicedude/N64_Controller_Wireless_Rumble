/*
 * n64_controller.h
 *
 * Created: 7/2/2013 22:30:39
 *  Author: delfi
 */ 


#ifndef N64_CONTROLLER_H_
#define N64_CONTROLLER_H_
#include <avr/io.h>

//Config Data PIN
#define N64_PORT	PORTB
#define N64_DDR		DDRB
#define N64_BIT		2
#define N64_PIN		PINB
#define N64_POS		(1<<N64_BIT)
#define N64_GP_REG	GPIOR0

//Assumes 8MHz Clock


#define CMD_REQUEST_INIT	0x00
#define CMD_RETURN_BUT		0x01
#define CMD_ 0x00


#endif /* N64_CONTROLLER_H_ */