#include "BMSerial.h"
#include "RoboClaw.h"

#define address 0x80

#define Kp 0x00010000
#define Ki 0x00008000
#define Kd 0x00004000
#define qpps 44000

//Arduino Mega and Leonardo chips only support some pins for receiving data back from the RoboClaw
//This is because only some pins of these boards support PCINT interrupts or are UART receivers.
//Mega: 0,10,11,12,13,14,15,17,19,50,51,52,53,A6,A7,A8,A9,A10,A11,A12,A13,A14,A15
//Leonardo: 0,8,9,10,11

//Arduino Due currently does not support SoftwareSerial. Only hardware uarts can be used, pins 0/1, 14/15, 16/17 or 18/19.

RoboClaw roboclaw(10,11);

void setup() {
  roboclaw.begin(38400);
  
  roboclaw.SetM1Constants(address,Kd,Kp,Ki,qpps);
  roboclaw.SetM2Constants(address,Kd,Kp,Ki,qpps);
}

void loop() {
}
