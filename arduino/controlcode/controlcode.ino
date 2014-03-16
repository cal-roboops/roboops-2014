 /*=================
 Command syntax:
 0;1;100\n
 
 Meaning: motor id: 0
          positive/negative: true
          value: 100
 
 =================*/
 
// stores the interpreted values from parsing
int values[2];

int pwmPins[4];
int dirPins[4];

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
//  int i = 0;
//  char inByte =0;
//  while(inByte != '!')
//  {
//    if (Serial.available())
//    {
//      inByte = Serial.read();
//      Serial.print(inByte);
//      dist[i] = inByte;
//      i++;
//    }
//  }
//  Serial.println(" ");
//  dist[i]='\0';
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
  }
  if (spd<0) {
    digitalWrite(dirPins[motor], 1);
    spd = -spd;
  } else {
    digitalWrite(dirPins[motor], 0);
  }
  long a = 255*long(spd)/1000;
  analogWrite(pwmPins[motor], a);
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
  
}


void loop() {
  // read from port 1, send to port 0:
  Serial.flush();
  delay(10);
  readLine(line);
  Serial.println(line);
  Serial.flush();
  delay(10);
  parseLine(line);
  
  flushBuffer();
  delay(100);

  arm(values[MOTOR_ID],values[SPEED]);
}




  
