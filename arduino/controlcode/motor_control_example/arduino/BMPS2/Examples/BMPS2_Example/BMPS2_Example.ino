//Requires BMPS2 and BMSerial libraries at http://downloads.orionrobotics.com/downloads/code/arduino.zip
#include <BMSerial.h>
#include <BMPS2.h>

BMPS2 ps2x(6,100);	//Use Arduino Pin 6 with 100ms maximum refresh rate

void setup(){
	Serial.begin(57600);
	
	//Clears pressed/released states
	ps2x.newButtonState();
	delay(100);
	ps2x.newButtonState();
}

void loop(){
	if(ps2x.isUpdated()){ //This will be true every 100ms
		int RX = ps2x.analog(PSS_RX);
		int RY = ps2x.analog(PSS_RY);
		int LX = ps2x.analog(PSS_LX);
		int LY = ps2x.analog(PSS_LY);
		int DX = ps2x.analog(PSAB_PAD_LEFTRIGHT);
		int DY = ps2x.analog(PSAB_PAD_UPDOWN);
		
		if(ps2x.buttonPressed(PSB_SELECT)){
		  Serial.println("Select");
		}
		
		if(ps2x.buttonPressed(PSB_START)){
		  Serial.println("Start");
		}
		
		if(ps2x.buttonPressed(PSB_GREEN)){
		  Serial.println("Green");
		}
		
		if(ps2x.buttonPressed(PSB_BLUE)){
		  Serial.println("Blue");
		}
		
		if(ps2x.buttonPressed(PSB_PINK)){
		  Serial.println("Pink");
		}
		
		if(ps2x.buttonPressed(PSB_RED)){
		  Serial.println("Red");
		}
		
		if(ps2x.buttonPressed(PSB_PAD_UP)){
		  Serial.println("UP");
		}
		
		if(ps2x.buttonPressed(PSB_PAD_DOWN)){
		  Serial.println("DOWN");
		}
		
		if(ps2x.buttonPressed(PSB_PAD_LEFT)){
		  Serial.println("LEFT");
		}
		
		if(ps2x.buttonPressed(PSB_PAD_RIGHT)){
		  Serial.println("RIGHT");
		}
		
		if(ps2x.buttonPressed(PSB_L1)){
		  Serial.println("L1");
		}
		
		if(ps2x.buttonPressed(PSB_L2)){
		  Serial.println("L2");
		}
		
		if(ps2x.buttonPressed(PSB_R1)){
		  Serial.println("R1");
		}
		
		if(ps2x.buttonPressed(PSB_R2)){
		  Serial.println("R2");
		}
		
		if(ps2x.buttonPressed(PSB_L3)){
		  Serial.println("L3");
		}
		
		if(ps2x.buttonPressed(PSB_R3)){
		  Serial.println("R3");
		}
		
		Serial.print(RX);
		Serial.print(" ");
		Serial.print(RY);
		Serial.print(" ");
		Serial.print(LX);
		Serial.print(" ");
		Serial.print(LY);
		Serial.print(" ");
		Serial.print(DX);
		Serial.print(" ");
		Serial.print(DY);
		Serial.println();	
	}
}
