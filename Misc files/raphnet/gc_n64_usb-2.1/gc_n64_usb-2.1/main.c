/* Name: main.c
 * Project: Gamecube/N64 to USB converter
 * Author: Raphael Assenat <raph@raphnet.net
 * Copyright: (C) 2007-2011 Raphael Assenat <raph@raphnet.net>
 * License: Proprietary, free under certain conditions. See Documentation.
 * Comments: Based on HID-Test by Christian Starkjohann
 * Tabsize: 4
 */
#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/pgmspace.h>
#include <avr/wdt.h>
#include <avr/sleep.h>
#include <util/delay.h>
#include <string.h>

#include "usbdrv.h"
#include "oddebug.h"
#include "gamepad.h"
#include "gamecube.h"
#include "n64.h"
#include "gcn64_protocol.h"

#include "devdesc.h"
#include "reportdesc.h"

#define MAX_REPORTS	2


static uchar *rt_usbHidReportDescriptor=NULL;
static int rt_usbHidReportDescriptorSize=0;
static uchar *rt_usbDeviceDescriptor=NULL;
static uchar rt_usbDeviceDescriptorSize=0;


int usbCfgSerialNumberStringDescriptor[] PROGMEM = {
 	USB_STRING_DESCRIPTOR_HEADER(USB_CFG_SERIAL_NUMBER_LENGTH),
 	'1', '0', '0', '0'
 };

static Gamepad *curGamepad = NULL;


/* ----------------------- hardware I/O abstraction ------------------------ */

static void hardwareInit(void)
{
	// init port C as input with pullup
	DDRC = 0x00;
	PORTC = 0xff;
	
	/* 1101 1000 bin: activate pull-ups except on USB lines 
	 *
	 * USB signals are on bit 0 and 2. 
	 *
	 * Bit 1 is connected with bit 0 (rev.C pcb error), so the pullup
	 * is not enabled.
	 * */
	PORTD = 0xf8;   

	/* Usb pin are init as outputs */
	DDRD = 0x01 | 0x04;    
	_delay_ms(11); 
	DDRD = 0x00;    /* 0000 0000 bin: remove USB reset condition */
			/* configure timer 0 for a rate of 12M/(1024 * 256) = 45.78 Hz (~22ms) */
	TCCR0 = 5;      /* timer 0 prescaler: 1024 */

	TCCR2 = (1<<WGM21)|(1<<CS22)|(1<<CS21)|(1<<CS20);
	OCR2 = 196; // for 60 hz

}

static uchar    reportBuffer[10];    /* buffer for HID reports */



/* ------------------------------------------------------------------------- */
/* ----------------------------- USB interface ----------------------------- */
/* ------------------------------------------------------------------------- */

static uchar    idleRates[MAX_REPORTS];           /* in 4 ms units */

usbMsgLen_t   usbFunctionDescriptor(struct usbRequest *rq)
{
	if ((rq->bmRequestType & USBRQ_TYPE_MASK) != USBRQ_TYPE_STANDARD)
		return 0;

	if (rq->bRequest == USBRQ_GET_DESCRIPTOR)
	{
		// USB spec 9.4.3, high byte is descriptor type
		switch (rq->wValue.bytes[1])
		{
			case USBDESCR_DEVICE:
				usbMsgPtr = rt_usbDeviceDescriptor;
				return rt_usbDeviceDescriptorSize;

			case USBDESCR_HID_REPORT:
				usbMsgPtr = rt_usbHidReportDescriptor;
				return rt_usbHidReportDescriptorSize;

		}
	}

	return 0;
}

static void gamepadVibrate(char on)
{
	if (curGamepad)
		if (curGamepad->setVibration)
				curGamepad->setVibration(on);
}

static int getGamepadReport(unsigned char *dstbuf, int id)
{
	if (curGamepad == NULL) {
		if (id==1) {
			dstbuf[0] = 1;
			dstbuf[1] = 0x7f;
			dstbuf[2] = 0x7f;
			dstbuf[3] = 0x7f;
			dstbuf[4] = 0x7f;
			dstbuf[5] = 0x7f;
			dstbuf[6] = 0x7f;
			dstbuf[7] = 0;
			dstbuf[8] = 0;

			return 9;
		}
		return 0;
	}
	else {
		return curGamepad->buildReport(dstbuf, id);
	}
}

