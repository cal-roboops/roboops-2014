// sending 'w' through serial port turns default LED on
// sending 's' through serial port turns default LED off

int sensorPin = A0;
int threshold = 512;
int count = 0;
int prevVal = 0;
int lastSent = 0;
  
void setup() {  
  Serial.begin(9600);
}

void loop() {
  int sensorVal = analogRead(sensorPin);
  // Serial.println(sensorVal);
  
  if (sensorVal > threshold && prevVal < threshold) {
    count++;
  }
  prevVal = sensorVal;
  
  if (lastSent > 1000) {
    Serial.println(count);
    lastSent = 0;
  }
  
  lastSent++;
  
  delay(1);
  
}
