//Arduino Mega and Leonardo chips only support some pins for receiving data back from the RoboClaw
//This is because only some pins of these boards support PCINT interrupts or are UART receivers.
//Mega: 0,10,11,12,13,14,15,17,19,50,51,52,53,A6,A7,A8,A9,A10,A11,A12,A13,A14,A15
//Leonardo: 0,8,9,10,11

//Arduino Due currently does not support SoftwareSerial. Only hardware uarts can be used, pins 0/1, 14/15, 16/17 or 18/19.

//Includes required to use Roboclaw library
#include "BMSerial.h"
#include "RoboClaw.h"

//Roboclaw Address
#define address 0x80

//Setup communcaitions with roboclaw. Use pins 10 and 11 with 10ms timeout
RoboClaw roboclaw(10,11,10000);

void setup() {
  //Communciate with roboclaw at 38400bps
  roboclaw.begin(38400);
}

void loop() {
  roboclaw.ForwardMixed(address, 64);
  delay(2000);
  roboclaw.BackwardMixed(address, 64);
  delay(2000);
  roboclaw.TurnRightMixed(address, 64);
  delay(2000);
  roboclaw.TurnLeftMixed(address, 64);
  delay(2000);
  roboclaw.ForwardBackwardMixed(address, 32);
  delay(2000);
  roboclaw.ForwardBackwardMixed(address, 96);
  delay(2000);
  roboclaw.LeftRightMixed(address, 32);
  delay(2000);
  roboclaw.LeftRightMixed(address, 96);
  delay(2000);
}
