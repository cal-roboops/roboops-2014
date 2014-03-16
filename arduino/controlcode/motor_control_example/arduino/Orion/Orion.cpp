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

//
// Includes
// 
#include "Orion.h"

extern SPIClass SPI;
OrionClass Orion;

//
// Constructor
//
OrionClass::OrionClass()
{
	GaitType = RG6;
	GaitStep = 0;
	SubStep = 0;
}

//
// Destructor
//
OrionClass::~OrionClass()
{
}

//
// Public methods
//

void OrionClass::begin()
{
	lipolow = false;
	lipoverylow = false;
	lipocritical = false;
	
	lastalarm = millis();
	lastalarmcheck = millis();

	// speaker pin
	pinMode(3, OUTPUT);
	
	// set the slaveSelectPin as an output:
	pinMode (10, OUTPUT);

	// initialize SPI:
	SPI.begin(); 
	SPI.setBitOrder(LSBFIRST);
	SPI.setClockDivider(SPI_CLOCK_DIV8);
	SPI.setDataMode(SPI_MODE3);

	delay(250);

	while(queryVersion()!=_ORION_VERSION){
		Serial.print("Orion Version not supported:");
		Serial.println(queryVersion(),HEX);
		tone(NOTE_A5,250);
		delay(250);
		tone(NOTE_A4,250);
		delay(250);
	}	
	
	for(int i=0;i<6;i++){
		LegRaised[i]=false;
		enableIKForce(i,1,0);
	}
}

void OrionClass::end()
{
}

void OrionClass::setPOffset(unsigned char pin, int offset)
{
	WritePacket(cmdPOffset,pin,offset);
}

void OrionClass::setAOffset(unsigned char pin, int offset)
{
	WritePacket(cmdAOffset,pin,offset);
}

void OrionClass::setPulse(unsigned char pin, int pulse)
{
	WritePacket(cmdPulse,pin,pulse);
}

void OrionClass::setAngle(unsigned char pin, int angle)
{
	WritePacket(cmdAngle,pin,angle);
}

void OrionClass::setSpeed(unsigned int time)
{
	WritePacket(cmdSpeed,time);
}

void OrionClass::setTime(unsigned int time)
{
	WritePacket(cmdTime,time);
}

void OrionClass::setPinHigh(unsigned char pin)
{
	WritePacket(cmdPinHigh,pin);
}

void OrionClass::setPinLow(unsigned char pin)
{
	WritePacket(cmdPinLow,pin);
}

void OrionClass::setPort(unsigned char port,unsigned char bits)
{
	WritePacket(cmdSetPort,port,bits);
}

unsigned int OrionClass::queryMove()
{
	return ReadPacketB(cmdQueryMove);
}

int OrionClass::queryPOffset(unsigned char pin)
{
	return ReadPacketW(cmdQueryPOffset,pin);
}

int OrionClass::queryAOffset(unsigned char pin)
{
	return ReadPacketW(cmdQueryAOffset,pin);
}

int OrionClass::queryPulse(unsigned char pin)
{
	return ReadPacketW(cmdQueryPulse,pin);
}

int OrionClass::queryAngle(unsigned char pin)
{
	return ReadPacketW(cmdQueryAngle,pin);
}

int OrionClass::queryFBPulse(unsigned char pin)
{
	return ReadPacketW(cmdQueryFBPulse,pin);
}

int OrionClass::queryFBAngle(unsigned char pin)
{
	return ReadPacketW(cmdQueryFBAngle,pin);
}

int OrionClass::queryFBError(unsigned char pin)
{
	return ReadPacketW(cmdQueryFBError,pin);
}

unsigned int OrionClass::queryVersion()
{
	return ReadPacketW(cmdQueryVersion);
}

unsigned int OrionClass::queryVoltage()
{
	return ReadPacketW(cmdQueryVoltage);
}

unsigned int OrionClass::pinState(unsigned char pin)
{
	return ReadPacketB(cmdPinState,pin);
}

unsigned int OrionClass::pinStateL(unsigned char pin)
{
	return ReadPacketB(cmdPinStateL,pin);
}

