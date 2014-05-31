#include "BMSerial.h"
#include "RoboClaw.h"

// This code uses the serial connection to read in angle values in degrees. All values must have three digits, even if you are 
// inputting values like 1. Therefore, one degree must be submitted as 001, two degrees 002, etc
//
// When using packet serial, each RoboClaw gains a specific address. To send a command to that controller, you must include the
// controller's address to ensure that it will receive the command.

// one drives left wheels, one drives right wheels, one drives swerves
#define ADDRESS_B_L 0x80
#define ADDRESS_B_R 0x81
#define ADDRESS_F_L 0x82
#define ADDRESS_F_R 0x83

#define ADDRESS_RIGHT 0x81
#define RIGHT_M1 BACK_RIGHT_WHEEL
#define RIGHT_M2 FRONT_RIGHT_WHEEL

#define ADDRESS_LEFT 0x80
#define LEFT_M1 BACK_LEFT_WHEEL
#define LEFT_M2 FRONT_LEFT_WHEEL

#define ADDRESS_SWERVE 0x83
#define SWERVE_M1 FRONT_LEFT_SWERVE
#define SWERVE_M2 FRONT_RIGHT_SWERVE
#define SWERVE_M1_INDEX F_L_INDEX
#define SWERVE_M2_INDEX F_R_INDEX

#define ENCODER_B_L 2
#define ENCODER_B_R 4
#define ENCODER_F_L 3
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
int goals[4]; //goals for servos

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
  
  encoders[B_L_INDEX] = ENCODER_B_L; // only used to match up with indices of motors
  encoders[B_R_INDEX] = ENCODER_B_R; // ''
  encoders[F_L_INDEX] = ENCODER_F_L;
  encoders[F_R_INDEX] = ENCODER_F_R;
  
  addresses[B_L_INDEX] = ADDRESS_B_L;
  addresses[B_R_INDEX] = ADDRESS_B_R;
  addresses[F_L_INDEX] = ADDRESS_F_L;
  addresses[F_R_INDEX] = ADDRESS_F_R;
  
  for(int i = 0; i < 4; i++)
  {
    goals[i] = 20;
  }

  swerves.begin(9600);
  Serial.begin(9600);
  Serial.setTimeout(10);
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

// gets position of encoder at pin
double getPosition(int pin)
{
  double p = pulseIn(pin, HIGH);
  p = map(p, 0, 1048, 0, 360);
  return p;
}

// sets a value for a motor or encoder, depending on given ids
void set(int motor, int value)
{
  boolean isEncoder = 0;
  int index;
  
  switch(motor)
  {
    case FRONT_LEFT_SWERVE:
      index = F_L_INDEX;
      isEncoder = 1;
      break;
    case FRONT_RIGHT_SWERVE:
      index = F_R_INDEX;
      isEncoder = 1;
      break;
    // no longer have support for back swerves
    case BACK_LEFT_SWERVE:
    case BACK_RIGHT_SWERVE:
      return;
  }
  
  if(isEncoder)
  {
    goals[index] = (float(value)*3.1) + 20;
    Serial.print("Set to ");
    Serial.println(goals[index]);
  }
  else
  {
    drive(motor, value);
  }
}

// sets a robo claw to a value; value between -1000 and 1000; id is 1 or 2 (motor); address is roboclaw address
void setRoboClaw(int id, int address, int value)
{
  setRoboClaw(id, address, value, 0, 1000, 0, 127);
}

// sets a robo claw to a value, fed given ranges and given output ranges
void setRoboClaw(int id, int address, int value, int minInp, int maxInp, int minVal, int maxVal)
{
  if(id == 1)
  {
    if(value > 0)
    {
      swerves.ForwardM1(address, map(value, minInp, maxInp, minVal, maxVal));
    }
    else
    {
      swerves.BackwardM1(address, map(-1*value, minInp, maxInp, minVal, maxVal));
    }
  }
  else
  {
    if(value > 0)
    {
      swerves.ForwardM2(address, map(value, minInp, maxInp, minVal, maxVal));
    }
    else
    {
      swerves.BackwardM2(address, map(-1*value, minInp, maxInp, minVal, maxVal));
    }
  }
}

// drives motors
void drive(int motor, int value)
{
  switch(motor)
  {
    case LEFT_M1:
      setRoboClaw(1, ADDRESS_LEFT, value);
      break;
    case LEFT_M2:
      setRoboClaw(2, ADDRESS_LEFT, value);
      break;
    case RIGHT_M1:
      setRoboClaw(1, ADDRESS_RIGHT, value);
      break;
    case RIGHT_M2:
      setRoboClaw(2, ADDRESS_RIGHT, value);
      break;
    default:
      Serial.println("What the fuck? Driving bad motor!");    
  }
}

// sets all swerves into position
void setAllSwerves()
{
  float delta;
  /*
  delta = int(goals[SWERVE_M1_INDEX] - getPosition(encoders[SWERVE_M1_INDEX]));
  if(delta > 10 || delta < 10)
  {
    setRoboClaw(1, ADDRESS_SWERVE, delta, 0, 360, 40, 80);
  }
  else
  {
    setRoboClaw(1, ADDRESS_SWERVE, 0); 
  }
  
  delta = int(goals[SWERVE_M2_INDEX] - getPosition(encoders[SWERVE_M2_INDEX]));
  if(delta > 10 || delta < 10)
  {
    setRoboClaw(2, ADDRESS_SWERVE, delta, 0, 360, 40, 80);
  }
  else
  {
    setRoboClaw(2, ADDRESS_SWERVE, 0); 
  }*/
}

void loop()
{
  //setAllSwerves();
  while(readLine(line))
  {
    Serial.println(line);
    //parseLine(line);
    //set(values[MOTOR_ID], values[SPEED]);
    setAllSwerves();
  }
  Serial.println(getPosition(encoders[2]));
  //setAllSwerves();
}

