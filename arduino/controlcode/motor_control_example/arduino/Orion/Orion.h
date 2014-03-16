/*
This library is free software; you can redistribute it and/or
modify it under the terms of the GNU Lesser General Public
License as published by the Free Software Foundation; either
version 2.1 of the License, or (at your option) any later version.

This library is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
Lesser General Public License for more details.

You should have received a copy of the GNU Lesser General Public
License along with this library; if not, write to the Free Software
Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
*/

#ifndef Orion_h
#define Orion_h

#include "Arduino.h"
#include <../SPI/SPI.h>

/******************************************************************************
* Definitions
******************************************************************************/

#define _ORION_VERSION 0x100	//Orion firmware version supported

#define _SS_VERSION 10 // software version of this library

#ifndef GCC_VERSION
#define GCC_VERSION (__GNUC__ * 10000 + __GNUC_MINOR__ * 100 + __GNUC_PATCHLEVEL__)
#endif

/*******************
Comp6DOF_n0m1 Fixed Point functions
*******************/
const int K1 = 5701;
const int K2 = -1645;
const int K3 = 446;
const unsigned int MINDELTADIV = 1;              /* final step size for divInt */

#define SINE_INDEX_WIDTH 4
#define SINE_INTERP_WIDTH 8

#if (SINE_INDEX_WIDTH + SINE_INTERP_WIDTH > 12)
	#error Invalid sine widths
#endif

#define SINE_INDEX_OFFSET (12 - SINE_INDEX_WIDTH)
#define SINE_INTERP_OFFSET (SINE_INDEX_OFFSET - SINE_INTERP_WIDTH)
#define QUADRANT_HIGH_MASK (1 << 13)
#define QUADRANT_LOW_MASK (1 << 12)
#if SINE_INTERP_OFFSET > 0
	#define SINE_ROUNDING (1 << (SINE_INTERP_OFFSET-1))
#else
	#define SINE_ROUNDING (0)
#endif

#define BITS(_VALUE_, _WIDTH_, _BIT_) (((_VALUE_) >> (_BIT_)) & ((1 << (_WIDTH_)) - 1))
#define SINE_TABLE_SIZE (1 << SINE_INDEX_WIDTH)
// Table of the first quadrant values.  Size is + 1 to store the first value of
// the second quadrant, hence we're storing 0 <= degrees <= 90.
static const int16_t sinLUT[SINE_TABLE_SIZE + 1] = {
        0,  3211,  6392,  9511, 12539, 15446, 18204, 20787,
    23169, 25329, 27244, 28897, 30272, 31356, 32137, 32609,
    32767
};

class OrionClass
{
#define DEADZONE 10

public:
#define BTNA !digitalRead(2)
#define BTNB !digitalRead(4)

