/*
 * n64_controller.c
 *
 * Created: 7/3/2013 0:02:37
 *  Author: delfi
 */ 
#include "n64_controller.h"


void n64_init(void){
	N64_PORT	&= ~N64_POS;  //Set pin low
	N64_DDR		&= ~N64_POS;	//Set pin to input
}

static inline __attribute__((always_inline)) void n64_sendbyte(unsigned char d){
	asm volatile(
	"ldi __tmp_reg__, [bit]\n"	//1
	"out gpr, r1\n"				//1
	"ldi r1, ~[bit]\n"			//1
	"clt\n"
	
	
	//Assume __tmp_reg__ = ddr | (n64_pos), r1 = ddr & ~n64_pos, Tflag=0
	"out [drp], __tmp__reg__\n"	//1  
	"lsl [dat]\n"			//1
	"brcc lbl_sb0\n"		//seq 2
	"out [drp], r1\n"
	"lbl_sb0:\n"
	"brtc 0\n"
	"brtc 0\n"
	"brtc 0\n"
	"nop\n"					//1
	"out [drp], r1\n"
	"lbl_sb0:\n"
	
	
	::
	[dat]"r"(d), [bit]"M"(N64_POS),
	[drp]"M"(_SFR_IO_ADDR(N64_DDR)),[gpr]"M"(_SFR_IO_ADDR(N64_GP_REG))
	);
}


void n64_ctrl_init(void){
	n64_sendbyte(0x00);
	n64_sendbyte(0x00);
	
}