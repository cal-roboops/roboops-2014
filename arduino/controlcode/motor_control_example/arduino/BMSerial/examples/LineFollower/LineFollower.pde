#include "BMSerial.h"

//Arduino Mega and Leonardo chips only support some pins for receiving data.
//This is because only some pins of these boards support PCINT interrupts or are UART receivers.
//Mega: 0,10,11,12,13,14,15,17,19,50,51,52,53,A6,A7,A8,A9,A10,A11,A12,A13,A14,A15
//Leonardo: 0,8,9,10,11

//Arduino Due currently does not support SoftwareSerial.

BMSerial sensorSerial(6,6);

void setup() {
  Serial.begin(38400);
  sensorSerial.begin(38400);
}

void loop() {
  sensorSerial.println("#AA");
  Serial.print("RState:");
  Serial.print(sensorSerial.readhex(10000),HEX);
  Serial.print(" CState:");
  Serial.print(sensorSerial.readhex(10000),HEX);
  Serial.print(" LState:");
  Serial.println(sensorSerial.readhex(10000),HEX);
  
  delay(100);
}
