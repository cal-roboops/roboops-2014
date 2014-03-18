#include "BMSerial.h"
#include "RoboClaw.h"

#define address 0x80

//Arduino Mega and Leonardo chips only support some pins for receiving data back from the RoboClaw
//This is because only some pins of these boards support PCINT interrupts or are UART receivers.
//Mega: 0,10,11,12,13,14,15,17,19,50,51,52,53,A6,A7,A8,A9,A10,A11,A12,A13,A14,A15
//Leonardo: 0,8,9,10,11

//Arduino Due currently does not support SoftwareSerial. Only hardware uarts can be used, pins 0/1, 14/15, 16/17 or 18/19.

/*Roboclaw Arduino library function definitions
	bool ForwardM1(uint8_t address, uint8_t speed);
	bool BackwardM1(uint8_t address, uint8_t speed);
	bool SetMinVoltageMainBattery(uint8_t address, uint8_t voltage);
	bool SetMaxVoltageMainBattery(uint8_t address, uint8_t voltage);
	bool ForwardM2(uint8_t address, uint8_t speed);
	bool BackwardM2(uint8_t address, uint8_t speed);
	bool ForwardBackwardM1(uint8_t address, uint8_t speed);
	bool ForwardBackwardM2(uint8_t address, uint8_t speed);
	bool ForwardMixed(uint8_t address, uint8_t speed);
	bool BackwardMixed(uint8_t address, uint8_t speed);
	bool TurnRightMixed(uint8_t address, uint8_t speed);
	bool TurnLeftMixed(uint8_t address, uint8_t speed);
	bool ForwardBackwardMixed(uint8_t address, uint8_t speed);
	bool LeftRightMixed(uint8_t address, uint8_t speed);
	uint32_t ReadEncM1(uint8_t address, uint8_t *status=NULL,bool *valid=NULL);
	uint32_t ReadEncM2(uint8_t address, uint8_t *status=NULL,bool *valid=NULL);
	uint32_t ReadSpeedM1(uint8_t address, uint8_t *status=NULL,bool *valid=NULL);
	uint32_t ReadSpeedM2(uint8_t address, uint8_t *status=NULL,bool *valid=NULL);
	bool ResetEncoders(uint8_t address);
	bool ReadVersion(uint8_t address,char *version);
	uint16_t ReadMainBatteryVoltage(uint8_t address,bool *valid=NULL);
	uint16_t ReadLogicBattVoltage(uint8_t address,bool *valid=NULL);
	bool SetMinVoltageLogicBattery(uint8_t address, uint8_t voltage);
	bool SetMaxVoltageLogicBattery(uint8_t address, uint8_t voltage);
	bool SetM1VelocityPID(uint8_t address, float Kd, float Kp, float Ki, uint32_t qpps);
	bool SetM2VelocityPID(uint8_t address, float Kd, float Kp, float Ki, uint32_t qpps);
	uint32_t ReadISpeedM1(uint8_t address,uint8_t *status=NULL,bool *valid=NULL);
	uint32_t ReadISpeedM2(uint8_t address,uint8_t *status=NULL,bool *valid=NULL);
	bool DutyM1(uint8_t address, uint16_t duty);
	bool DutyM2(uint8_t address, uint16_t duty);
	bool DutyM1M2(uint8_t address, uint16_t duty1, uint16_t duty2);
	bool SpeedM1(uint8_t address, uint32_t speed);
	bool SpeedM2(uint8_t address, uint32_t speed);
	bool SpeedM1M2(uint8_t address, uint32_t speed1, uint32_t speed2);
	bool SpeedAccelM1(uint8_t address, uint32_t accel, uint32_t speed);
	bool SpeedAccelM2(uint8_t address, uint32_t accel, uint32_t speed);
	bool SpeedAccelM1M2(uint8_t address, uint32_t accel, uint32_t speed1, uint32_t speed2);
	bool SpeedDistanceM1(uint8_t address, uint32_t speed, uint32_t distance, uint8_t flag=0);
	bool SpeedDistanceM2(uint8_t address, uint32_t speed, uint32_t distance, uint8_t flag=0);
	bool SpeedDistanceM1M2(uint8_t address, uint32_t speed1, uint32_t distance1, uint32_t speed2, uint32_t distance2, uint8_t flag=0);
	bool SpeedAccelDistanceM1(uint8_t address, uint32_t accel, uint32_t speed, uint32_t distance, uint8_t flag=0);
	bool SpeedAccelDistanceM2(uint8_t address, uint32_t accel, uint32_t speed, uint32_t distance, uint8_t flag=0);
	bool SpeedAccelDistanceM1M2(uint8_t address, uint32_t accel, uint32_t speed1, uint32_t distance1, uint32_t speed2, uint32_t distance2, uint8_t flag=0);
	bool ReadBuffers(uint8_t address, uint8_t &depth1, uint8_t &depth2);
	bool ReadCurrents(uint8_t address, uint8_t &current1, uint8_t &current2);
	bool SpeedAccelM1M2_2(uint8_t address, uint32_t accel1, uint32_t speed1, uint32_t accel2, uint32_t speed2);
	bool SpeedAccelDistanceM1M2_2(uint8_t address, uint32_t accel1, uint32_t speed1, uint32_t distance1, uint32_t accel2, uint32_t speed2, uint32_t distance2, uint8_t flag=0);
	bool DutyAccelM1(uint8_t address, uint16_t duty, uint16_t accel);
	bool DutyAccelM2(uint8_t address, uint16_t duty, uint16_t accel);
	bool DutyAccelM1M2(uint8_t address, uint16_t duty1, uint16_t accel1, uint16_t duty2, uint16_t accel2);
	bool ReadM1VelocityPID(uint8_t address,float &Kp_fp,float &Ki_fp,float &Kd_fp,uint32_t &qpps);
	bool ReadM2VelocityPID(uint8_t address,float &Kp_fp,float &Ki_fp,float &Kd_fp,uint32_t &qpps);
	bool SetMainVoltages(uint8_t address,uint16_t min,uint16_t max);
	bool SetLogicVoltages(uint8_t address,uint16_t min,uint16_t max);
	bool ReadMinMaxMainVoltages(uint8_t address,uint16_t &min,uint16_t &max);
	bool ReadMinMaxLogicVoltages(uint8_t address,uint16_t &min,uint16_t &max);
	bool SetM1PositionPID(uint8_t address,float kd,float kp,float ki,float kiMax,uint32_t deadzone,uint32_t min,uint32_t max);
	bool SetM2PositionPID(uint8_t address,float kd,float kp,float ki,float kiMax,uint32_t deadzone,uint32_t min,uint32_t max);
	bool ReadM1PositionPID(uint8_t address,float &Kp,float &Ki,float &Kd,float &KiMax,uint32_t &DeadZone,uint32_t &Min,uint32_t &Max);
	bool ReadM2PositionPID(uint8_t address,float &Kp,float &Ki,float &Kd,float &KiMax,uint32_t &DeadZone,uint32_t &Min,uint32_t &Max);
	bool SpeedAccelDeccelPositionM1(uint8_t address,uint32_t accel,uint32_t speed,uint32_t deccel,uint32_t position,uint8_t flag);
	bool SpeedAccelDeccelPositionM2(uint8_t address,uint32_t accel,uint32_t speed,uint32_t deccel,uint32_t position,uint8_t flag);
	bool SpeedAccelDeccelPositionM1M2(uint8_t address,uint32_t accel1,uint32_t speed1,uint32_t deccel1,uint32_t position1,uint32_t accel2,uint32_t speed2,uint32_t deccel2,uint32_t position2,uint8_t flag);
	bool ReadTemp(uint8_t address, uint16_t &temp);
	uint8_t ReadError(uint8_t address,bool *valid=NULL);
	bool ReadEncoderModes(uint8_t address, uint8_t &M1mode, uint8_t &M2mode);
	bool SetM1EncoderMode(uint8_t address,uint8_t mode);
	bool SetM2EncoderMode(uint8_t address,uint8_t mode);
	bool WriteNVM(uint8_t address);
*/


RoboClaw roboclaw(10,11);

void setup() {
  roboclaw.begin(38400);

  // put your setup code here, to run once:
}

void loop() {

  // put your main code here, to run repeatedly: 
 
}
