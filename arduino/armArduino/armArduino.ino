 /*=================
 Command syntax:
 0;1;100\n
 
 Meaning: motor id: 0
          positive/negative: true
          value: 100
 
 =================*/
#include "Servo.h"
 
#define MINCLAW 100
#define MAXCLAW 500
#define MINELBOW 200
#define MAXELBOW 600
#define MINSHOUL 100
#define MAXSHOUL 500
#define MINROT 100
#define MAXROT 500

#define SHOULDER_HORIZONTAL 8
#define SHOULDER_VERTICAL 9
#define ELBOW 10
#define CLAW 13

#define CAM_X 11
#define CAM_Y 12

#define SHOULDER_HORIZONTAL_INDEX 3
#define SHOULDER_VERTICAL_INDEX 1
#define ELBOW_INDEX 0
#define CLAW_INDEX 2

#define SHOULDER_HORIZONTAL_PWM 9
#define SHOULDER_VERTICAL_PWM 11
#define ELBOW_PWM 3
#define CLAW_PWM 6

#define SHOULDER_HORIZONTAL_DIR 8
#define SHOULDER_VERTICAL_DIR 12
#define ELBOW_DIR 2
#define CLAW_DIR 7

#define SHOULDER_HORIZONTAL_PONT A3
#define SHOULDER_VERTICAL_PONT A1
#define ELBOW_PONT A0
#define CLAW_PONT A2

#define MAST_SERVO 4
#define PAN_SERVO A0

Servo mast, pan;
int panPosition;
int mastOpened;
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

unsigned long flushTimer;

void setup()
{
  pinMode(SHOULDER_HORIZONTAL_PWM, OUTPUT);
  pinMode(SHOULDER_VERTICAL_PWM, OUTPUT);
  pinMode(ELBOW_PWM, OUTPUT);
  pinMode(CLAW_PWM, OUTPUT);

  pinMode(SHOULDER_HORIZONTAL_DIR, OUTPUT);
  pinMode(SHOULDER_VERTICAL_DIR, OUTPUT);
  pinMode(ELBOW_DIR, OUTPUT);
  pinMode(CLAW_DIR, OUTPUT);
  
  pinMode(MAST_SERVO, OUTPUT);
  pinMode(PAN_SERVO, OUTPUT);
  
  motorChar[2] = '\0';
  valuesChar[3] = '\0';
  line[9] = '\0';
  pwmPins[SHOULDER_HORIZONTAL_INDEX] = SHOULDER_HORIZONTAL_PWM;
  pwmPins[SHOULDER_VERTICAL_INDEX] = SHOULDER_VERTICAL_PWM;
  pwmPins[ELBOW_INDEX] = ELBOW_PWM;
  pwmPins[CLAW_INDEX] = CLAW_PWM;
  dirPins[SHOULDER_HORIZONTAL_INDEX] = SHOULDER_HORIZONTAL_DIR;
  dirPins[SHOULDER_VERTICAL_INDEX] = SHOULDER_VERTICAL_DIR;
  dirPins[ELBOW_INDEX] = ELBOW_DIR;
  dirPins[CLAW_INDEX] = CLAW_DIR;
  dirState[SHOULDER_HORIZONTAL_INDEX] = 0;
  dirState[SHOULDER_VERTICAL_INDEX] = 0;
  dirState[ELBOW_INDEX] = 0;
  dirState[CLAW_INDEX] = 0;
  dirLock[SHOULDER_HORIZONTAL_INDEX] = 0;
  dirLock[SHOULDER_VERTICAL_INDEX] = 0;
  dirLock[ELBOW_INDEX] = 0;
  dirLock[CLAW_INDEX] = 0;
  
  Serial.begin(9600);
  Serial.setTimeout(10);
  
  //mast.attach(MAST_SERVO);
  //pan.attach(PAN_SERVO);
  
  panPosition = 90;
  mastOpened = 0;
  
  //pan.write(panPosition);
  //closeCamera();
  
  analogWrite(pwmPins[SHOULDER_HORIZONTAL_INDEX], 0);
  analogWrite(pwmPins[SHOULDER_VERTICAL_INDEX], 0);
  analogWrite(pwmPins[ELBOW_INDEX], 0);
  analogWrite(pwmPins[CLAW_INDEX], 0);
  
  flushTimer=millis();
}
     

/* Read a whole line in serial console, wait if the serial is not available */
boolean readLine(char* dist)
{
  return Serial.readBytesUntil('!', dist, 9);
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
  while (Serial.available())
  {
    Serial.read();
  }
}

