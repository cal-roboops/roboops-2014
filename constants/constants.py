#####################
#  		PORTS       #
#####################
FRONT_LEFT_WHEEL = 0
FRONT_RIGHT_WHEEL = 1
BACK_LEFT_WHEEL = 2
BACK_RIGHT_WHEEL = 3

FRONT_LEFT_SWERVE = 4
FRONT_RIGHT_SWERVE = 5
BACK_LEFT_SWERVE = 6
BACK_RIGHT_SWERVE = 7

ARM_0 = 8
ARM_1 = 9
ARM_2 = 10

CLAW = 13

CAM_X = 11
CAM_Y = 12
#I don't know how ports work. But it's a dictionary. So shouldn't matter.
#You can even call each motor a string. Whatever's easier to send to arduinos.

#####################
#  	    MODES       #
#####################
TANK_DRIVE = 0
PIVOT_SPIN = 1
STRAFE_DRIVE = 2

TIMEOUT = .600 #in s

#####################
#  	  Sliders       #
#####################
LEFT_SENSITIVITY = 0
RIGHT_SENSITIVITY = 1
ARM_0_SENSITIVITY = 3
ARM_1_SENSITIVITY = 4
ARM_2_SENSITIVITY = 5
CLAW_SENSITIVITY = 6

LEFT_SENSITIVITYA = 7
RIGHT_SENSITIVITYA = 8
ARM_0_SENSITIVITYA = 9
ARM_1_SENSITIVITYA = 10
ARM_2_SENSITIVITYA = 11
CLAW_SENSITIVITYA = 12

#####################
#  	 Controllers    #
#####################
BUTTON = 0
AXIS = 1
HAT = 2

##
#XBOX 360
##
#Buttons
A_BUTTON=0
B_BUTTON=1
X_BUTTON=2
Y_BUTTON=3
LEFT_BUTTON=4
RIGHT_BUTTON=5
BACK_BUTTON=6
START_BUTTON=7
L_ANALOG_BUTTON=8
R_ANALOG_BUTTON=9

#Axes
L_ANALOG_X= 0
L_ANALOG_Y=1 #(up is -1)
TRIGGER=2 #(positive max)
#R Trigger : 2 (negative max)
R_ANALOG_Y=3 #(up is -1)
R_ANALOG_X=4

#Hat :
HAT=0