#include <SPI.h>
#include <Orion.h>

void setup() {
  Serial.begin(115200);

  // initialize Orion
  Orion.begin();
}

void loop() {
  Serial.print("Version:");
  Serial.println(Orion.QueryVersion(),HEX);
  delay(500);
}

