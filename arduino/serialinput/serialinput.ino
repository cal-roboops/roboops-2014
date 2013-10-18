// sending 'w' through serial port turns default LED on
// sending 's' through serial port turns default LED off

void setup() {  
  Serial.begin(9600);
  pinMode(13, OUTPUT);
}

void loop() {
  int inByte = Serial.read();
  
  
  switch (inByte) {
    case 'w':
      digitalWrite(13, HIGH);
      break;
    case 's':
      digitalWrite(13, LOW);
      break;
  }
  
  delay(100);
}