	enum GAITTYPE{
		RG6,RG12,QR9,TRI4,TRI6,TRI8,TRIO12,TRIO18,WAVE12,WAVE18
	};

#define NOTE_C1 33
#define NOTE_CS1 35
#define NOTE_D1 37
#define NOTE_EF1 39
#define NOTE_E1 41
#define NOTE_F1 44
#define NOTE_FS1 46
#define NOTE_G1 49
#define NOTE_GS1 52
#define NOTE_A1 55
#define NOTE_BF1 58
#define NOTE_B1 62 

#define NOTE_C2 65
#define NOTE_CS2 69
#define NOTE_D2 73
#define NOTE_EF2 78
#define NOTE_E2 82
#define NOTE_F2 87
#define NOTE_FS2 93
#define NOTE_G2 98
#define NOTE_GS2 104
#define NOTE_A2 110
#define NOTE_BF2 117
#define NOTE_B2 124 

#define NOTE_C3 131
#define NOTE_CS3 139
#define NOTE_D3 147
#define NOTE_EF3 156
#define NOTE_E3 165
#define NOTE_F3 175
#define NOTE_FS3 185
#define NOTE_G3 196
#define NOTE_GS3 208
#define NOTE_A3 220
#define NOTE_BF3 233
#define NOTE_B3 247 

#define NOTE_C4 262
#define NOTE_CS4 277
#define NOTE_D4 294
#define NOTE_EF4 311
#define NOTE_E4 330
#define NOTE_F4 349
#define NOTE_FS4 370
#define NOTE_G4 392
#define NOTE_GS4 415
#define NOTE_A4 440
#define NOTE_BF4 466
#define NOTE_B4 494

#define NOTE_C5 523
#define NOTE_CS5 554
#define NOTE_D5 587
#define NOTE_EF5 622
#define NOTE_E5 659
#define NOTE_F5 699
#define NOTE_FS5 740
#define NOTE_G5 784
#define NOTE_GS5 831
#define NOTE_A5 880
#define NOTE_BF5 932
#define NOTE_B5 988

#define NOTE_C6 1047
#define NOTE_CS6 1109
#define NOTE_D6 1175
#define NOTE_EF6 1245
#define NOTE_E6 1319
#define NOTE_F6 1397
#define NOTE_FS6 1480
#define NOTE_G6 1568
#define NOTE_GS6 1661
#define NOTE_A6 1760
#define NOTE_BF6 1865
#define NOTE_B6 1976

#define NOTE_C7 2093
#define NOTE_CS7 2217
#define NOTE_D7 2349
#define NOTE_EF7 2489
#define NOTE_E7 2637
#define NOTE_F7 2794
#define NOTE_FS7 2960
#define NOTE_G7 3136
#define NOTE_GS7 3322
#define NOTE_A7 3520
#define NOTE_BF7 3729
#define NOTE_B7 3951

private:
#define GREENLED A1
#define REDLED A0

#define cmdNOP 				0x00
#define cmdPOffset 			0x01
#define cmdAOffset 			0x02
#define cmdPulse 				0x03
#define cmdAngle 				0x04
#define cmdSpeed 				0x05
#define cmdTime 				0x06
#define cmdSetAccel 			0x07
#define cmdSetAccelJerk 	0x08
#define cmdSetDeccel 		0x09
#define cmdSetDeccelJerk 	0x0A
#define cmdPinHigh 			0x0B
#define cmdPinLow 			0x0C
#define cmdSetPort 			0x0D
#define cmdQueryMove 		0x0E
#define cmdQueryPOffset		0x0F
#define cmdQueryAOffset		0x10
#define cmdQueryPulse 		0x11
#define cmdQueryAngle 		0x12
#define cmdQueryFBPulse 	0x13
#define cmdQueryFBAngle 	0x14

#define cmdQueryFBError 	0x16
#define cmdQueryVersion 	0x17
#define cmdQueryVoltage		0x18
#define cmdPinState 			0x19
#define cmdPinStateL 		0x1A
#define cmdResetRegs 		0x1B
#define cmdGetReg 			0x1C
#define cmdSetReg 			0x1D
#define cmdStopMove 			0x1E
#define cmdStopPulse 		0x1F
#define cmdWriteEEprom 		0x20
#define cmdReadEEprom 		0x21
#define cmdGroup 				0x22
#define cmdSetXPos 			0x23
#define cmdSetYPos 			0x24
#define cmdSetZPos 			0x25
#define cmdSetWPos			0x26
#define cmdWriteRegisters	0x27
#define cmdCalcMove 			0x28
#define cmdGetXPos			0x29
#define cmdGetYPos			0x2A
#define cmdGetZPos			0x2B
#define cmdGetWPos			0x2C

#define cmdSet1XYZPos 		0x30
#define cmdSet2XYZPos 		0x31
#define cmdSet3XYZPos 		0x32
#define cmdSet4XYZPos 		0x33
#define cmdSet5XYZPos 		0x34
#define cmdSet6XYZPos 		0x35
#define cmdSet7XYZPos 		0x36
#define cmdSet8XYZPos 		0x37

#define cmdQueryGroup1		0x43
#define cmdQueryGroup2		0x44
#define cmdQueryGroup3		0x45
#define cmdQueryGroup4		0x46
#define cmdQueryGroup5		0x47
#define cmdQueryGroup6		0x48
#define cmdQueryGroup7		0x49
#define cmdQueryGroup8		0x4A
#define cmdQueryForce		0x4B
#define cmdSetForce			0x4C
#define cmdEnableForce		0x4D
#define cmdEnableIKForce	0x4E
#define cmdQueryForceTrig			0x4F
#define cmdQueryIKForceTrig		0x50
#define cmdEnableIKMinMax			0x51

#define cmdSetServoDir		0x80
#define cmdSetServoIKDir	0x81
#define cmdSetServoIKPin	0x82
#define cmdSetServoIKLength		0x83
#define cmdSetServoMin		0x84
#define cmdSetServoMax		0x85
#define cmdSetServoDegree	0x86
#define cmdQueryServoMin	0x87
#define cmdQueryServoMax	0x88

#define cmdExecute 			0xAA

private:
//Fixed Point Sin/Atan2 functions from Comp6DOF_n0m1 library
int atan2Int(int y, int x)
{
  int result;    /* angle in degrees times 100 */

  /* check for -32768 which is not handled correctly */
  if (x == -32768) x = -32767;
  if (y == -32768) y = -32767;

  /* check for quadrants */
  if ((x >= 0) && (y >= 0))  /* range 0 to 90 degrees */
    result = atanInt(y, x);
  else if ((x <= 0) && (y >= 0))  /* range 90 to 180 degrees */
    result = (int)(1800 - (int)atanInt(y, (int)-x));
  else if ((x <= 0) && (y <= 0))  /* range -180 to -90 degrees */
    result = (int)((int)-1800 + atanInt((int)-y, (int)-x));
  else /* x >=0 and y <= 0 giving range -90 to 0 degrees */
    result = (int)(-atanInt((int)-y, x));

  return result;
}

int atanInt(int y, int x)
{
  long angle;   /* angle in degrees times 100 */
  int ratio;   /* ratio of y / x or vice versa */
  long tmp;     /* temporary variable */

  /* check for pathological cases */
  if ((x == 0) && (y == 0)) return (0);
  if ((x == 0) && (y != 0)) return (900);

  /* check for non-pathological cases */
  if (y <= x)
  {
    ratio = ((long)y*0x4000)/(long)x;
  }
  else
  {
    ratio = ((long)x*0x4000)/(long)y;
  }

  /* first, third and fifth order polynomial approximation */
  angle = (long) K1 * (long) ratio;
  tmp = ((long) ratio >> 5) * ((long) ratio >> 5) * ((long) ratio >> 5);
  angle += (tmp >> 15) * (long) K2;
  tmp = (tmp >> 20) * ((long) ratio >> 5) * ((long) ratio >> 5);
  angle += (tmp >> 15) * (long) K3;
  angle = angle >> 15;

  /* check if above 45 degrees */
  if (y > x) angle = (int)(9000 - angle);

  /* for tidiness, limit result to range 0 to 9000 equals 0.0 to 90.0 degrees */
  if (angle < 0) angle = 0;
  if (angle > 9000) angle = 9000;

  return ((int) angle/10);
}

int cosInt(int angle)
{
	angle+=900;
	return sinInt(angle);
}

int sinInt(unsigned int angle)
{
	if(angle>=0x8000)
		angle+=3600;
	angle = (long)angle*0x4000/3600;	//convert to 2.14 fixed point units

   angle += SINE_ROUNDING;
	long interp = BITS(angle, SINE_INTERP_WIDTH, SINE_INTERP_OFFSET);
	byte index = BITS(angle, SINE_INDEX_WIDTH, SINE_INDEX_OFFSET);

	bool isOddQuadrant = (angle & QUADRANT_LOW_MASK) == 0;
	bool isNegativeQuadrant = (angle & QUADRANT_HIGH_MASK) != 0;
    
	if (!isOddQuadrant) {
		index = SINE_TABLE_SIZE - 1 - index;
	}
	
    // Do calculations with 32 bits since the multiplication can overflow 16 bits
	long x1 = sinLUT[index];
	long x2 = sinLUT[index+1];
   long approximation = ((x2-x1) * interp) >> SINE_INTERP_WIDTH;
    
	int sine;
	if (isOddQuadrant) {
		sine = x1 + approximation;
	} else {
		sine = x2 - approximation;
	}
    
	if (isNegativeQuadrant) {
		sine *= -1;
	}

	return sine;
}

