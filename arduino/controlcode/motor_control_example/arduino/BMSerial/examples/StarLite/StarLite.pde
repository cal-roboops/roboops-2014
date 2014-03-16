#include "BMSerial.h"

//Arduino Mega and Leonardo chips only support some pins for receiving data back.
//This is because only some pins of these boards support PCINT interrupts or are UART receivers.
//Mega: 0,10,11,12,13,14,15,17,19,50,51,52,53,A6,A7,A8,A9,A10,A11,A12,A13,A14,A15
//Leonardo: 0,8,9,10,11

//Arduino Due does not currently support SoftwareSerial.

BMSerial ledSerial(13,13);

void setup() {
  Serial.begin(38400);
  ledSerial.begin(38400);
  ledSerial.println("#AFF01");    //white
}

void loop() {
   //quick color changes. No fade
  ledSerial.println("#AFF01");    //white
  delay(1000);
  ledSerial.println("#AFC01");    //cyan
  delay(1000);
  ledSerial.println("#AE301");    //magenta
  delay(1000);
  ledSerial.println("#AE001");    //blue
  delay(1000);
  ledSerial.println("#A1F01");    //yellow
  delay(1000);
  ledSerial.println("#A1C01");    //green
  delay(1000);
  ledSerial.println("#A0301");    //red
  delay(1000);
  ledSerial.println("#A0001");    //black
  delay(1000);

  //multi multi color with fade
  //colors will cycle until new colors are loaded
  ledSerial.println("#AFF40FC40E3401F401C4003400040");
  delay(20000);
}
