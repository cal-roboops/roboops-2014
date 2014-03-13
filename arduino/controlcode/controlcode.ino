 /*=================
 Command syntax:
 0;1;100\n
 
 Meaning: motor id: 0
          positive/negative: true
          value: 100
 
 =================*/
 
// stores the interpreted values from parsing
int values[2];

const int MOTOR_ID = 0,
          SPEED = 1;

// stores the read string for motor id
char motorChar[2];
     motorChar[1] = '\0';

// stores the read string for values
char valuesChar[4];
     valuesChar[3] = '\0';

// stores the read line
char line[9];
     line[8] = '\0';

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
  } while(inByte != '\n')
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


void setup() {
  // initialize both serial ports:
  Serial.begin(9600);
}

void loop() {
  // read from port 1, send to port 0:
  readLine(line);
  parseLine(line);
  Serial.println("Parsed Info:");
  Serial.println(values[MOTOR_ID]);
  Serial.println(values[SPEED]);
  Serial.println("End of line");
}



  