	unsigned char readindex;
	unsigned char inpacket[4];
	unsigned int inpacketcrc;
	unsigned int outpacketcrc;
	unsigned char transfer(unsigned char outdata,boolean calcincrc=false){
		//ccitt crc calculation
		unsigned int x = ((outpacketcrc>>8) ^ outdata) & 0xff; 
		x ^= x>>4; 
		outpacketcrc = (outpacketcrc << 8) ^ (x << 12) ^ (x <<5) ^ x; 					
		
		unsigned int indata = SPI.transfer(outdata);
		
		if(calcincrc){
			x = ((inpacketcrc>>8) ^ indata) & 0xff; 
			x ^= x>>4; 
			inpacketcrc = (inpacketcrc << 8) ^ (x << 12) ^ (x <<5) ^ x; 					
		}
		
		return indata;
	};
	
	unsigned int WritePacket(unsigned char cmd){
		return WritePacket(cmd,0,0,0,0,0,0);
	};

	unsigned int WritePacket(unsigned char cmd, unsigned char arg1){
		return WritePacket(cmd,arg1,0,0,0,0,0);
	};

	unsigned int WritePacket(unsigned char cmd, int arg1){
		return WritePacket(cmd,arg1>>8,arg1&0xFF,0,0,0,0);
	};

