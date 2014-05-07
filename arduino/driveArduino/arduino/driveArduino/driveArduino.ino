#include "BMSerial.h"
#include "RoboClaw.h"

// This code uses the serial connection to read in angle values in degrees. All values must have three digits, even if you are 
// inputting values like 1. Therefore, one degree must be submitted as 001, two degrees 002, etc
//
// When using packet serial, each RoboClaw gains a specific address. To send a command to that controller, you must include the
// controller's address to ensure that it will receive the command.

// Example input: 06;0;180!
#define ADDRESS_B_L 0x80
#define ADDRESS_B_R 0x80
#define ADDRESS_F_L 0x80
#define ADDRESS_F_R 0x80

#define ENCODER_B_L 2
#define ENCODER_B_R 2
#define ENCODER_F_L 2
#define ENCODER_F_R 2

RoboClaw swerves[4]; //0, 1, 2, 3: B_L, B_R, F_L, F_R

int encoders[4]; //0, 1, 2, 3: same as swerves
char addresses[4];

int values[2];

const int MOTOR_ID = 0,
SPEED = 1;

// stores the read string for motor id
char motorChar[3];

// stores the read string for values
char valuesChar[4];

// stores the read line
char line[10];
char last[10];

// the current position of the swerve module, out of 360 degrees, adjusted for the gear ratio of 3:1?
int currPos = 0;
// the number of full rotations on the encoder
int rot = 0;
int newRot = 0;

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

double getPosition(int pin)
{
  double p = pulseIn(pin, HIGH);
  Serial.println(p);
  p = map(p, 0, 1300, 0, 360);  //was 1048 instead of 1300: should be limite of the encoder
  return p;
}

double normalize(double delta)
{
  if(delta > 180)
  {
    delta = delta - 360;
  }
  else if(delta < -180)
  {
    delta = delta + 360;
  }
  return delta;
}

void activate(int motor, int value)
{
  float destination = float(value);
  float delta;
  int isEncoder;
  
  if (destination >= 240) {
    newRot = 2;
  } else if (destination >= 120) {
    newRot = 1;
  }
  destination = destination - newRot*120;

  switch(motor)
  {
  case 6:
    motor=0;
    isEncoder = 1;
    break;
  }

  if(isEncoder)
  {
    //currPos = getPosition(encoders[motor]);
    delta = normalize(getPosition(encoders[motor]) - destination);
    //delta = currPos;
    
    while (rot != newRot) {
      if (rot > newRot) {
        swerves[motor].ForwardM1(addresses[motor],64);
        rot--;
      } else {
        swerves[motor].BackwardM2(addresses[motor],64);
        rot++;
      }
      if (rot == 3) {
        rot = 0;
      }
    }
    if (rot == 3) {
      rot = 0;
    }
    if (currPos > destination) {
      //swerves[motor].ForwardM1(addresses[motor], map(delta, 0, 180, 0, 64));
      swerves[motor].ForwardM1(addresses[motor], map(delta, 0, 359, 0, 127));
    } else if (currPos < destination) {
      //swerves[motor].BackwardM1(addresses[motor], map(-1*delta, 0, 180, 0, 64)); 
      swerves[motor].BackwardM1(addresses[motor], map(-1*delta, 0, 359, 0, 127)); 
    }

    //if(delta >= 0)
    //{
    //  swerves[motor].ForwardM1(addresses[motor], map(delta, 0, 180, 0, 64));
    //}
    //else
    //{
    //  swerves[motor].BackwardM1(addresses[motor], map(-1*delta, 0, 180, 0, 64));       
    //}
  }
  else
  {
    if(value > 0)
    {
      swerves[motor].ForwardM2(addresses[motor], map(value, 0, 1000, 0, 64));
    }
    else
    {
      swerves[motor].BackwardM2(addresses[motor], map(-1*value, 0, 1000, 0, 64)); 
    }
  }
}

void setup()
{
  pinMode(ENCODER_B_L, INPUT);
  pinMode(ENCODER_B_R, INPUT);
  pinMode(ENCODER_F_L, INPUT);
  pinMode(ENCODER_F_R, INPUT);
  swerves[0] = RoboClaw(10, 11, 100); // Pin 10 on the Arduino connects to S2 on 10 and Pin 11 connects to S1 on the RoboClaw
  //swerves[1] = RoboClaw(10, 11, 10000); 
  //swerves[2] = RoboClaw(10, 11, 10000); 
  //swerves[3] = RoboClaw(10, 11, 10000);

  encoders[0] = ENCODER_B_L;
  encoders[1] = ENCODER_B_R;
  encoders[2] = ENCODER_F_L;
  encoders[3] = ENCODER_F_R;

  addresses[0] = ADDRESS_B_L;
  addresses[1] = ADDRESS_B_R;
  addresses[2] = ADDRESS_F_L;
  addresses[3] = ADDRESS_F_R;

  swerves[0].begin(9600);
  Serial.begin(9600);
  Serial.setTimeout(100);
}

void loop()
{
  Serial.flush();
  delay(10);
  readLine(line);
  Serial.println(line);
  Serial.flush();
  delay(10);
  parseLine(line);

  flushBuffer();


  //if (strcmp(line, last) != 0) {
  activate(values[MOTOR_ID], values[SPEED]);
  //}

  //strlcpy(last, line,sizeof(last));
}

