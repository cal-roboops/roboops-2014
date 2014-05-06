#include "BMSerial.h"

//Arduino Due only supports using hardware uarts, pins 0/1, 14/15, 16/17 or 18/19.

BMSerial mySerial(10,11);

void setup() {
  mySerial.begin(38400);
}

void loop() {
  mySerial.write(1);
  mySerial.write(-1);
  delay(2000);
  mySerial.write(127);
  mySerial.write(-127);
  delay(2000);
}
