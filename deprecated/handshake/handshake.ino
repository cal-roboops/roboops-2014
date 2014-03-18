// sending 'w' through serial port turns default LED on
// sending 's' through serial port turns default LED off

String UID = "ARM_CONTROLLER";
int uid_size = sizeof(UID);

// arm controller variables
int armLowPin = 12;
int armHighPin = 13;
int armSensorPin = A0;
int armDirection = 0;
int armCount = 0;
int armPrevVal = 0;

// claw controller variables
// lowPin is connected to lower input pin of HBridge
// lower output pin of HBridge connected to positive (red) motor terminal
int clawLowPin = 10;
int clawHighPin = 11;
int clawSensorPin = A1;
int clawDirection = 0;
int clawCount = 0;
int clawPrevVal = 0;

int baseLowPin = 8;
int baseHighPin = 9;
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
  
  char buffer[uid_size];
  while (true) {
    Serial.println(UID);
    delay(500);
    Serial.readBytes(buffer, 14);
    
    boolean isUID = true;
    for (int i = 0; i < uid_size; i++) {
      if (buffer[i] != UID[i]) {
        isUID = false;
        break;
      }
    }
    
    if (isUID) {
      break;
    }
  }
  
  Serial.println("Connected!");
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
      armStop();
      break;  
    case 'z':
      armCCW();
      break;
    case 'w':
      clawClose();
      break;
    case 's':
      clawStop();
      break;
    case 'x':
      clawOpen();
      break;
    case 'e':
      baseCW();
      break;
    case 'd':
      baseStop();
      break;
    case 'c':
      baseCCW();
      break;
  }
  
  delay(1);
}

void armCW() {
  digitalWrite(armHighPin, HIGH);
  digitalWrite(armLowPin, LOW);
  armDirection = 1;
}

void armCCW() {
  digitalWrite(armHighPin, LOW);
  digitalWrite(armLowPin, HIGH);
  armDirection = -1;
}

void armStop() {
  digitalWrite(armHighPin, LOW);
  digitalWrite(armLowPin, LOW);
  armDirection = 0;
}

void clawClose() {
  digitalWrite(clawHighPin, HIGH);
  digitalWrite(clawLowPin, LOW);
  clawDirection = 1;
}

void clawOpen() {
  digitalWrite(clawHighPin, LOW);
  digitalWrite(clawLowPin, HIGH);
  clawDirection = -1;
}

void clawStop() {
  digitalWrite(clawHighPin, LOW);
  digitalWrite(clawLowPin, LOW);
  clawDirection = 0;
}

void baseCW() {
  digitalWrite(baseHighPin, HIGH);
  digitalWrite(baseLowPin, LOW);
  baseDirection = 1;
}

void baseCCW() {
  digitalWrite(baseHighPin, LOW);
  digitalWrite(baseLowPin, HIGH);
  baseDirection = -1;
}

void baseStop() {
  digitalWrite(baseHighPin, LOW);
  digitalWrite(baseLowPin, LOW);
  baseDirection = 0;
}

