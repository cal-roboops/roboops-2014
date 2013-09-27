import time
from collections import deque                 

#####################
#  		PORTS       #
#####################
FRONT_LEFT_WHEEL = 0
FRONT_RIGHT_WHEEL = 1
BACK_LEFT_WHEEL = 2
BACK_RIGHT_WHEEL = 3

ARM_0 = 4
ARM_1 = 5
ARM_6 = 6

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

TIMEOUT = 300 #in ms

class commandParser():
	def __init__(self):
		return;

	def parse_command_to(self, string, motor_man):
		i, action, value, length = 0, "", "", len(string)
		while i < length :
			if string[i] == ':' :
				i += 1
				while i < length and string[i] != ';':
					value += string[i]
					i += 1
				value = float(value)
				motor_man.queue_command(action, value)
				action, value = "", ""
			else :
				action = string[i]
			i += 1

class motorManager():
	motorList = {}
	mode = TANK_DRIVE
	queue = None

	def __init__(self) :
		self.motorList = {}

		self.motorList[FRONT_LEFT_WHEEL] = time.time()
		self.motorList[FRONT_RIGHT_WHEEL] = time.time()
		self.motorList[BACK_LEFT_WHEEL] = time.time()
		self.motorList[BACK_RIGHT_WHEEL] = time.time()


		self.motorList[ARM_0] = time.time()
		self.motorList[ARM_0] = time.time()
		self.motorList[ARM_0] = time.time()


		self.motorList[CAM_X] = time.time()
		self.motorList[CAM_Y] = time.time()

		self.mode = TANK_DRIVE
		self.queue = deque()

	def update(self) :
		for motor_name in self.motorList :
			motor = self.motorList[motor_name]
			if motor[1] > time.time() :
				motor[0] = 0
				#set comamnd to 0
				#send communications to gui + motors

	def resolve_queue(self) :
		while len(self.queue) :
			t_action = self.popleft()
			update_command(t_action[0], t_action[1])

	def queue_command(self, key, value) :
		self.queue.append((key, value))

	def update_command(self, key, value) :
		if key == MODE_COMMAND :
			mode = value
		elif key == TURN_COMMAND :
			#do turning shit, I don't know. Something depending on the mode, I guess.
			print("HEY PYTHON LET ME PUT EMPTY IFS PLEASE YOU FUCKING BASTARD")
		elif key == LEFT_DRIVE_COMMAND :
			update_port(FRONT_LEFT_WHEEL, value)
			update_port(BACK_LEFT_WHEEL, value)
		elif key == RIGHT_DRIVE_COMMAND :
			update_port(FRONT_RIGHT_WHEEL, value)
			update_port(BACK_RIGHT_WHEEL, value)
		elif key in self.motorList :
			update_port(port, value)
		else :
			print("COMMAND NOT RECOGNIZED YOU DIMWIT")

	def update_port(self,port, value) :
		self.motorList[port][1] = time.time()

		#send power to motrs
		#send communication back to us, updating us on values

def test_me():
	cP = commandParser()
	mm = motorManager()
	cP.parse_command_to("A:100;B:200;C:300;T:400", mm)
