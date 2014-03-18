//#include <util/crc16.h> //we may use a built in crc caluclation later but not right now
#include "BMPS2.h"

#if ARDUINO > 22
#include "Arduino.h"
#else
#include "WProgram.h"
#include "pins_arduino.h"
#endif

boolean BMPS2::newButtonState() {
	return newButtonState(0xFFFF);   
}

boolean BMPS2::newButtonState(unsigned int buttonmask) {
   return (buttonPressed(buttonmask)||buttonReleased(buttonmask))!=0;
}

boolean BMPS2::button(uint16_t buttonmask) {
   return (buttons&buttonmask)!=0;
}

boolean BMPS2::buttonPressed(unsigned int buttonmask) {
   read_ps2();
   return (buttonpressed&buttonmask)!=0;
}

boolean BMPS2::buttonReleased(unsigned int buttonmask) {
   read_ps2();
   return (buttonreleased&buttonmask)!=0;
}
  
int BMPS2::analog(byte button) {
   read_ps2();
   if(button<2){
   	return (int) ((data[(button*2)+6]<<8) | data[(button*2)+7]);
   }
   if(button<6){
   	return (char)data[button+8];
   }
	return data[button+8];
}

void BMPS2::motors(boolean motor1, byte motor2) {
	write(0x80);
	write((motor1? 0x80:0)	| (motor2 & 0x7f));
}

//===============================================
//Any read command will always return:
//2 digital button bytes
//2 last down button bytes
//2 last up button bytes
// 
//Bit mask data:
//0 = R2
//1 = R1
//2 = L2
//3 = L1
//4 = Square
//5 = Cross
//6 = Circle
//7 = Tri
//8 = LY
//9 = LX
//10 = RY
//11 = RX
//12 = DPAD Up/Down (2 bytes) 
//13 = DPAD Left/Right (2 bytes)
//14 = unused
// 0x7ffff -> 16 bytes
// 2 bytes Checksum
void BMPS2::read_ps2()
{
	long time = millis() - lasttime;
	int newdata;
	unsigned int x;
	unsigned char tdata[24];
	if(time>=rate){	//ps2 only updates data every 30ms
		lasttime = millis();

		bufferindex=0;
		buffercrc=0xFFFF;
		unsigned long mask = readMask;

		flush();	//flush buffer incase there is any garbage
		
		//command mask to read back all data
		listen();
		write(readCmd >> 8);
		write(readCmd & 0xff);
		
		// Now we used to simply delay for 8ms, but this is longer than the whole recv should take...
		while (mask && ((millis()-lasttime) < 10)) { // should never take this long, but...
			if (mask & 1) {
				newdata = read(0);
				if (newdata != -1) {
					if(bufferindex<22){
						//ccitt crc calculation
						x = ((buffercrc>>8) ^ newdata) & 0xff; 
						x ^= x>>4; 
						buffercrc = (buffercrc << 8) ^ (x << 12) ^ (x <<5) ^ x;
					}
					// Save away the byte... 
					tdata[bufferindex++]=newdata;
					mask >>= 1;	// shift down 1...
				}
			}
			else {
				// Bit not used so bypass it...
				tdata[bufferindex++]=0;	// don't fill with complete garbage...
				mask >>= 1;
			}
		}
		if (!mask) {
			// we read everything including checksum bytes...
			unsigned int crc = (tdata[22]<<8) | tdata[23];
			if(buffercrc==crc){
				updated = true;
				for(int i=0;i<22;i++){
					data[i]=tdata[i];
				}
				buttons=~((data[0]<<8)|(data[1]&0xFF));
				buttonpressed=((data[2]<<8)|(data[3]&0xFF));
				buttonreleased=((data[4]<<8)|(data[5]&0xFF));
				dpadx = ((data[6]<<8)|(data[7]&0xFF));
				dpady = ((data[8]<<8)|(data[9]&0xFF));
				rxaxis = data[10];
				ryaxis = data[11];
				lxaxis = data[12];
				lyaxis = data[13];
			}
#ifdef DEBUG
			else{
				Serial.print("Bad packet ");
				for (int j=0; j < 24; j++) {
					Serial.print(tdata[j], HEX);
					Serial.print(" ");
				}
				Serial.println("");
			}
#endif			
		}
#ifdef DEBUG
		else {
			Serial.println("Timeout");
		}
#endif
	}
}