	unsigned int WritePacket(unsigned char cmd, unsigned int arg1){
		return WritePacket(cmd,arg1>>8,arg1&0xFF,0,0,0,0);
	};

	unsigned int WritePacket(unsigned char cmd, unsigned char arg1, unsigned char arg2){
		return WritePacket(cmd,arg1,arg2,0,0,0,0);
	};

	unsigned int WritePacket(unsigned char cmd, unsigned char arg1, int arg2){
		return WritePacket(cmd,arg1,arg2>>8,arg2&0xFF,0,0,0);
	};

	unsigned int WritePacket(unsigned char cmd, unsigned char arg1, unsigned int arg2){
		return WritePacket(cmd,arg1,arg2>>8,arg2&0xFF,0,0,0);
	};

	unsigned int WritePacket(unsigned char cmd, int arg1, int arg2, int arg3){
		return WritePacket(cmd,arg1>>8,arg1&0xFF,arg2>>8,arg2&0xFF,arg3>>8,arg3&0xFF);
	};

	unsigned int WritePacket(unsigned char cmd, unsigned char val1, unsigned char val2, unsigned char val3, unsigned char val4, unsigned char val5, unsigned char val6){
		unsigned int incrc,outcrc;
		do{
			outpacketcrc=0xFFFF;
			inpacketcrc=0xFFFF;
			
			transfer(cmd);
			delayMicroseconds(8);
			inpacket[0] = transfer(val1,true);
			delayMicroseconds(4);
			inpacket[1] = transfer(val2,true);
			delayMicroseconds(4);
			inpacket[2] = transfer(val3,true);
			delayMicroseconds(4);
			inpacket[3] = transfer(val4,true);
			delayMicroseconds(4);
			incrc = transfer(val5)<<8;
			delayMicroseconds(7);
			incrc |= transfer(val6);
			delayMicroseconds(12);
			outcrc = SPI.transfer(outpacketcrc>>8)<<8;
			delayMicroseconds(12);
			outcrc |= SPI.transfer(outpacketcrc);

			if(outpacketcrc!=outcrc){
				delayMicroseconds(1000);
			}
		}while(outpacketcrc!=outcrc);

		return incrc;
	};
	   
	unsigned char ReadPacketB(unsigned char cmd,unsigned delay=1000){
		unsigned int incrc;
		do{
			WritePacket(cmd,++readindex);
			delayMicroseconds(delay);
			for(int i=0;i<20;i++){
				incrc=WritePacket(cmdNOP);
				delayMicroseconds(delay);
				if(inpacketcrc==incrc && inpacket[0]==readindex){
					break;
				}
			}
		}while(inpacketcrc!=incrc || inpacket[0]!=readindex);
	
		return inpacket[1];
	}

