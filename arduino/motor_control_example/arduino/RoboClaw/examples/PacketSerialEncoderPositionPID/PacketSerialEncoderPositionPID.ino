//Position PID example.  Velocity PID is not used. Speeds are not controlled by feedback.
//This example is the most like a standard RC servo

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
#define Kp 0
#define Ki 0
#define Kd 0
#define Qpps 3600

//Position PID coefficients
#define PosKp 2000
#define PosKi 4
#define PosKd 20000
#define KiMax 12.5
#define DeadZone 10
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

//This function will delay the program so the loop it
//executes in will run no more than every val milliseconds
int lasttime=0;
void rate(int val)
{
  while(millis()-lasttime<val);
  lasttime = millis();
}

void SetPosition(int pos)
{
  //Send position command. The Roboclaw library is using acknowledgement
  //When a valid command is received the roboclaw sends back a byte to ackowledge it.
  if(!roboclaw.SpeedAccelDeccelPositionM1(address,0,3600,0,pos,1))
    terminal.println("SpeedAccelDeccelPositionM1 Failed!");

  displayspeed();
}

//This is the first function arduino runs on reset/power up
void setup() {
  //Open terminal and roboclaw serial ports
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

//Main loop
void loop() {
  for(int pos = 175;pos<1900;pos+=10){
    rate(10);
    SetPosition(pos);
  }

  for(int pos = 1900;pos>175;pos-=10){
    rate(10);
    SetPosition(pos);
  }
}