void OrionClass::resetRegs()
{
	WritePacket(cmdResetRegs);
}

unsigned int OrionClass::getReg(unsigned char reg)
{
	return ReadPacketW(cmdGetReg,reg);
}

void OrionClass::setReg(unsigned char reg,unsigned int data)
{
	WritePacket(cmdSetReg,reg,data);
}

void OrionClass::stopMove(unsigned char group)
{
	WritePacket(cmdStopMove,group);
}

void OrionClass::stopPulse(unsigned char pin)
{
	WritePacket(cmdStopPulse,pin);
}

void OrionClass::group(unsigned char group)
{
	WritePacket(cmdGroup,group);
}

void OrionClass::calcMove()
{
	WritePacket(cmdCalcMove);
}

void OrionClass::setXPos(int pos)
{
	WritePacket(cmdSetXPos,pos);
}

void OrionClass::setYPos(int pos)
{
	WritePacket(cmdSetYPos,pos);
}

void OrionClass::setZPos(int pos)
{
	WritePacket(cmdSetZPos,pos);
}

void OrionClass::setWPos(int pos)
{
	WritePacket(cmdSetWPos,pos);
}

void OrionClass::setXYZPos(unsigned char index,int x,int y,int z)
{
	WritePacket(cmdSet1XYZPos+index,x,y,z);
}

void OrionClass::writeRegisters()
{
	WritePacket(cmdWriteRegisters);
}

int OrionClass::getXPos(unsigned char index)
{
	return ReadPacketW(cmdGetXPos,index);
}

int OrionClass::getYPos(unsigned char index)
{
	return ReadPacketW(cmdGetYPos,index);
}

int OrionClass::getZPos(unsigned char index)
{
	return ReadPacketW(cmdGetZPos,index);
}

int OrionClass::getWPos(unsigned char index)
{
	return ReadPacketW(cmdGetWPos,index);
}

int OrionClass::queryForce(unsigned char index)
{
	return ReadPacketW(cmdQueryForce,index,5000);
}

void OrionClass::execute()
{
	WritePacket(cmdExecute);
}

void OrionClass::setForce(unsigned char index,int force)
{
	WritePacket(cmdSetForce,index,force);
}

void OrionClass::enableForce(unsigned char index,unsigned char enable)
{
	WritePacket(cmdEnableForce,index,enable);
}

void OrionClass::enableIKForce(unsigned char leg,unsigned char joint,unsigned char enable)
{
	WritePacket(cmdEnableIKForce,(leg*4)+joint,enable);
}

unsigned char OrionClass::queryForceTrig(unsigned char index)
{
	return ReadPacketB(cmdQueryForceTrig,index);
}

unsigned char OrionClass::queryIKForceTrig(unsigned char leg,unsigned char joint)
{
	return ReadPacketB(cmdQueryIKForceTrig,(unsigned char)((leg*4)+joint));
}

void OrionClass::setServoDir(unsigned char index,unsigned char dir)
{
	WritePacket(cmdSetServoDir,index,dir);
}

void OrionClass::setServoIKDir(unsigned char leg,unsigned char dir)
{
	WritePacket(cmdSetServoIKDir,leg,dir);
}

void OrionClass::setServoIKPin(unsigned char leg,unsigned char joint,unsigned char pin)
{
	WritePacket(cmdSetServoIKPin,(leg*4)+joint,pin);
}

void OrionClass::setServoIKLength(unsigned char leg,unsigned char joint,unsigned int length)
{
	WritePacket(cmdSetServoIKLength,(leg*4)+joint,length);
}

void OrionClass::setServoMin(unsigned char index,int min)
{
	WritePacket(cmdSetServoMin,index,min);
}

void OrionClass::setServoMax(unsigned char index,int max)
{
	WritePacket(cmdSetServoMax,index,max);
}

void OrionClass::setServoDegree(unsigned char index,unsigned int degree)
{
	WritePacket(cmdSetServoDegree,index,degree);
}

int OrionClass::queryServoMin(unsigned char pin)
{
	return ReadPacketW(cmdQueryServoMin,pin);
}

int OrionClass::queryServoMax(unsigned char pin)
{
	return ReadPacketW(cmdQueryServoMax,pin);
}