	unsigned char ReadPacketB(unsigned char cmd,unsigned char arg1,unsigned delay=1000){
		unsigned int incrc;
		do{
			WritePacket(cmd,++readindex,arg1);
			delayMicroseconds(delay);
			for(int i=0;i<20;i++){
				incrc=WritePacket(cmdNOP);
				delayMicroseconds(delay);
				if(inpacketcrc==incrc && inpacket[0]==readindex){
					break;
				}
			}
		}while(inpacketcrc!=incrc || inpacket[0]!=readindex);
	
		return inpacket[1];
	}

	unsigned int ReadPacketW(unsigned char cmd,unsigned delay=1000){
		unsigned int incrc;
		do{
			WritePacket(cmd,++readindex);
			delayMicroseconds(delay);
			for(int i=0;i<20;i++){
				incrc=WritePacket(cmdNOP);
				delayMicroseconds(delay);
				if(inpacketcrc==incrc && inpacket[0]==readindex){
					break;
				}
			}
		}while(inpacketcrc!=incrc || inpacket[0]!=readindex);

		return (inpacket[1]<<8)|inpacket[2];
	};

	unsigned int ReadPacketW(unsigned char cmd,unsigned char arg1,unsigned delay=1000){
		unsigned int incrc;
		do{
			WritePacket(cmd,++readindex,arg1);
			delayMicroseconds(delay);
			for(int i=0;i<20;i++){
				incrc=WritePacket(cmdNOP);
				delayMicroseconds(delay);
				if(inpacketcrc==incrc && inpacket[0]==readindex){
					break;
				}
			}
		}while(inpacketcrc!=incrc || inpacket[0]!=readindex);

		return (inpacket[1]<<8)|inpacket[2];
	};
		
	enum legtype{
		cRR,cRM,cRF,cLR,cLM,cLF
	}leg;

	GAITTYPE GaitType;

	int LegDroop;
	unsigned char LiftSteps;
	unsigned char LowerSteps;
	unsigned char UpSteps;
	unsigned char DownSteps;
	unsigned char StepsInGait;
	unsigned char GaitLeg[6];
	int GaitStep;
	int SubStep;
	
	boolean LegRaised[6];
	long GaitPosX[6];
	long GaitPosY[6];
	long GaitPosZ[6];
	long GaitPosR[6];

	long LastXPos[6];
	long LastYPos[6];
	long LastZPos[6];
	
	int BodyPosX[6];
	int BodyPosY[6];
	int BodyPosZ[6];

	int LegPosX[6];
	int LegPosY[6];
	int LegPosZ[6];

	int curXdown,curZdown,curRdown;		

	int BalanceMode;
 	long BalanceTransRate;
  	int BalanceAngleRate;
	long BalanceTransX;
	long BalanceTransZ;
	long BalanceTransY;
	int BalancePitch;
	int BalanceRoll;

	long lastalarm;
	long lastalarmcheck;
	int lipolow;
	int lipoverylow;
	int lipocritical;

	boolean CalcLegStep(unsigned char LegIndex,int Xdist,int Ydist,int Zist,int Rdist,int Xdown,int Zdown,int Rdown,int Rate,boolean EnableForce);

	void UpdateGaitSelect();

public:
	void setBalanceMode(int mode) { BalanceMode=mode; };
	void setBalanceTransRate(long rate) { BalanceTransRate=rate; };
	void setBalanceAngleRate(int rate) { BalanceAngleRate=rate; };
	int getBalanceMode() { return BalanceMode; };
	int getBalanceTransX() { return BalanceTransX; };
	int getBalanceTransY() { return BalanceTransY; };
	int getBalanceTransZ() { return BalanceTransZ; };
	int getBalancePitch() { return BalancePitch; };
	int getBalanceRoll() { return BalanceRoll; };
	
	void setBodyPos(unsigned char leg,int x,int y,int z){
		BodyPosX[leg] = x;
		BodyPosY[leg] = y;
		BodyPosZ[leg] = z;
	};
	