static unsigned char _FFB_effect_index;
#define LOOP_MAX	0xFFFF
static unsigned int _loop_count;

static void effect_loop()
{
	if (_loop_count) {
		if (_loop_count != LOOP_MAX) {
			_loop_count--;
		}
	}
}

// Output Report IDs for various functions
#define REPORT_SET_EFFECT			0x01
#define REPORT_SET_PERIODIC			0x04
#define REPORT_SET_CONSTANT_FORCE	0x05	
#define REPORT_EFFECT_OPERATION		0x0A
#define REPORT_CREATE_EFFECT		0x09
#define REPORT_PID_POOL				0x0D

// For the 'Usage Effect Operation' report
#define EFFECT_OP_START			1
#define EFFECT_OP_START_SOLO	2
#define EFFECT_OP_STOP			3

// Feature report
#define PID_SIMULTANEOUS_MAX	3
#define PID_BLOCK_LOAD_REPORT	2

usbMsgLen_t	usbFunctionSetup(uchar data[8])
{
	usbRequest_t    *rq = (void *)data;
	int i;

	usbMsgPtr = reportBuffer;
	if((rq->bmRequestType & USBRQ_TYPE_MASK) == USBRQ_TYPE_CLASS){    /* class request type */
			
		switch(rq->bRequest)
		{
			case USBRQ_HID_GET_REPORT:
				{
					char error = 1;

					// USB 7.2.1 GET_report_request
					//
					// wValue high byte : report type
					// wValue low byte : report id
#if 1
					switch (rq->wValue.bytes[1]) // type
					{
						case 1: // input report
							if (rq->wValue.bytes[0]) {
								return getGamepadReport(reportBuffer, rq->wValue.bytes[0]);
							}
							if (rq->wValue.bytes[0] == 0x03) {
								reportBuffer[0] = rq->wValue.bytes[0];
								reportBuffer[1] = 1; // _FFB_effect_index;
								// 1 : Block load success, 2 : Block load full (OOM), 3 : block load error
								reportBuffer[2] = error;
								reportBuffer[3] = 1;
								reportBuffer[4] = 1;
	//							gamepadVibrate(1);
								return 5;
							}
													
							break;

						case 2: // output report
							break;

						case 3: // feature report
							if (rq->wValue.bytes[0] == PID_BLOCK_LOAD_REPORT) {
//								gamepadVibrate(1);
								reportBuffer[0] = rq->wValue.bytes[0];
								reportBuffer[1] = 0x1;
								reportBuffer[2] = 0x1;
								reportBuffer[3] = 10;
								reportBuffer[4] = 10;
								return 5;
							}
							else if (rq->wValue.bytes[0] == PID_SIMULTANEOUS_MAX) {
								reportBuffer[0] = rq->wValue.bytes[0];
								reportBuffer[1] = 1;
								reportBuffer[2] = 1;
								reportBuffer[3] = 0xff;
								reportBuffer[4] = 1;
								return 5;
							}
							break;
					}
#endif
				}

			case USBRQ_HID_SET_REPORT:
				{
					return USB_NO_MSG;	
				}

			case USBRQ_HID_GET_IDLE:
				if (rq->wValue.bytes[0] > 0 && rq->wValue.bytes[0] <= MAX_REPORTS) {
					usbMsgPtr = idleRates + (rq->wValue.bytes[0] - 1);
					return 1;
				}
				break;

			case USBRQ_HID_SET_IDLE:
				if (rq->wValue.bytes[0]==0) {
					for (i=0; i<MAX_REPORTS; i++)
						idleRates[i] = rq->wValue.bytes[1];
				}
				else {
					if (rq->wValue.bytes[0] > 0 && rq->wValue.bytes[0] <= MAX_REPORTS) {
						idleRates[rq->wValue.bytes[0]-1] = rq->wValue.bytes[1];
					}
				}
				break;
		}
	}else{
	/* no vendor specific requests implemented */
	}
	return 0;
}


