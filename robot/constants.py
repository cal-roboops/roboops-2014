#####################
#  		PORTS       #
#####################
FRONT_LEFT_WHEEL = 0
FRONT_RIGHT_WHEEL = 1
BACK_LEFT_WHEEL = 2
BACK_RIGHT_WHEEL = 3

ARM_0 = 4
ARM_1 = 5
ARM_2 = 6

CAM_X = 7
CAM_Y = 8
#I don't know how ports work. But it's a dictionary. So shouldn't matter.
#You can even call each motor a string. Whatever's easier to send to arduinos.

#####################
#  	   COMMANDS     #
#####################
FRONT_LEFT_WHEEL_COMMAND = '0'
FRONT_RIGHT_WHEEL_COMMAND = '1'
BACK_LEFT_WHEEL_COMMAND = '2'
BACK_RIGHT_WHEEL_COMMAND = '3'

ARM_0_COMMAND = '4'
ARM_1_COMMAND = '5'
ARM_2_COMMAND = '6'

CAM_X_COMMAND = '7'
CAM_Y_COMMAND = '8'

MODE_COMMAND = 'M'
TURN_COMMAND = 'T'
LEFT_DRIVE_COMMAND = 'L'
RIGHT_DRIVE_COMMAND = 'R'
#put more special commands here

#####################
#  	    MODES       #
#####################
TANK_DRIVE = 0
PIVOT_SPIN = 1

TIMEOUT = .600 #in s
