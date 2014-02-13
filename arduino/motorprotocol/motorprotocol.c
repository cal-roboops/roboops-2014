/*
    This file contains the protocal definition and parser for motor package.
    The file is in C format, but should be converted to arduino before compiling.

 */


/* Package spec:
    [0-9];[000-255];[000-255]
    Motor|Speed    |Position

    The line should be exactly 10 characters (1 motor code + ';' + 3 speed code + ';' + 3 position code + 1 end of line)


 */

/* Definition of motor control package */
typedef struct
{
    int is_valid;
    int motor;
    int speed;
    int position;
} Package;


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


/* Parse the input line into a motor control package */
Package parse_package(char* line) {
    Package pack;

    if (strlen(line) != 10) {
        return new_package(0, 0, 0, 0);  //If the length of the line is not 10, this is not a valid package
    }
    char motor_code[1];
    motor_code[0] = line[0];
    char speed_code[3];
    char position_code[3];
    int i;
    for (i = 0; i < 3; i++) {
        speed_code[i] = line[i + 2];
        position_code[i] = line[i + 6];
    }
    int motor = atoi(motor_code);
    int speed = atoi(speed_code);
    int position = atoi(speed_code);
    return new_package(1, motor, speed, position);
}