static unsigned char vibration_on = 0;
static unsigned char constant_force = 0;
static unsigned char magnitude = 0;

static void decideVibration(void)
{
	if (!_loop_count)
		vibration_on = 0;

	if (!vibration_on)
	{
		gamepadVibrate(0);
	}
	else {
		if (constant_force > 0x7f) {
			gamepadVibrate(1);
		}
		if (magnitude > 0x7f) { 
			gamepadVibrate(1);
		}
	}
	
}

uchar usbFunctionWrite(uchar *data, uchar len)
{
	if (len < 1)
		return 1;

	//

	switch(data[0]) // Report ID
	{
		case REPORT_CREATE_EFFECT:
			_FFB_effect_index = data[1];
			break;

		case REPORT_SET_EFFECT:	
			/* Byte 0 : Effect block index
			 * Byte 1 : Effect type
			 * Bytes 2-3 : Duration
			 * Bytes 4-5 : Trigger repeat interval
			 * Bytes 6-7 : Sample period
			 * Byte 8 : Gain
			 * Byte 9 : Trigger button
			 */

		//	gamepadVibrate(1);
			_FFB_effect_index = data[1];
			break;

		case REPORT_SET_CONSTANT_FORCE:
			if (data[1]==1) {
				constant_force = data[2];
				decideVibration();
			}
			break;

		case REPORT_SET_PERIODIC:
			magnitude = data[2];
			decideVibration();
			break;

		case REPORT_EFFECT_OPERATION:
			if (len != 4)
				return 1;

			/* Byte 0 : report ID 
			 * Byte 1 : bit 7=rom flag, bits 6-0=effect block index
			 * Byte 2 : Effect operation
			 * Byte 3 : Loop count */
			_loop_count = data[3]<<3;

			switch(data[1] & 0x7F) // Effect block index
			{
				case 1: // constant force

				case 3: // square				
				case 4: // sine
					switch (data[2]) // effect operation
					{
						case EFFECT_OP_START:
							vibration_on = 1;
							decideVibration();
							break;

						case EFFECT_OP_START_SOLO:
							vibration_on = 1;
							decideVibration();
							break;	

						case EFFECT_OP_STOP:
							vibration_on = 0;
							decideVibration();
							break;
					}
					break;


				// TODO : should probably drop these from the descriptor since they are
				// not used.					

				case 2: // ramp
				case 5: // triangle
				case 6: // sawtooth up
				case 7: // sawtooth down
				case 8: // spring
				case 9: // damper
				case 10: // inertia
				case 11: // friction
				case 12: // custom force data
					break;
			}

			break;
	}

	return 1;
}

/* ------------------------------------------------------------------------- */

void transferGamepadReport(int id)
{
	if (usbInterruptIsReady())
	{ 	
		char len;
		int xfer_len;
		int j;

		len = getGamepadReport(reportBuffer, id);

		for (j=0; j<len; j+=8)
		{
			xfer_len = (len-j) < 8 ? (len-j) : 8;

			usbSetInterrupt(reportBuffer+j, xfer_len);
			while (!usbInterruptIsReady())
			{
				usbPoll();
				wdt_reset();
			}
		}
	}
}