	void setLegPos(unsigned char leg,int x,int y,int z){
		LegPosX[leg] = x+BodyPosX[leg];
		LegPosY[leg] = y+BodyPosY[leg];
		LegPosZ[leg] = z+BodyPosZ[leg];
	};

	// public methods
	OrionClass();
	~OrionClass();

	void begin();
	void end();

   void setPOffset(unsigned char pin, int offset);
   void setAOffset(unsigned char pin, int offset);
   void setPulse(unsigned char pin, int pulse);
   void setAngle(unsigned char pin, int angle);
   void setSpeed(unsigned int time);
   void setTime(unsigned int time);
   void setPinHigh(unsigned char pin);
   void setPinLow(unsigned char pin);
   void setPort(unsigned char port,unsigned char bits);
   unsigned int queryMove();
   int queryPOffset(unsigned char pin);
   int queryAOffset(unsigned char pin);
   int queryPulse(unsigned char pin);
   int queryAngle(unsigned char pin);
   int queryFBPulse(unsigned char pin);
   int queryFBAngle(unsigned char pin);
   int queryFBError(unsigned char pin);
   unsigned int queryVersion();
   unsigned int queryVoltage();
   unsigned int pinState(unsigned char pin);
   unsigned int pinStateL(unsigned char pin);
   void resetRegs();
   unsigned int getReg(unsigned char reg);
   void setReg(unsigned char reg,unsigned int data);
   void stopMove(unsigned char group);
   void stopPulse(unsigned char pin);
   void group(unsigned char group);
   void calcMove();
   void setXPos(int pos);
   void setYPos(int pos);
   void setZPos(int pos);
   void setWPos(int pos);
   void setXYZPos(unsigned char index,int x,int y,int z);
   void writeRegisters();
	int getXPos(unsigned char index);
	int getYPos(unsigned char index);
	int getZPos(unsigned char index);
	int getWPos(unsigned char index);
	int queryForce(unsigned char index);
	void setForce(unsigned char index,int force);
	void enableForce(unsigned char index,unsigned char enable);
	void enableIKForce(unsigned char leg,unsigned char joint,unsigned char enable);
	unsigned char queryForceTrig(unsigned char index);
	unsigned char queryIKForceTrig(unsigned char leg,unsigned char joint);
	void setServoDir(unsigned char index,unsigned char dir);
	void setServoIKDir(unsigned char leg,unsigned char dir);
	void setServoIKPin(unsigned char leg,unsigned char joint,unsigned char pin);
	void setServoIKLength(unsigned char leg,unsigned char joint,unsigned int length);
	void setServoMin(unsigned char index,int min);
	void setServoMax(unsigned char index,int max);
	void setServoDegree(unsigned char index,unsigned int degree);
   int queryServoMin(unsigned char pin);
   int queryServoMax(unsigned char pin);
	void enableIKMinMax(boolean state);
	boolean queryAtLimits();

   void execute();

	void gaitSelect(GAITTYPE sel);
	void execGait(int BodyYaw,int BodyRoll,int BodyPitch,int BodyRotOffsetX,int BodyRotOffsetY,int BodyRotOffsetZ,int BodyOffsetX,int BodyOffsetY,int BodyOffsetZ,int Xdist,int Ydist,int Zdist,int Rdist,int XLegAdj,int ZLegAdj,int Rate,boolean EnableForce=false);
	
	void tone(unsigned int note,unsigned int time,boolean wait=true){
      ::tone(3,note,time);
      if(wait)
      	delay(time);	//delay while tone plays so Serial commands dont execute.  Otherwise Serial interrupts will distort tones generated
	};
	
	boolean checkLipo();
	
	void green(boolean state){
		pinMode(GREENLED, OUTPUT);
		if(state)
			digitalWrite(GREENLED, HIGH);
		else
			digitalWrite(GREENLED, LOW);
	};
	void red(boolean state){
		pinMode(REDLED, OUTPUT);
		if(state)
			digitalWrite(REDLED, HIGH);
		else
			digitalWrite(REDLED, LOW);
	};
};

extern OrionClass Orion;

// Arduino 0012 workaround
#undef int
#undef char
#undef long
#undef byte
#undef float
#undef abs
#undef round

#endif
