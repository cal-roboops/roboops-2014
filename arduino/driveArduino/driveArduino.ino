#include "BMSerial.h"
#include "RoboClaw.h"

// This code uses the serial connection to read in angle values in degrees. All values must have three digits, even if you are 
// inputting values like 1. Therefore, one degree must be submitted as 001, two degrees 002, etc
//
// When using packet serial, each RoboClaw gains a specific address. To send a command to that controller, you must include the
// controller's address to ensure that it will receive the command.
#define ADDRESS_B_L 0x80
#define ADDRESS_B_R 0x81
#define ADDRESS_F_L 0x82
#define ADDRESS_F_R 0x83

#define ENCODER_B_L 2
#define ENCODER_B_R 3
#define ENCODER_F_L 4
#define ENCODER_F_R 5

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


void set(int motor, int value)
{
  boolean isEncoder = 0;
  
  switch(motor)
  {
    case 6:
      motor=0;
      isEncoder = 1;
      break;
      
    case 7:
      motor=1;
      isEncoder = 1;
      break;
    case 2:
      motor=0;
      isEncoder = 0;
      break;
    case 3:
      motor=1;
      isEncoder = 0;
      break;
  }
  
  if(isEncoder) {
    goals[motor] = (float(value)*3.1)+45;
  } else {
    if(value > 0) {
      swerves.ForwardM2(addresses[motor], map(value, 0, 1000, 0, 127));
    } else {
      swerves.BackwardM2(addresses[motor], map(-1*value, 0, 1000, 0, 127)); 
    }
  }
}

void activate() {
  float delta;
  for (int i = 0; i<2; i++) {  
    delta = goals[i]-getPosition(encoders[i]);

    //Serial.println(addresses[i]);
    if(delta > 5) {
      swerves.ForwardM1(0x80+i, map(delta, 0, 360, 10, 127));
    } else if (delta < 5) {
      swerves.BackwardM1(0x80+i, map(-1*delta, 0, 360, 10, 127));       
    } else {
        swerves.ForwardM1(0x80+i, 0);
    }
  }
}

void setup()
{
  pinMode(ENCODER_B_L, INPUT);
  pinMode(ENCODER_B_R, INPUT);
  pinMode(ENCODER_F_L, INPUT);
  pinMode(ENCODER_F_R, INPUT);
  swerves = RoboClaw(10, 11, 100); // Pin 10 on the Arduino connects to S2 on 10 and Pin 11 connects to S1 on the RoboClaw
  
  encoders[0] = ENCODER_B_L;
  encoders[1] = ENCODER_B_R;
  encoders[2] = ENCODER_F_L;
  encoders[3] = ENCODER_F_R;
  
  addresses[0] = ADDRESS_B_L;
  addresses[1] = ADDRESS_B_R;
  addresses[2] = ADDRESS_F_L;
  addresses[3] = ADDRESS_F_R;

  
  goals[0] = 45;
  goals[1] = 45;
  goals[2] = 45;
  goals[3] = 45;
  
  swerves.begin(9600);
  Serial.begin(9600);
  Serial.setTimeout(100);
}

void loop()
{
  Serial.flush();
  delay(10);
  activate();
  readLine(line);
  Serial.println(line);
  Serial.flush();
  delay(10);
  activate();
  parseLine(line);
  
  flushBuffer();
  
  set(values[MOTOR_ID], values[SPEED]);
  activate();

}