int main(void)
{
	char must_report = 0, first_run = 1;
	uchar   idleCounters[MAX_REPORTS];
	int error_count = 0, i;

	memset(idleCounters, 0, MAX_REPORTS);

	hardwareInit();
	odDebugInit();

	/* I know both gamepads uses the same report descriptor and
	 * the same device descriptor */
	rt_usbHidReportDescriptor = gcn64_usbHidReportDescriptor;
	rt_usbHidReportDescriptorSize = getUsbHidReportDescriptor_size();
	rt_usbDeviceDescriptor = (void*)usbDescrDevice;
	rt_usbDeviceDescriptorSize = getUsbDescrDevice_size();

	// Do hardwareInit again. It causes a USB reset. 
	hardwareInit();
	gcn64protocol_hwinit();

	wdt_enable(WDTO_2S);
	usbInit();
	sei();
	DBG1(0x00, 0, 0);
	
	while (1) 
	{	
		wdt_reset();
	
		if (error_count > 30) {
			curGamepad = NULL;
		}

		if (curGamepad == NULL) {
			gamepadVibrate(0);
			error_count = 0;

			// this must be called at each 50 ms or less
			usbPoll();
			transferGamepadReport(1); // We know they all have only one
			_delay_ms(30);
			usbPoll();
	
			switch(gcn64_detectController())
			{
				case CONTROLLER_IS_N64:
					curGamepad = n64GetGamepad();
					curGamepad->init();
					gamepadVibrate(0);
					error_count = 0;
					break;

				case CONTROLLER_IS_GC:
					curGamepad = gamecubeGetGamepad();
					curGamepad->init();
					gamepadVibrate(0);
					error_count = 0;
					break;

					// Unknown means weird reply from the controller
					// try the old, bruteforce approach.
				case CONTROLLER_IS_UNKNOWN:
					/* Check for gamecube controller */
					curGamepad = gamecubeGetGamepad();
					curGamepad->init();
					if (curGamepad->probe()) {
						curGamepad = curGamepad;
						gamepadVibrate(0);
						error_count = 0;
						goto detected;
					}

					usbPoll();
					_delay_ms(40);
					usbPoll();

					/* Check for n64 controller */
					curGamepad = n64GetGamepad();
					curGamepad->init();
					if (curGamepad->probe())  {
						curGamepad = curGamepad;
						gamepadVibrate(0);
						error_count = 0;
						goto detected;
					}

					break;
			}
			continue;
		}

		
detected:		
		/* main event loop */
		wdt_reset();

		// this must be called at each 50 ms or less
		usbPoll();

		if (first_run) {
			decideVibration();
			if (curGamepad->update()) {
				error_count++;
			} else {
				error_count = 0;
			}
			first_run = 0;
		}

		/* Try to report at the granularity requested by
		 * the host. */
		if (TIFR & (1<<OCF2))
		{
			TIFR = 1<<OCF2;
			if (!must_report)
			{
				decideVibration();

				// Wait! Before doing this, let an USB interrupt occur. This
				// prevents USB interrupts from occuring during the
				// timing sensitive Gamecube/N64 communication.
				//
				// USB communication interrupts are triggering at regular
				// intervals on my machine. Between interrupts, we have 900uS of
				// free time.
				//
				// The trick here is to put the CPU in idle mode ; That is, wating
				// for interrupts, doing nothing. When the CPU resumes, an interrupt
				// has been serviced. The final delay helps when we get more than
				// once in a row (it happens, saw it on the scope. It was inserting
				// a huge delay in the command I was sending to the controller)
				//
				sleep_enable();
				sleep_cpu();
				sleep_disable();
				_delay_us(100);

				if (curGamepad->update()) {
					error_count++;
				} else {
					error_count = 0;
				}

				/* Check what will have to be reported */
				for (i=0; i<curGamepad->num_reports; i++) {
	                if (curGamepad->changed(i+1)) {
    	                must_report |= (1<<i);
        	        }
				}
            }

		}

		if(TIFR & (1<<TOV0)){   /* 22 ms timer */
			TIFR = 1<<TOV0;

			effect_loop();
			for (i=0; i<curGamepad->num_reports; i++)
			{
				// 0 means 
                if(idleRates[i] != 0){ 
                    if (idleCounters[i] > 4) {
                        idleCounters[i] -= 5;   /* 22 ms in units of 4 ms */
                    } else {
                        // reset the counter and schedule a report for this
                        idleCounters[i] = idleRates[i];
                        must_report |= (1<<i);
                    }
                }
			}
		}

		if(must_report)
		{
			for (i = 0; i < curGamepad->num_reports; i++)
			{
				if ((must_report & (1<<i)) == 0)
					continue;

				transferGamepadReport(i+1);
			}

			must_report = 0;
		}
	}
	return 0;
}


/* ------------------------------------------------------------------------- */
