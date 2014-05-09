 /*=================
 Command syntax:
 0;1;100\n
 
 Meaning: motor id: 0
          positive/negative: true
          value: 100
 
 =================*/
#define MINCLAW 100
#define MAXCLAW 500
#define MINELBOW 200
#define MAXELBOW 600
#define MINSHOUL 100
#define MAXSHOUL 500
#define MINROT 100
#define MAXROT 500
// stores the interpreted values from parsing
int values[2];

int pwmPins[4];
int dirPins[4];
bool dirState[4];
bool dirLock[4];

const int MOTOR_ID = 0,
          SPEED = 1;

// stores the read string for motor id
char motorChar[3];


// stores the read string for values
char valuesChar[4];

char incomingByte;

// stores the read line
char line[10];
char newline[10];
     

/* Read a whole line in serial console, wait if the serial is not available */
void readLine(char* dist)
{
  int count = 0;
  Serial.readBytesUntil('!', dist, 9);
//  while (Serial.available() > 0 && count<9 && incomingByte != '!') {
//    // read the incoming byte:
//    incomingByte = Serial.read();
//    dist[count] = incomingByte;
//    count++;
//    
//    checkArm();
//  }
}

/* Parses a line, writes to global variables */
void parseLine(char *line)
{
  int i = 0;
  
  motorChar[0] = line[0];
  motorChar[1] = line[1];
  
  values[MOTOR_ID] = atoi(motorChar);
  
  for(i = 0; i < 3; i++)
  {
    valuesChar[i] = line[i + 5];
  }

  values[SPEED] = atoi(valuesChar)* (line[3] == '1' ? -1 : 1);
}

void flushBuffer()
{
  while (Serial.available()) {
    Serial.read();
  }
}

void arm(int motor, int spd) {
  switch(motor) {
    case 13:
      motor=2;
      break;
    case 10:
      motor=0;
      break;
    case 9:
      motor=1;
      break;
    case 8:
      motor=3;
  }
  if (spd<0) {
    digitalWrite(dirPins[motor], 1);
    if (dirLock[motor] && dirState[motor]==1) {
      spd = 0;
    } else {
      spd = -spd;
      dirLock[motor] = 0;
    }
    dirState[motor] = 1;
  } else {
    digitalWrite(dirPins[motor], 0);
    if (dirLock[motor] && dirState[motor]==0) {
      spd = 0;
    } else {
      dirLock[motor] = 0;
    }
    dirState[motor] = 0;
  }
  long a = 255*long(spd)/1000;

  analogWrite(pwmPins[motor], a);
}

void checkArm() {
  int pot = analogRead(A0);
//  Serial.print("Position: ");
//  Serial.println(pot);
  
  if ((pot < MINELBOW && dirState[0]) || (pot > MAXELBOW && !dirState[0])) {
    dirLock[0] = 1;
    analogWrite(pwmPins[0], 0);
  }
  pot = analogRead(A1);
  if ((pot < MINSHOUL && dirState[1]) || (pot > MAXSHOUL && !dirState[1])) {
     dirLock[1] = 1;
    analogWrite(pwmPins[1], 0);
  }
  pot = analogRead(A2);
  if ((pot < MINCLAW && dirState[2]) || (pot > MAXCLAW && !dirState[2])) {
     dirLock[2] = 1;
    analogWrite(pwmPins[2], 0);
  }
  pot = analogRead(A3);
  if ((pot < MINROT && dirState[3]) || (pot > MAXROT && !dirState[3])) {
     dirLock[3] = 1;
    analogWrite(pwmPins[3], 0);
  }
}

void setup() {
  // initialize both serial ports:
  Serial.flush();
  delay(10);
  motorChar[2] = '\0';
  valuesChar[3] = '\0';
  line[9] = '\0';
  pwmPins[0] = 3;
  pwmPins[1] = 5;
  pwmPins[2] = 6;
  pwmPins[3] = 9;
  dirPins[0] = 2;
  dirPins[1] = 4;
  dirPins[2] = 7;
  dirPins[3] = 8;
  dirState[0] = 0;
  dirState[1] = 0;
  dirState[2] = 0;
  dirState[3] = 0;
  dirLock[0] = 0;
  dirLock[1] = 0;
  dirLock[2] = 0;
  dirLock[3] = 0;
  
  analogWrite(pwmPins[0], 0);
  analogWrite(pwmPins[1], 0);
  analogWrite(pwmPins[2], 0);
  analogWrite(pwmPins[3], 0);
  
  Serial.begin(9600);
  Serial.setTimeout(100);
  
  
  pinMode(2, OUTPUT);
  pinMode(3, OUTPUT);
  pinMode(4, OUTPUT);
  pinMode(5, OUTPUT);
  pinMode(6, OUTPUT);
  pinMode(7, OUTPUT);
  pinMode(8, OUTPUT);
  
}


void loop() {
  // read from port 1, send to port 0:
  Serial.flush();
  
  delay(10);
  readLine(line);
  //checkArm();
  Serial.println(line);
  Serial.flush();
  checkArm();
  delay(10);
  parseLine(line);

  flushBuffer();
  delay(10);
  
  arm(values[MOTOR_ID],values[SPEED]);
  checkArm();
  //Serial.println("10");
}




  
