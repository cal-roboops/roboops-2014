//Arduino Mega and Leonardo chips only support some pins for receiving data back from the RoboClaw
//This is because only some pins of these boards support PCINT interrupts or are UART receivers.
//Mega: 0,10,11,12,13,14,15,17,19,50,51,52,53,A6,A7,A8,A9,A10,A11,A12,A13,A14,A15
//Leonardo: 0,8,9,10,11

//Arduino Due currently does not support SoftwareSerial. Only hardware uarts can be used, pins 0/1, 14/15, 16/17 or 18/19.


#include "BMSerial.h"
#include "RoboClaw.h"

#define address 0x80

//Velocity PID coefficients
#define Kp 2
#define Ki 4
#define Kd 0
#define Qpps 14400 //3600

//Define the pins to use and the timeout for serial communications to the Roboclaw
RoboClaw roboclaw(10,11,10000);

void setup() {
  //Communicate at 38400bps
  roboclaw.begin(38400);

  //Set Velocity PID coefficients
  roboclaw.SetM1VelocityPID(address,Kd,Kp,Ki,Qpps);
  roboclaw.SetM2VelocityPID(address,Kd,Kp,Ki,Qpps);
}

void loop() {
}