void arm(int motor, int spd)
{
  switch(motor)
  {
    case CLAW:
      motor=CLAW_INDEX;
      break;
    case ELBOW:
      motor=ELBOW_INDEX;
      break;
    case SHOULDER_HORIZONTAL:
      motor=SHOULDER_HORIZONTAL_INDEX;
      break;
    case SHOULDER_VERTICAL:
      motor=SHOULDER_VERTICAL_INDEX;
      break;
    case CAM_X:
      panCamera(spd);
      return;
    case CAM_Y:
      if(spd == 0)
      {
        mastOpened = 0;
      }
      else
      {
        mastOpened = 1;
        deployCamera();
      }
    default:
      return;
  }
  if (spd<0)
  {
    digitalWrite(dirPins[motor], 1);
    dirState[motor] = 1;
    spd = -1*spd;
  }
  else
  {
    digitalWrite(dirPins[motor], 0);
    dirState[motor] = 0;
  }
  //long a = ;//255*long(spd)/1000;
  
  Serial.println(pwmPins[motor]);
  Serial.println(map(spd, 0, 1000, 0, 255));

  analogWrite(pwmPins[motor], map(spd, 0, 1000, 0, 255));
  
  /*
  if(pwmPins[motor] == 9)
  {
    Serial.println("Writing here!");
    analogWrite(9, );
  }*/
}

void deployCamera()
{
  mast.attach(MAST_SERVO);
  for(int i = mast.read(); i > 1; i--)
  {
    mast.write(i);
    delay(15);
    Serial.println(mast.read());
  }
  mast.detach();
}

void closeCamera()
{
  /*
  for(int i = mast.read(); i < 85; i++)
  {
    mast.write(i);
    delay(15);
  }*/
}

void panCamera(int heading)
{
  panPosition += heading;
  if(panPosition > 180)
    panPosition = 180;
  if(panPosition < 0)
    panPosition = 0;
  //panPosition = (panPosition > 180 ? 180 : (panPosition < 0 ? 0 : panPosition));
  pan.write(panPosition);
  Serial.println(pan.read());
}

void checkArm()
{
  int pot = analogRead(ELBOW_PONT);
//  Serial.print("Position: ");
//  Serial.println(pot);
  
  if ((pot < MINELBOW && dirState[ELBOW_INDEX]) || (pot > MAXELBOW && !dirState[ELBOW_INDEX]))
  {
    dirLock[ELBOW_INDEX] = 1;
    analogWrite(pwmPins[ELBOW_INDEX], 0);
  }
  pot = analogRead(SHOULDER_VERTICAL_PONT);
  if ((pot < MINSHOUL && dirState[SHOULDER_VERTICAL_INDEX]) || (pot > MAXSHOUL && !dirState[SHOULDER_VERTICAL_INDEX]))
  {
    dirLock[SHOULDER_VERTICAL_INDEX] = 1;
    analogWrite(pwmPins[SHOULDER_VERTICAL_INDEX], 0);
  }
  pot = analogRead(CLAW_PONT);
  if ((pot < MINCLAW && dirState[CLAW_INDEX]) || (pot > MAXCLAW && !dirState[CLAW_INDEX]))
  {
    dirLock[CLAW_INDEX] = 1;
    analogWrite(pwmPins[CLAW_INDEX], 0);
  }
  pot = analogRead(SHOULDER_HORIZONTAL_PONT);
  if ((pot < MINROT && dirState[SHOULDER_HORIZONTAL_INDEX]) || (pot > MAXROT && !dirState[SHOULDER_HORIZONTAL_INDEX]))
  {
    dirLock[SHOULDER_HORIZONTAL_PWM] = 1;
    analogWrite(pwmPins[SHOULDER_HORIZONTAL_INDEX], 0);
  }
}

void loop()
{
  if (millis()-flushTimer > 30000) {
    while(Serial.available()) Serial.read();
    flushTimer=millis();
  }
  
  while(readLine(line))
  {
    Serial.println(line);
    //checkArm();
    parseLine(line);
   
    arm(values[MOTOR_ID], values[SPEED]);
    //checkArm(); 
  }
  
  //deployCamera();
  
  //digitalWrite(8, HIGH);
  //analogWrite(9, 100);
  
  
  /*
  Serial.flush();
  
  delay(10);
  readLine(line);
  Serial.println(line);
  Serial.flush();
  checkArm();
  delay(10);
  parseLine(line);

  flushBuffer();
  delay(10);
  
  arm(values[MOTOR_ID],values[SPEED]);
  checkArm();*/
  
  /*
  panCamera(25);
  delay(600);
  panCamera(-25);
  delay(600);*/
  //deployCamera();
  
  //analogWrite(SHOULDER_VERTICAL_PWM, 0);
  //analogWrite(SHOULDER_HORIZONTAL_PWM, 0);
  /*
  analogWrite(SHOULDER_HORIZONTAL_PWM, 400);
  delay(500);
  analogWrite(SHOULDER_HORIZONTAL_PWM, 0);
  delay(100000);*/
}




  