void OrionClass::enableIKMinMax(boolean state)
{
	WritePacket(cmdEnableIKMinMax,state);
}

boolean OrionClass::checkLipo()
{
  long time = millis() - lastalarm;
  if(lipocritical>=10){
    //Lipo protection
    this->tone(NOTE_A5,250);
    this->tone(NOTE_G4,250);
    return true;
  }
  else if(lipoverylow>=10){
    //Lipo protection
    if(time>1000){
      lastalarm=millis();
      this->tone(NOTE_A5,100,false);
    }
  }
  else if(lipolow>=10){
    //Lipo protection
    if(time>3000){
      lastalarm=millis();
      this->tone(NOTE_A5,100,false);
    }
  }

  if((millis()-lastalarmcheck)>100){
    lastalarmcheck=millis();
    int voltage = queryVoltage();
    if(voltage<700){
      lipolow++;
      if(lipolow>10)
      	lipolow=10;
    }
    else
      lipolow=0;
    if(voltage<650){
      lipoverylow++;
      if(lipoverylow>10)
      	lipoverylow=10;
    }
    else
      lipoverylow=0;
    if(voltage<620)			//Orion will auto cutoff servo use at this level but there is still approx 100ma draw from all electronics
      lipocritical++;		//once triggered always triggered
    else
    	lipocritical=0;
  }
  
  return false;
}

void OrionClass::gaitSelect(GAITTYPE sel)
{
	GaitType = sel;
}

void OrionClass::UpdateGaitSelect()
{
	//Only update gait selection at the end of a cycle
	switch(GaitType){
	case RG6:
   	GaitLeg[cLF] = 4;	  
   	GaitLeg[cLM] = 2;	  
   	GaitLeg[cLR] = 0;
   	GaitLeg[cRF] = 1;	
   	GaitLeg[cRM] = 5;
   	GaitLeg[cRR] = 3;	  
   	  	    
		LegDroop = 0;
   	LiftSteps = 1;
   	LowerSteps = 1;
		UpSteps = 2;
		DownSteps = 4;
   	StepsInGait = 6;
		break;
	case RG12:
   	GaitLeg[cLF] = 8;
   	GaitLeg[cLM] = 4;
   	GaitLeg[cLR] = 0;
   	GaitLeg[cRF] = 2;
   	GaitLeg[cRM] = 10;
   	GaitLeg[cRR] = 6;

		LegDroop = 0;
   	LiftSteps = 1;
   	LowerSteps = 1;
		UpSteps = 4;
		DownSteps = 8;
   	StepsInGait = 12;	
  		break;
	case QR9:
   	GaitLeg[cLF] = 6;
   	GaitLeg[cLM] = 3;	  
   	GaitLeg[cLR] = 0;   
   	GaitLeg[cRF] = 1;
   	GaitLeg[cRM] = 7;
     	GaitLeg[cRR] = 4;
   	  
		LegDroop = 0;
   	LiftSteps = 1;
   	LowerSteps = 1;
		UpSteps = 3;
		DownSteps = 6;
   	StepsInGait = 9;	    
      break;
	case TRI4: 
   	GaitLeg[cLF] = 2;
   	GaitLeg[cLM] = 0;
   	GaitLeg[cLR] = 2;
   	GaitLeg[cRF] = 0;
   	GaitLeg[cRM] = 2;
   	GaitLeg[cRR] = 0;
   	  
		LegDroop = 40;
   	LiftSteps = 1;
   	LowerSteps = 1;
		UpSteps = 2;
		DownSteps = 2;
   	StepsInGait = 4;   
   	break;
	case TRI6:    
   	GaitLeg[cLF] = 3;
   	GaitLeg[cLM] = 0;
   	GaitLeg[cLR] = 3;   
   	GaitLeg[cRF] = 0;
   	GaitLeg[cRM] = 3;
   	GaitLeg[cRR] = 0;
 
		LegDroop = 40;
   	LiftSteps = 1;
   	LowerSteps = 1;
		UpSteps = 2;
		DownSteps = 4;
   	StepsInGait = 6;   
   	break;
	case TRI8:
   	GaitLeg[cLF] = 4;
   	GaitLeg[cLM] = 0;
   	GaitLeg[cLR] = 4;
   	GaitLeg[cRF] = 0;
   	GaitLeg[cRM] = 4;
   	GaitLeg[cRR] = 0;
   	  
		LegDroop = 20;
   	LiftSteps = 1;
   	LowerSteps = 1;
		UpSteps = 4;
		DownSteps = 4;
   	StepsInGait = 8;    
   	break;
	case TRIO12: 
   	GaitLeg[cLF] = 6;
   	GaitLeg[cLM] = 1;
   	GaitLeg[cLR] = 8;  
   	GaitLeg[cRF] = 0;
   	GaitLeg[cRM] = 7;
   	GaitLeg[cRR] = 2;
   	  
		LegDroop = 0;
   	LiftSteps = 1;
   	LowerSteps = 1;
		UpSteps = 4;
		DownSteps = 8;
   	StepsInGait = 12;   
   	break;
	case TRIO18: 
   	GaitLeg[cLF] = 9;
   	GaitLeg[cLM] = 1;
   	GaitLeg[cLR] = 11;  
   	GaitLeg[cRF] = 0;
   	GaitLeg[cRM] = 10;
   	GaitLeg[cRR] = 2;
   	  
		LegDroop = 0;
   	LiftSteps = 1;
   	LowerSteps = 1;
		UpSteps = 7;
		DownSteps = 11;
   	StepsInGait = 18;   
   	break;
	case WAVE12:
   	GaitLeg[cLF] = 4;
   	GaitLeg[cLM] = 2;
   	GaitLeg[cLR] = 0;
   	GaitLeg[cRF] = 10;
   	GaitLeg[cRM] = 8;
   	GaitLeg[cRR] = 6;
   	  
		LegDroop = 0;
   	LiftSteps = 1;
   	LowerSteps = 1;
		UpSteps = 2;
		DownSteps = 10;
   	StepsInGait = 12;	    
		break;
	case WAVE18:  
   	GaitLeg[cLF] = 9;
   	GaitLeg[cLM] = 6;
   	GaitLeg[cLR] = 3;
   	GaitLeg[cRF] = 0;
   	GaitLeg[cRM] = 15;
   	GaitLeg[cRR] = 12;
   	  
		LegDroop = 0;
   	LiftSteps = 1;
   	LowerSteps = 1;
		UpSteps = 3;
		DownSteps = 15;
   	StepsInGait = 18;	    
		break;
	}
}

