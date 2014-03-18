/******************************************************************
*  BasicMicro Serial Wireless PS2 controller library
*  
*  Modified from Super amazing PS2 controller Arduino Library v1.8
*
*This program is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 3 of the License, or(at your option) any later version.
This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.
<http://www.gnu.org/licenses/>
*  
******************************************************************/


#ifndef BMPS2_h
#define BMPS2_h

#include <BMSerial.h>

#if ARDUINO > 22
#include "Arduino.h"
#else
#include "WProgram.h"
#endif

//These are our button constants
#define PSB_SELECT      0x0001
#define PSB_L3          0x0002
#define PSB_R3          0x0004
#define PSB_START       0x0008
#define PSB_PAD_UP      0x0010
#define PSB_PAD_RIGHT   0x0020
#define PSB_PAD_DOWN    0x0040
#define PSB_PAD_LEFT    0x0080
#define PSB_L2          0x0100
#define PSB_R2          0x0200
#define PSB_L1          0x0400
#define PSB_R1          0x0800
#define PSB_GREEN       0x1000
#define PSB_RED         0x2000
#define PSB_BLUE        0x4000
#define PSB_PINK        0x8000
#define PSB_TRIANGLE    0x1000
#define PSB_CIRCLE      0x2000
#define PSB_CROSS       0x4000
#define PSB_SQUARE      0x8000

//These are stick values
#define PSS_RX 2
#define PSS_RY 3
#define PSS_LX 4
#define PSS_LY 5

//These are analog buttons
#define PSAB_PAD_LEFTRIGHT	0
#define PSAB_PAD_UPDOWN		1
#define PSAB_L2          	11
#define PSAB_R2          	13
#define PSAB_L1          	10
#define PSAB_R1          	12
#define PSAB_GREEN       	6
#define PSAB_RED         	7
#define PSAB_BLUE        	8
#define PSAB_PINK        	9
#define PSAB_TRIANGLE    	6
#define PSAB_CIRCLE      	7
#define PSAB_CROSS       	8
#define PSAB_SQUARE      	9

#define PSABC_NONE			0	// No button Pressures
#define PSABC_PAD			1	// The PAD UP/DOwn/LEFT/RIGHT
#define PSABC_ALL			2	// all the pressures


class BMPS2 : public BMSerial{
private:
	unsigned int rate;
public:
	
	BMPS2(uint8_t pin, unsigned int updaterate=30, uint8_t pressures=PSABC_ALL) : BMSerial(pin, pin)
	{
		begin(57600);
		lasttime = 0;
		buffercrc = bufferindex = 0;
		buttonpressed = buttonreleased = 0;
		updated = false;
		rate = updaterate;
		switch (pressures) {
		  case 0:	// no pressures
			readCmd = 0x0f00;		// just LX, LY, RX, RY
			readMask = 0xc03c3f;	// Which bytes have valid data... including fixed plus checksum...
			break;
		  case 1:    // Add PSAB PAD values
			readCmd = 0x3f00;		// just DPAD + LX, LY, RX, RY
			readMask = 0xc03fff;	// Which bytes have valid data... including fixed plus checksum...
			break;
		  default: 		// all pressures 
			readCmd = 0x7fff;
			readMask = 0xffffff;
		}
	};
	
	boolean isUpdated(){
		read_ps2();
		boolean state = updated;
		updated = false;
		return state;
	};
	
	void setRate(unsigned int updaterate)
	{
		rate = updaterate;
	};
	
	
	boolean button(uint16_t);
	boolean newButtonState();
	boolean newButtonState(unsigned int);
	boolean buttonPressed(unsigned int);
	boolean buttonReleased(unsigned int);
	int analog(byte);
	void motors(boolean motor1, byte motor2);

private:
	unsigned int buttons;
	unsigned int buttonpressed;
	unsigned int buttonreleased;

	unsigned int readCmd;
	unsigned long readMask;

	int dpadx,dpady;
	char rxaxis,ryaxis;
	char lxaxis,lyaxis;
	
	unsigned char data[22];

	boolean updated;
	
	void read_ps2();

	long lasttime;
	unsigned char bufferindex;
	uint16_t buffercrc;
};

#endif



