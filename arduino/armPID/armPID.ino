

#include <PID_v1.h>

/*=================
 Command syntax:
 0;1;100\n
 
 Meaning: motor id: 0
          positive/negative: true
          value: 100
 
=================*/
#define MINCLAW 100
#define MAXCLAW 500
#define MINELBOW 100
#define MAXELBOW 500
#define MINSHOUL 100
#define MAXSHOUL 500
#define MINROT 100
#define MAXROT 500
// stores the interpreted values from parsing
int values[2];

int pwmPins[4];
int dirPins[4];
bool dirState[4];

#define SAMPLETIME 100
//Define Variables we'll be connecting to
double Setpoint, LastInput, Input, Output, Speed;
unsigned long time;
//Specify the links and initial tuning parameters
PID ElbowPID(&Input, &Output, &Setpoint,1,0,0, DIRECT);
PID ShoulPID(&Input, &Output, &Setpoint,1,0,0, DIRECT);
PID RotPID(&Input, &Output, &Setpoint,1,0,0, DIRECT);

const int MOTOR_ID = 0,
          SPEED = 1;

// stores the read string for motor id
char motorChar[3];


// stores the read string for values
char valuesChar[4];
     

// stores the read line
char line[10];
     

/* Read a whole line in serial console, wait if the serial is not available */
void readLine(char* dist)
{
  Serial.readBytesUntil('!', dist, 9);
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
    spd = -spd,;
    dirState[motor] = 1;
  } else {
    digitalWrite(dirPins[motor], 0);
    dirState[motor] = 0;
  }
  long a = 255*long(spd)/1000;
  analogWrite(pwmPins[motor], a);
}

void checkArm() {
  int pot = analogRead(A0);
  if ((pot < MINELBOW && dirState[0]) || (pot > MAXELBOW && !dirState[0])) {
    analogWrite(pwmPins[0], 0);
  }
  if ((pot < MINSHOUL && dirState[1]) || (pot > MAXSHOUL && !dirState[1])) {
    analogWrite(pwmPins[1], 0);
  }
  if ((pot < MINCLAW && dirState[2]) || (pot > MAXCLAW && !dirState[2])) {
    analogWrite(pwmPins[2], 0);
  }
  if ((pot < MINROT && dirState[3]) || (pot > MAXROT && !dirState[3])) {
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
  
  analogWrite(pwmPins[0], 0);
  analogWrite(pwmPins[1], 0);
  analogWrite(pwmPins[2], 0);
  analogWrite(pwmPins[3], 0);
  
  Serial.begin(9600);
  
  
  pinMode(2, OUTPUT);
  pinMode(3, OUTPUT);
  pinMode(4, OUTPUT);
  pinMode(5, OUTPUT);
  pinMode(6, OUTPUT);
  pinMode(7, OUTPUT);
  pinMode(8, OUTPUT);
  
    //initialize the variables we're linked to
  LastInput = 0;
  Input = 0;
  Setpoint = 500;
  Output = 0;
  Speed = 0;
  time = millis();
  
  //turn the PID on
  ElbowPID.SetMode(AUTOMATIC);
  ElbowPID.SetSampleTime(SAMPLETIME);
  ElbowPID.SetOutputLimits(-1000,1000);  
  
  ShoulPID.SetMode(AUTOMATIC);
  ShoulPID.SetSampleTime(SAMPLETIME);
  ShoulPID.SetOutputLimits(-255,255);  
  
  RotPID.SetMode(AUTOMATIC);
  RotPID.SetSampleTime(SAMPLETIME);
  RotPID.SetOutputLimits(-255,255);  
}


void loop() {
  // read from port 1, send to port 0:
  Serial.flush();
  delay(10);
  readLine(line);
  //Serial.println(line);
  Serial.flush();
  delay(10);
  parseLine(line);
  checkArm();
  flushBuffer();
  delay(100);
  
  Setpoint = values[SPEED];
  LastInput = Input;
  Input = analogRead(A0);
  Speed= 1000*((Input-LastInput)/(millis()-time));
  time=millis();
  
  Serial.print("Input: ");
  Serial.println(Speed);
  Serial.print("Setpoint: ");
  Serial.println(Setpoint);
  ElbowPID.Compute();
  
  Serial.print("Output: ");
  Serial.println(Output);
  arm(values[MOTOR_ID],Output);
  //analogWrite(3,Output);
  
}
