// sending 'w' through serial port turns default LED on
// sending 's' through serial port turns default LED off

// arm controller variables
int armLeftPin = 13;
int armRightPin = 12;
int armSensorPin = A0;
int armDirection = 0;
int armCount = 0;
int armPrevVal = 0;

// claw controller variables
int clawLeftPin = 11;
int clawRightPin = 10;
int clawSensorPin = A1;
int clawDirection = 0;
int clawCount = 0;
int clawPrevVal = 0;

int baseLeftPin = 9;
int baseRightPin = 8;
int baseDirection = 0;

int lastSent = 0;
int threshold = 512;
  
void setup() {  
  Serial.begin(9600);
  pinMode(13, OUTPUT);
  pinMode(12, OUTPUT);
  pinMode(11, OUTPUT);
  pinMode(10, OUTPUT);
  pinMode(9, OUTPUT);
  pinMode(8, OUTPUT);
  digitalWrite(13, LOW);
  digitalWrite(12, LOW);
  digitalWrite(11, LOW);
  digitalWrite(10, LOW);
  digitalWrite(9, LOW);
  digitalWrite(8, LOW);
}

int temp = 0;

void loop() {
  // encoder updating
  temp = analogRead(armSensorPin);
  if (temp > threshold && armPrevVal < threshold) {
    armCount += armDirection;
  }
  armPrevVal = temp;
  
  temp = analogRead(clawSensorPin);
  if (temp > threshold && clawPrevVal < threshold) {
    clawCount += clawDirection;
  }
  clawPrevVal = temp;
  
  if (lastSent > 1000) {
    Serial.print("arm:");
    Serial.println(armCount);
    Serial.print("claw:");
    Serial.println(clawCount);
    lastSent = 0;
  }
  lastSent++;
  
  // serial communication
  int inByte = Serial.read();
  switch (inByte) {
    case 'q':
      armCW();
      break;
    case 'a':
      armCCW();
      break;
    case 'w':
      clawCW();
      break;
    case 's':
      clawCCW();
      break;
    case 'e':
      baseCW();
      break;
    case 'd':
      baseCCW();
      break;
  }
  
  delay(1);
}

void armCW() {
  digitalWrite(armLeftPin, HIGH);
  digitalWrite(armRightPin, LOW);
  armDirection = 1;
}

void armCCW() {
  digitalWrite(armLeftPin, LOW);
  digitalWrite(armRightPin, HIGH);
  armDirection = -1;
}

void clawCW() {
  digitalWrite(clawLeftPin, HIGH);
  digitalWrite(clawRightPin, LOW);
  clawDirection = 1;
}

void clawCCW() {
  digitalWrite(clawLeftPin, LOW);
  digitalWrite(clawRightPin, HIGH);
  clawDirection = -1;
}

void baseCW() {
  digitalWrite(baseLeftPin, HIGH);
  digitalWrite(baseRightPin, LOW);
  baseDirection = 1;
}

void baseCCW() {
  digitalWrite(baseLeftPin, LOW);
  digitalWrite(baseRightPin, HIGH);
  baseDirection = -1;
}

