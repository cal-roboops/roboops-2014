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
char motorChar[2];


// stores the read string for values
char valuesChar[4];
     

// stores the read line
char line[9];
     

/* Read a whole line in serial console, wait if the serial is not available */
void readLine(char* dist)
{
  int i = 0;
  char inByte;
  do
  {
    if (Serial.available())
    {
      inByte = Serial.read();
      dist[i] = inByte;
      i++;
    }
  } while(inByte != '!');
}

/* Parses a line, writes to global variables */
void parseLine(char *line)
{
  int i = 0;
  
  motorChar[0] = line[0];
  values[MOTOR_ID] = atoi(motorChar);
  
  for(i = 0; i < 3; i++)
  {
    valuesChar[i] = line[i + 4];
  }
  
  values[SPEED] = atoi(valuesChar)* (line[2] == '1' ? -1 : 1);
}


void arm(int motor, int spd) {
  if (spd<0) {
    digitalWrite(pwmPins[motor], 1);
    Serial.println(spd);
    spd = -spd;
  } else {
    digitalWrite(dirPins[motor], 0);
    Serial.println(spd);
  }
  analogWrite(motor+2, 255*(spd/1000));
}

void setup() {
  // initialize both serial ports:
  motorChar[1] = '\0';
  valuesChar[3] = '\0';
  line[8] = '\0';
  pwmPins[0] = 3;
  pwmPins[1] = 5;
  pwmPins[2] = 6;
  pwmPins[3] = 9;
  dirPins[0] = 2;
  dirPins[1] = 4;
  dirPins[2] = 7;
  dirPins[3] = 8;
  
  Serial.begin(9600);
  
  //speed
  pinMode(2, OUTPUT);
  pinMode(3, OUTPUT);
  pinMode(4, OUTPUT);
  pinMode(5, OUTPUT);
  //direction
  pinMode(30, OUTPUT);
  pinMode(31, OUTPUT);
  pinMode(32, OUTPUT);
  pinMode(33, OUTPUT);
}


void loop() {
  // read from port 1, send to port 0:
  readLine(line);
  parseLine(line);
  arm(values[MOTOR_ID],values[SPEED]);
  Serial.println("Parsed Info:");
  Serial.println(values[MOTOR_ID]);
  Serial.println(values[SPEED]);
  Serial.println("End of line");
}




  