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

class motorManager():
	def __init__(self) :
		self.motor_list = {}
		self.com_to_mot_map = {}

		self.motor_list[FRONT_LEFT_WHEEL] = time.time()
		self.motor_list[FRONT_RIGHT_WHEEL] = time.time()
		self.motor_list[BACK_LEFT_WHEEL] = time.time()
		self.motor_list[BACK_RIGHT_WHEEL] = time.time()

		self.motor_list[ARM_0] = time.time()
		self.motor_list[ARM_1] = time.time()
		self.motor_list[ARM_2] = time.time()

		self.motor_list[CAM_X] = time.time()
		self.motor_list[CAM_Y] = time.time()

		self.com_to_mot_map[FRONT_LEFT_WHEEL_COMMAND] = FRONT_LEFT_WHEEL
		self.com_to_mot_map[FRONT_RIGHT_WHEEL_COMMAND] = FRONT_RIGHT_WHEEL
		self.com_to_mot_map[BACK_LEFT_WHEEL_COMMAND] = BACK_LEFT_WHEEL
		self.com_to_mot_map[BACK_RIGHT_WHEEL_COMMAND] = BACK_RIGHT_WHEEL

		self.com_to_mot_map[ARM_0_COMMAND] = ARM_0
		self.com_to_mot_map[ARM_1_COMMAND] = ARM_1
		self.com_to_mot_map[ARM_2_COMMAND] = ARM_2

		self.com_to_mot_map[CAM_X_COMMAND] = CAM_X
		self.com_to_mot_map[CAM_Y_COMMAND] = CAM_Y

		self.mode = TANK_DRIVE
		self.queue = deque()

	def update(self) :
		for motor_name in self.motor_list :
			motor = self.motor_list[motor_name]
			if motor[1] > time.time() :
				motor[0] = 0
				#set comamnd to 0
				#send communications to gui + motors

	def parse_commands(self, string) :
		i, action, value, length = 0, "", "", len(string)
		while i < length :
			if string[i] == ':' :
				i += 1
				while i < length and string[i] != ';':
					value += string[i]
					i += 1
				value = float(value)
				self.queue.append((action, float(value)))
				action, value = "", ""
			else :
				action = string[i]
			i += 1

	def resolve_queue(self) :
		while len(self.queue) :
			t_action = self.popleft()
			update_command(t_action[0], t_action[1])

	def update_command(self, key, value) :
		if key == MODE_COMMAND :
			mode = value
		elif key == TURN_COMMAND :
			#do turning, depending on mode
			print("Turn here")
		elif key == LEFT_DRIVE_COMMAND :
			update_port(FRONT_LEFT_WHEEL, value)
			update_port(BACK_LEFT_WHEEL, value)
		elif key == RIGHT_DRIVE_COMMAND :
			update_port(FRONT_RIGHT_WHEEL, value)
			update_port(BACK_RIGHT_WHEEL, value)
		elif key in self.com_to_mot_map :
			update_port(self.com_to_mot_map[key], value)
		else :
			print("COMMAND NOT RECOGNIZED YOU DIMWIT")

	def update_port(self,port, value) :
		self.motor_list[port][1] = time.time()

		#send power to motors
		#send communication back to us, updating us on values

def test_me():
	mm = motorManager()
	mm.parse_command("A:100;B:200;C:300;T:400")
