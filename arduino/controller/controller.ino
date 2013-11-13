// sending 'w' through serial port turns default LED on
// sending 's' through serial port turns default LED off

int sensorPin = A0;
int threshold = 512;
int count = 0;
int prevVal = 0;
int lastSent = 0;
  
void setup() {  
  Serial.begin(9600);
  pinMode(13, OUTPUT);
  pinMode(12, OUTPUT);
  digitalWrite(13, LOW);
  digitalWrite(12, LOW);
}

void loop() {
  // encoder updating
  int sensorVal = analogRead(sensorPin);
  if (sensorVal > threshold && prevVal < threshold) {
    count++;
  }
  prevVal = sensorVal;
  if (lastSent > 1000) {
    Serial.println(count);
    lastSent = 0;
  }
  lastSent++;
  
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
  }
  
  delay(1);
}
