/* Package spec:
    [0-9];[000-255];[000-255]
    Motor|Speed    |Position

    The line should be exactly 10 characters (1 motor code + ';' + 3 speed code + ';' + 3 position code + 1 end of line)


 */

/* Definition of motor control package */
typedef struct {
    int is_valid;
    int motor;
    int speed;
    int position;
} Package;