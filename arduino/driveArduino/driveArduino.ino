#include "BMSerial.h"
#include "RoboClaw.h"

// This code uses the serial connection to read in angle values in degrees. All values must have three digits, even if you are 
// inputting values like 1. Therefore, one degree must be submitted as 001, two degrees 002, etc
//
// When using packet serial, each RoboClaw gains a specific address. To send a command to that controller, you must include the
// controller's address to ensure that it will receive the command.

// M1 is swerve, M2 is motor
#define ADDRESS_B_L 0x80
#define ADDRESS_B_R 0x81
#define ADDRESS_F_L 0x82
#define ADDRESS_F_R 0x83

#define ENCODER_B_L 2
#define ENCODER_B_R 3
#define ENCODER_F_L 4
#define ENCODER_F_R 5

#define B_L_INDEX 0
#define B_R_INDEX 1
#define F_L_INDEX 2
#define F_R_INDEX 3

#define FRONT_LEFT_WHEEL  0
#define FRONT_RIGHT_WHEEL  1
#define BACK_LEFT_WHEEL  2
#define BACK_RIGHT_WHEEL  3

#define FRONT_LEFT_SWERVE  4
#define FRONT_RIGHT_SWERVE  5
#define BACK_LEFT_SWERVE  6
#define BACK_RIGHT_SWERVE  7

RoboClaw swerves; //0, 1, 2, 3: B_L, B_R, F_L, F_R

int encoders[4]; //0, 1, 2, 3: same as swerves
char addresses[4];

int values[2];
int goals[4];

const int MOTOR_ID = 0,
          SPEED = 1;

// stores the read string for motor id
char motorChar[3];

// stores the read string for values
char valuesChar[4];

// stores the read line
char line[10];

void setup()
{
  pinMode(ENCODER_B_L, INPUT);
  pinMode(ENCODER_B_R, INPUT);
  pinMode(ENCODER_F_L, INPUT);
  pinMode(ENCODER_F_R, INPUT);
  swerves = RoboClaw(10, 11, 100); // Pin 10 on the Arduino connects to S2 on 10 and Pin 11 connects to S1 on the RoboClaw
  
  encoders[B_L_INDEX] = ENCODER_B_L;
  encoders[B_R_INDEX] = ENCODER_B_R;
  encoders[F_L_INDEX] = ENCODER_F_L;
  encoders[F_R_INDEX] = ENCODER_F_R;
  
  addresses[B_L_INDEX] = ADDRESS_B_L;
  addresses[B_R_INDEX] = ADDRESS_B_R;
  addresses[F_L_INDEX] = ADDRESS_F_L;
  addresses[F_R_INDEX] = ADDRESS_F_R;
  
  goals[0] = 45;
  goals[1] = 45;
  goals[2] = 45;
  goals[3] = 45;
  
  swerves.begin(9600);
  Serial.begin(9600);
  Serial.setTimeout(100);
}

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
  while (Serial.available())
  {
    Serial.read();
  }
}

double getPosition(int pin)
{
  double p = pulseIn(pin, HIGH);
  p = map(p, 0, 1048, 0, 360);
  return p;
}


void set(int motor, int value)
{
  boolean isEncoder = 0;
  int index;
  
  switch(motor)
  {
    case FRONT_LEFT_WHEEL:
      index = F_L_INDEX;
      isEncoder = 0;
      break;
    case FRONT_RIGHT_WHEEL:
      index = F_R_INDEX;
      isEncoder = 0;
      break;
    case BACK_LEFT_WHEEL:
      index = B_L_INDEX;
      isEncoder = 0;
      break;
    case BACK_RIGHT_WHEEL:
      index = B_R_INDEX;
      isEncoder = 0;
      break;
    case FRONT_LEFT_SWERVE:
      index = F_L_INDEX;
      isEncoder = 1;
      break;
    case FRONT_RIGHT_SWERVE:
      index = F_R_INDEX;
      isEncoder = 1;
      break;
    case BACK_LEFT_SWERVE:
      index = B_L_INDEX;
      isEncoder = 1;
      break;
    case BACK_RIGHT_SWERVE:
      index = B_R_INDEX;
      isEncoder = 1;
      break;
  }
  
  if(isEncoder)
  {
    goals[index] = (float(value)*3.1)+45;
  }
  else
  {
    drive(index, value);
  }
}

// drives motors
void drive(int motorIndex, int value)
{
  if(value > 0)
  {
    swerves.ForwardM2(addresses[motorIndex], map(value, 0, 1000, 0, 127)); 
  }
  else
  {
    swerves.BackwardM2(addresses[motorIndex], map(value, 0, 1000, 0, 127)); 
  }
}

// sets all swerves into position
void setAllSwerves()
{
  float delta;
  for (int i = 0; i<4; i++)
  {  
    delta = goals[i]-getPosition(encoders[i]);

    //Serial.println(addresses[i]);
    if(delta > 5)
    {
      swerves.ForwardM1(addresses[i], map(delta, 0, 360, 10, 127));
    }
    else if (delta < 5)
    {
      swerves.BackwardM1(addresses[i], map(-1*delta, 0, 360, 10, 127));       
    }
    else
    {
      swerves.ForwardM1(addresses[i], 0);
    }
  }
}

void loop()
{
  Serial.flush();
  delay(10);
  setAllSwerves();
  readLine(line);
  Serial.println(line);
  Serial.flush();
  delay(10);
  setAllSwerves();
  parseLine(line);
  
  flushBuffer();
  
  set(values[MOTOR_ID], values[SPEED]);
  setAllSwerves();
}
