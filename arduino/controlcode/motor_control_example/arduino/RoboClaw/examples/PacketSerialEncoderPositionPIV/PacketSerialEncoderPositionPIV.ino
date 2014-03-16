//Position PIV example.  Velocity PID is used. Speeds are controlled by feedback.

//Warning!!!!
//Arduino Mega and Leonardo chips only support some pins for receiving data back from the RoboClaw
//This is because only some pins of these boards support PCINT interrupts or are UART receivers.
//Mega: 0,10,11,12,13,14,15,17,19,50,51,52,53,A6,A7,A8,A9,A10,A11,A12,A13,A14,A15
//Leonardo: 0,8,9,10,11

//Arduino Due currently does not support software serial. Only hardware uarts can be used, pins 0/1, 14/15, 16/17 or 18/19.

//Includes required to use Roboclaw library
#include "BMSerial.h"
#include "RoboClaw.h"

//Roboclaw Address
#define address 0x80

//Note: PID coeffcients will need to be tuned for this code to work with your motor.

//Velocity PID coefficients
#define Kp 2
#define Ki 4
#define Kd 0
#define Qpps 14400 //3600

//Position PID coefficients
#define PosKp 2500
#define PosKi 0
#define PosKd 20000
#define KiMax 0
#define DeadZone 5 //10
#define Min 50
#define Max 1950

//Definte terminal for display. Use hardware serial pins 0 and 1
BMSerial terminal(0,1);

//Setup communcaitions with roboclaw. Use pins 10 and 11 with 10ms timeout
RoboClaw roboclaw(10,11,10000);

//Display Encoder and Speed for Motor 1
void displayspeed(void)
{
  uint8_t status1,status2;
  bool valid1,valid2;
  int32_t enc1 = roboclaw.ReadEncM1(address, &status1, &valid1);
  int32_t speed1 = roboclaw.ReadSpeedM1(address, &status2, &valid2);
  
  if(valid1){
    terminal.print("Encoder1:");
    terminal.print(enc1,DEC);
    terminal.print(" ");
    terminal.print(status1,HEX);
    terminal.print(" ");
  }
  if(valid2){
    terminal.print("Speed1:");
    terminal.print(speed1,DEC);
    terminal.print(" ");
  }
  
  terminal.println();
}

void SetPosition(int accel,int speed,int deccel,int pos)
{
  //Send position command. The Roboclaw library is using acknowledgement
  //When a valid command is received the roboclaw sends back a byte to ackowledge it.
  if(!roboclaw.SpeedAccelDeccelPositionM1(address,accel,speed,deccel,pos,1))
    terminal.println("SpeedAccelDeccelPositionM1 Failed!");
}

//This is the first function arduino runs on reset/power up
void setup() {
  //Open terminal and roboclaw at 38400bps
  terminal.begin(57600);
  roboclaw.begin(38400);
  
  terminal.println("Starting...");
  
  //Use Absolute Encoder and Enable encoders in RC mode
  roboclaw.SetM1EncoderMode(address,0x81);

  //Set PID Coefficients
  roboclaw.SetM1VelocityPID(address,Kd,Kp,Ki,Qpps);
  roboclaw.SetM1PositionPID(address,PosKd,PosKp,PosKi,KiMax,DeadZone,Min,Max);

  //Save settings in Roboclaw eeprom.  Uncomment to save settings.
  //roboclaw.WriteNVM(address);
}

void loop() {
  SetPosition(32000,1000,32000,1700);
  uint32_t last = millis();
  while(millis()-last<5000){
    displayspeed();
    delay(50);
  }

  SetPosition(32000,1000,32000,300);
  last = millis();
  while(millis()-last<5000){
    displayspeed();
    delay(50);
  }
}