void OrionClass::execGait(int BodyYaw,int BodyRoll,int BodyPitch,
				              int BodyRotOffsetX,int BodyRotOffsetY,int BodyRotOffsetZ,
				              int BodyOffsetX,int BodyOffsetY,int BodyOffsetZ,
            				  int Xdist,int Ydist,int Zdist,int Rdist,
            				  int XLegAdj, int ZLegAdj,
            				  int Rate,
            				  boolean EnableForce)
{
//	Xdist=-Xdist;
//	Zdist=-Zdist;
//	Rdist=-Rdist;
		
	if(SubStep==0){
		if(GaitStep==0)
			UpdateGaitSelect();

   	curXdown=-Xdist;
   	curZdown=-Zdist;
   	curRdown=-Rdist;
		for(int i=0;i<6;i++){
	   	int legstep;
		   legstep = GaitStep - GaitLeg[i];
  			if(legstep<0)
	  			legstep+=StepsInGait;
		   if(legstep>=UpSteps){
			  	int totalsteps = StepsInGait*Rate;
		  		int step = legstep*Rate;
				//downsteps
   			int Xdown=(-Xdist-GaitPosX[i])/(totalsteps-step);
   			int Zdown=(-Zdist-GaitPosZ[i])/(totalsteps-step);
   			int Rdown=(-Rdist-GaitPosR[i])/(totalsteps-step);
   			if(Xdist<0){
   				if(Xdown<curXdown)
   					curXdown=Xdown;
   			}
   			if(Xdist>0){
   				if(Xdown>curXdown)
   					curXdown=Xdown;
   			}
   			if(Zdist<0){
   				if(Zdown<curZdown)
   					curZdown=Zdown;
   			}
   			if(Zdist>0){
   				if(Zdown>curZdown)
   					curZdown=Zdown;
   			}
   			if(Rdist<0){
   				if(Rdown<curRdown)
   					curRdown=Rdown;
   			}
   			if(Rdist>0){
   				if(Rdown>curRdown)
   					curRdown=Rdown;
   			}
   		}
		}
	}

	//reset values used for calculation of balance
	BalanceTransX = 0;
	BalanceTransZ = 0;
	BalanceTransY = 0;
	BalancePitch = 0;
	BalanceRoll = 0;

	boolean active=false;
	for(int i=0;i<6;i++){
		//Calculate next leg position for Gait selected
 		if(CalcLegStep(i,Xdist,Ydist,Zdist,Rdist,curXdown,curZdown,curRdown,Rate,EnableForce))
			active=true;
 
		if(BalanceMode&0x01){
			BalanceTransX+=GaitPosX[i];
			BalanceTransY+=GaitPosY[i];
			BalanceTransZ+=GaitPosZ[i];
 		}
		if(BalanceMode&0x02){
			int pitch=0,roll=0;
			if(i<3)
				roll = atan2Int(GaitPosY[i],abs(LegPosX[i]+BodyPosX[i]));
			else
				roll = -atan2Int(GaitPosY[i],abs(LegPosX[i]+BodyPosX[i]));

			if(LegPosZ[i]+BodyPosZ[i]>=100)
				pitch = -atan2Int(GaitPosY[i],abs(LegPosZ[i]+BodyPosZ[i]));
			if(LegPosZ[i]+BodyPosZ[i]<=-100)
				pitch = atan2Int(GaitPosY[i],abs(LegPosZ[i]+BodyPosZ[i]));

			BalancePitch+=pitch;
			BalanceRoll+=roll;
		}
	}
	BalanceTransX=BalanceTransX*BalanceTransRate/600;
	BalanceTransY=BalanceTransY*BalanceTransRate/600;
	BalanceTransZ=BalanceTransZ*BalanceTransRate/600;
	BalancePitch=BalancePitch*BalanceAngleRate/600;
	BalanceRoll=BalanceRoll*BalanceAngleRate/600;
	
	for(int i=0;i<6;i++){
	   //precalculation for angles
	   int gaitposr = GaitPosR[i];
	   long SinA = sinInt(BodyYaw+gaitposr)>>1;
	   long CosA = cosInt(BodyYaw+gaitposr)>>1;
	   long SinB = sinInt(BodyRoll+BalanceRoll)>>1;
	   long CosB = cosInt(BodyRoll+BalanceRoll)>>1;
	   long SinG = sinInt(BodyPitch+BalancePitch)>>1;
	   long CosG = cosInt(BodyPitch+BalancePitch)>>1;

		//Calcualtion of body rotation matrix
		long X = LegPosX[i]+BodyRotOffsetX;
		long Y = LegPosY[i]+BodyRotOffsetY;
		long Z = LegPosZ[i]+BodyRotOffsetZ;

		LastXPos[i] = (( (CosA*CosB>>14)*X - (SinA*CosB>>14)*Z + SinB*Y )>>14) - (BodyPosX[i]+BodyRotOffsetX) + BodyOffsetX + GaitPosZ[i] - BalanceTransX;
		LastYPos[i] = (( (SinA*SinG>>14)*X - ((CosA*SinB>>14)*CosG>>14)*X + (CosA*SinG>>14)*Z + ((SinA*SinB>>14)*CosG>>14)*Z + (CosB*CosG>>14)*Y )>>14) - (BodyPosY[i]+BodyRotOffsetY) + BodyOffsetY + GaitPosY[i] - BalanceTransY;
		LastZPos[i] = (( (SinA*CosG>>14)*X + ((CosA*SinB>>14)*SinG>>14)*X + (CosA*CosG>>14)*Z - ((SinA*SinB>>14)*SinG>>14)*Z - (CosB*SinG>>14)*Y )>>14) - (BodyPosZ[i]+BodyRotOffsetZ) + BodyOffsetZ + GaitPosX[i] - BalanceTransZ;
		if(i<3)
			setXYZPos(i,LastXPos[i],LastYPos[i],LastZPos[i]);
		else     
			setXYZPos(i,-LastXPos[i],LastYPos[i],LastZPos[i]);
	}

	//Execute movement
	execute();

	//Update Step
	if(active || GaitStep || SubStep){
		SubStep++;
		if(SubStep>=Rate){
			SubStep=0;
			if(!active){
				GaitStep=0;
			}
			else{
				GaitStep++;
				if(GaitStep==StepsInGait){
					GaitStep=0;
				}
			}
		}
	}	
}

