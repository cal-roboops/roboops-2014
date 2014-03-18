#include "package.h"
/*
    This file contains the protocal definition and parser for motor package.
 */






/* Constructor for motor control package */
Package new_package(int is_valid, int motor, int speed, int position)
{
    Package pack;
    pack.is_valid = is_valid;
    pack.motor = motor;
    pack.speed = speed;
    pack.position = position;
    return pack;
}


/* Read a whole line in serial console, wait if the serial is not available */
void read_line(char* dist) {
  int i = 0;
  char inByte;
  while (true) {
    if (Serial.available()) {
      inByte = Serial.read();
      dist[i] = inByte;
      i++;
      if (inByte == '\n') {
        dist[i] = '\0';
        break;
      }
    }
  }
}


    

/* Parse the input line into a motor control package */
Package parse_package(char* line) {

    if (strlen(line) != 10) {
        return new_package(0, 0, 0, 0);  //If the length of the line is not 10, this is not a valid package
    }
    char motor_code[2];
    motor_code[1] = '\0';
    motor_code[0] = line[0];
    char speed_code[4];
    char position_code[4];
    speed_code[3] = '\0';
    position_code[3] = '\0';
    int i;
    for (i = 0; i < 3; i++) {
        speed_code[i] = line[i + 2];
        position_code[i] = line[i + 6];
    }
    int motor = atoi(motor_code);
    int speed = atoi(speed_code);
    int position = atoi(position_code);
    return new_package(1, motor, speed, position);
}



void setup() {
  // initialize both serial ports:
  Serial.begin(9600);
}

void loop() {
  // read from port 1, send to port 0:
  char line[100];
  read_line(line);
  Package p = parse_package(line);
  Serial.println("======= Package Info =======");
  Serial.println(p.is_valid);
  Serial.println(p.motor);
  Serial.println(p.speed);
  Serial.println(p.position);
  Serial.println("======= End =======");
  // read from port 0, send to port 1:
}



  
