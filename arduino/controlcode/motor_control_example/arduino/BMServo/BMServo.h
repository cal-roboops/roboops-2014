/*
 BMServo.cpp - Hardware driven Servo library for Arduino using 16 bit timers
 Copyright (c) 2012 Nathan Scherdin.  All right reserved.
 
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

#ifndef BMServo_h
#define BMServo_h

#include <Arduino.h> 
#include <inttypes.h>

#define Servo_VERSION           1      // software version of this library

class BMServo
{
private:
	#define SetServo9(angle) OCR1A=(((((long)angle-(long)servooffset)*134)/10)+24000)/8
	#define StopServo9 OCR1A=28
	#define SetServo10(angle) OCR1B=(((((long)angle-(long)servooffset)*134)/10)+24000)/8
	#define StopServo10 OCR1B=28

public:
	BMServo(unsigned char pin){
		servopin = pin;
		servoangle = 0;
		servonewangle = 0;
		lastupdate = millis();
		active=false;
	};

	void begin()
	{
		TCCR1A = (TCCR1A & (_BV(COM1A0) | _BV(COM1A1) | _BV(COM1B0) | _BV(COM1B1))) | _BV(WGM11);
		TCCR1B = TCCR1A | _BV(WGM13) | _BV(WGM12) | _BV(CS11);

		if(servopin==9){
			StopServo9;
			digitalWrite(9, LOW);
			pinMode(9, OUTPUT);
			TCCR1A = (TCCR1A & ~_BV(COM1A0)) | _BV(COM1A1);
		}
		if(servopin==10){
			StopServo10;
			digitalWrite(10, LOW);
			pinMode(10, OUTPUT);
			TCCR1A = (TCCR1A & ~_BV(COM1B0)) | _BV(COM1B1);
		}
		ICR1 = clockCyclesPerMicrosecond()*(20000L)/8;
	}
		
	void setAOffset(int offset){
		servooffset = offset;
	}

	int getAOffset(){
		return servooffset;
	}

	void setAngle(int angle,int time){
		active=true;
		servonewangle=angle;
		servorate=(servonewangle-servoangle)*20/time;
	};
	
	void update()
	{
		if((millis()-lastupdate)>=20){
			lastupdate = millis();
			if(active){
				servoangle+=servorate;
				if(servorate>0 && servoangle>servonewangle){
					servoangle=servonewangle;
					servorate=0;
				}
				if(servorate<0 && servoangle<servonewangle){
					servoangle=servonewangle;
					servorate=0;
				}
			
				if(servopin==9)
					SetServo9(servoangle);
				if(servopin==10)
					SetServo10(servoangle);
			}
		}
	};
	
	int queryAngle(){
		return servoangle;
	};
	
	int queryFBAngle(){
		int angle;
		if(servopin==9){
			TCCR1A = TCCR1A & ~(_BV(COM1A0) | _BV(COM1A1));
			delay(20);  //allow time for any PWM pulse to finish
			do{
				angle = -GetServoPos();
			}while(angle==0);
			TCCR1A = (TCCR1A & ~_BV(COM1A0)) | _BV(COM1A1);
		}
		if(servopin==10){
			TCCR1A = TCCR1A & ~(_BV(COM1B0) | _BV(COM1B1));
			delay(20);  //allow time for any PWM pulse to finish
			do{
				angle = -GetServoPos();
			}while(angle==0);
			TCCR1A = (TCCR1A & ~_BV(COM1B0)) | _BV(COM1B1);
		}
		return angle-servooffset;
	};
	
	void stopPulse(){
		active=false;
		if(servopin==9)
			StopServo9;
		if(servopin==10)
			StopServo10;
	};
	
private:
	boolean active;
	int servoangle,servonewangle,servorate,servooffset;
	unsigned char servopin;
	double lastupdate;
	
	int GetServoPos()
	{
		unsigned char old_SPCR = SPCR;
		if(servopin==10)
			SPCR &= ~_BV(SPE);

		unsigned long width = 0;
		unsigned long numloops = 0;

		uint8_t bit = digitalPinToBitMask(servopin);
		uint8_t port = digitalPinToPort(servopin);
		uint8_t stateMask = bit;

		digitalWrite(servopin, LOW);
		delay(10);
		digitalWrite(servopin, HIGH);
		delayMicroseconds(31);
		digitalWrite(servopin, LOW);
		pinMode(servopin, INPUT);

		// wait for any previous pulse to end
		while ((*portInputRegister(port) & bit) == stateMask)
			if(numloops++>100)
				return 0;
		// wait for the pulse to start
		while ((*portInputRegister(port) & bit) != stateMask)
			if(numloops++>100)
				return 0;
		// wait for the pulse to stop
		while ((*portInputRegister(port) & bit) == stateMask) {
			width++;
		}

		pinMode(servopin, OUTPUT);
		if(servopin==10)
			SPCR = old_SPCR;
		
		return (long)(width*130/100)-1527;  //10ths of degree
	}
};

#endif