boolean OrionClass::CalcLegStep(unsigned char LegIndex,int Xdist,int Ydist,int Zdist,int Rdist,int Xdown,int Zdown, int Rdown,int Rate,boolean EnableForce)
{
   Xdist/=2;
   Ydist=-Ydist;
   Zdist/=2;
   Rdist/=2;
   	
   boolean active = Xdist || Zdist || Rdist;
   boolean moving = abs(GaitPosX[LegIndex])>DEADZONE || abs(GaitPosZ[LegIndex])>DEADZONE ||  abs(GaitPosR[LegIndex])>DEADZONE || LegRaised[LegIndex];

   int legstep;
   legstep = GaitStep - GaitLeg[LegIndex];
  	if(legstep<0)
  		legstep+=StepsInGait;
  	int step = (legstep*Rate)+SubStep;

   if(legstep<UpSteps){
   	//upsteps
	  	int totalsteps = UpSteps*Rate;
		
  		if(step==((UpSteps-LowerSteps)*Rate) && EnableForce){
  			enableIKForce(LegIndex,1,1);	//enable force detect and clear force flags
  		}
		if(legstep>=(UpSteps-LowerSteps)){
 			if(!Orion.queryIKForceTrig(LegIndex,1)){
				//lower leg
		   	GaitPosX[LegIndex]+=(Xdist-GaitPosX[LegIndex])/(totalsteps-step);
   			GaitPosZ[LegIndex]+=(Zdist-GaitPosZ[LegIndex])/(totalsteps-step);
				GaitPosR[LegIndex]+=(Rdist-GaitPosR[LegIndex])/(totalsteps-step);
				GaitPosY[LegIndex]+=(-GaitPosY[LegIndex])/(totalsteps-step);
			}
		}
		else{
			//raise/hold leg
			if(active || moving){
				LegRaised[LegIndex]=true;
		   	GaitPosX[LegIndex]+=(Xdist-GaitPosX[LegIndex])/(totalsteps-step);
				GaitPosZ[LegIndex]+=(Zdist-GaitPosZ[LegIndex])/(totalsteps-step);
				GaitPosR[LegIndex]+=(Rdist-GaitPosR[LegIndex])/(totalsteps-step);
				if(legstep<LiftSteps){
					GaitPosY[LegIndex]+=(Ydist-GaitPosY[LegIndex])/((LiftSteps*Rate)-step);
				}
			}
		}
	}
   else{
		LegRaised[LegIndex]=false;
   	if(active || moving){
			//downsteps
   		GaitPosX[LegIndex]+=Xdown;
   		GaitPosZ[LegIndex]+=Zdown;
   		GaitPosR[LegIndex]+=Rdown;
   	}

		if(!Orion.queryIKForceTrig(LegIndex,1)){
			if(active && step>=(UpSteps*Rate) && step<((UpSteps*Rate)+(Rate/2)+1)){
				//lower leg
				GaitPosY[LegIndex]=(long)LegDroop * (SubStep+1) / ((Rate/2)+1);
	   	}
   		else
			if(active && step>=((StepsInGait*Rate)-((Rate/2)+1)) && step<(StepsInGait*Rate)){
				//raise leg
				GaitPosY[LegIndex]=LegDroop-((long)LegDroop * (SubStep+1-(Rate/2)) / ((Rate/2)+1));
   		}
   		else{
	   		GaitPosY[LegIndex]=LegDroop;
		   }
		}
   }
   
   return active || moving;
}
