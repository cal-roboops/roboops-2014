#include "BMSerial.h"
#include "RoboClaw.h"

// This code uses the serial connection to read in angle values in degrees. All values must have three digits, even if you are 
// inputting values like 1. Therefore, one degree must be submitted as 001, two degrees 002, etc
//
// When using packet serial, each RoboClaw gains a specific address. To send a command to that controller, you must include the
// controller's address to ensure that it will receive the command.
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
  p = map(p, 0, 1048, 0, 360);
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
  
  switch(motor)
  {
    case 6:
      motor=0;
      break;
  }
  
  delta = normalize(getPosition(encoders[motor]) - destination);
  
  if(delta >= 0)
  {
    swerves[motor].ForwardM1(addresses[motor], map(delta, 0, 180, 0, 64));
  }
  else
  {
    swerves[motor].BackwardM1(addresses[motor], map(-1*delta, 0, 180, 0, 64));       
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
  
  activate(values[MOTOR_ID], values[SPEED]);
}
