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
int initial[4]; //initial positions for servos
int prevDir[4]; // previous direction for servos
int overShot[4]; // counts of how many times overshot
int additive[4]; // hotfix to add power slightly

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
  
  for(int i = 0; i < 4; i++)
  {
    goals[i] = 20;
    overShot[i] = 0;
    prevDir[i] = 0;
    additive[i] = 0;
    //initial[i] = getPosition(encoders[i]); 
  }
  
  initial[2] = 0;

  swerves.begin(9600);
  Serial.begin(9600);
  Serial.setTimeout(100);
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
    goals[index] = (float(value)*3.1) + 20;
    Serial.print("Set to ");
    Serial.println(goals[index]);
    overShot[index] = 0;
  }
  else
  {
    drive(index, value);
  }
}

// drives motors
void drive(int motorIndex, int value)
{
  //Serial.println("Driving!");
  //Serial.println(addresses[motorIndex]);
  if(value > 0)
  {
    swerves.ForwardM2(addresses[motorIndex], map(value, 0, 1000, 0, 127)); 
  }
  else
  {
    swerves.BackwardM2(addresses[motorIndex], map(-1*value, 0, 1000, 0, 127)); 
  }
}

// sets all swerves into position
void setAllSwerves()
{
  float delta;
  for (int i = 0; i<4; i++)
  {
    delta = int(goals[i] + int(initial[i] - getPosition(encoders[i]))%360);
    //delta = delta > 180 ? -(delta - 180) : (delta < -180 ? -(delta + 180) : delta);
    if(i==2)
    {
      Serial.print("Goal : ");
      Serial.println(goals[i]);
      Serial.print("Position : ");
      Serial.println(getPosition(encoders[i]));
      Serial.print("Initial : ");
      Serial.println(initial[i]);
      Serial.println(delta);
    }

    //Serial.println(addresses[i]);
    if(delta > 10)
    {
      swerves.ForwardM1(addresses[i], map(delta, 0, 360, 40, 80));      
    }
    else if(delta < -10)
    {
      swerves.BackwardM1(addresses[i], map(delta, 0, 360, 40, 80));
    }
    else
    {
     swerves.ForwardM1(addresses[i], 0); 
    }
    /*
    if(delta > 10 || delta < -10)
    {
      if(delta > 0)
      {
        if(prevDir[i] == -1)
        {
          overShot[i] += 0;
          additive[i] = 0; 
        }
        else
        {
          additive[i] += 0;
        }
        swerves.BackwardM1(addresses[i], int(additive[i] + map(delta/(overShot[i] + 1), 0, 360, 40, 80)));//+ 50/(overShot[i] + 1)));//map(delta/(overShot[i] + 1), 0, 360, 20, 127));
        if(i==2)
        {
          Serial.print("Moving forward at ");
          Serial.println(int(additive[i] + map(delta/(overShot[i] + 1), 0, 360, 40, 80)));//int(additive[i] + 50/(overShot[i] + 1)));//map(delta/(overShot[i] + 1), 0, 360, 20, 127));
        }
        prevDir[i] = 1;
      }
      else if (delta < 0)
      {
        if(prevDir[i] == 1)
        {
          overShot[i] += 0;
          additive[i] = 0;
        }
        else
        {
          additive[i] += 0;
        }
        swerves.ForwardM1(addresses[i], int(additive[i] +map(-1*delta/(overShot[i] + 1), 0, 360, 40, 80)));//+ 50/(overShot[i] + 1)));//map(-1*delta/(overShot[i] + 1), 0, 360, 20, 127));
        if(i==2)
         { 
          Serial.print("Moving backward at ");
          Serial.println(int(additive[i] +map(-1*delta/(overShot[i] + 1), 0, 360, 40, 80)));//int(additive[i] + 50/(overShot[i] + 1)));//map(-1*delta/(overShot[i] + 1), 0, 360, 20, 127));
          
          Serial.print(delta); Serial.println(" is less than -10.");
         }
        prevDir[i] = -1;
      }
    }
    else
    {
      overShot[i] = 0;
      additive[i] = 0;
      swerves.ForwardM1(addresses[i], 0);
    }
  }*/
}

void loop()
{
  setAllSwerves();
  while(readLine(line))
  {
    Serial.println(line);
    parseLine(line);
    set(values[MOTOR_ID], values[SPEED]);
    setAllSwerves();
  }
  Serial.println(getPosition(encoders[2]));
  setAllSwerves();
  /*
  Serial.println(getPosition(encoders[2]));
  delay(10);
  /*/
}
