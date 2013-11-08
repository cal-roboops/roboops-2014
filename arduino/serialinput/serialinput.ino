// sending 'w' through serial port turns default LED on
// sending 's' through serial port turns default LED off

void setup() {  
  Serial.begin(9600);
  pinMode(13, OUTPUT);
  pinMode(12, OUTPUT);
  digitalWrite(13, LOW);
  digitalWrite(12, LOW);
}

void loop() {
  int inByte = Serial.read();
  
  
  switch (inByte) {
    case 'w':
      digitalWrite(13, HIGH);
      digitalWrite(12, LOW);
      break;
    case 's':
      digitalWrite(13, LOW);
      digitalWrite(12, HIGH);
      break;
    case 'a':
      digitalWrite(13, LOW);
      digitalWrite(12, LOW);
      break;
  }
  
  delay(100);
}
