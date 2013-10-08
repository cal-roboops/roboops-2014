import time
from collections import deque
from constants import *
from Queue import Queue

class motorManager():
	def __init__(self, queue_in, queue_out) :
		self.motor_timeouts = {}
		self.motors = {}

		self.motor_timeouts[FRONT_LEFT_WHEEL] = time.time()
		self.motors[FRONT_LEFT_WHEEL_COMMAND] = FRONT_LEFT_WHEEL
		self.motors[FRONT_LEFT_WHEEL] = FRONT_LEFT_WHEEL_COMMAND
		self.motor_timeouts[FRONT_RIGHT_WHEEL] = time.time()
		self.motors[FRONT_RIGHT_WHEEL_COMMAND] = FRONT_RIGHT_WHEEL
		self.motors[FRONT_RIGHT_WHEEL] = FRONT_RIGHT_WHEEL_COMMAND
		self.motor_timeouts[BACK_LEFT_WHEEL] = time.time()
		self.motors[BACK_LEFT_WHEEL_COMMAND] = BACK_LEFT_WHEEL
		self.motors[BACK_LEFT_WHEEL] = BACK_LEFT_WHEEL_COMMAND
		self.motor_timeouts[BACK_RIGHT_WHEEL] = time.time()
		self.motors[BACK_RIGHT_WHEEL_COMMAND] = BACK_RIGHT_WHEEL
		self.motors[BACK_RIGHT_WHEEL] = BACK_RIGHT_WHEEL_COMMAND

		self.motor_timeouts[ARM_0] = time.time()
		self.motors[ARM_0_COMMAND] = ARM_0
		self.motors[ARM_0] = ARM_0_COMMAND
		self.motor_timeouts[ARM_1] = time.time()
		self.motors[ARM_1_COMMAND] = ARM_1
		self.motors[ARM_1] = ARM_1_COMMAND
		self.motor_timeouts[ARM_2] = time.time()
		self.motors[ARM_2_COMMAND] = ARM_2
		self.motors[ARM_2] = ARM_2_COMMAND

		self.motor_timeouts[CAM_X] = time.time()
		self.motors[CAM_X_COMMAND] = CAM_X
		self.motors[CAM_X] = CAM_X_COMMAND
		self.motor_timeouts[CAM_Y] = time.time()
		self.motors[CAM_Y_COMMAND] = CAM_Y
		self.motors[CAM_Y] = CAM_Y_COMMAND

		self.mode = TANK_DRIVE
		self.queue = deque()

		self.queue_in = queue_in
		self.queue_out = queue_out

		self.is_active = True

	def update(self) :
		self.resolve_queue()
		self.check_timeouts()

	def check_timeouts(self) :
		while(self.is_active):
			for motor in self.motor_timeouts :
				if self.motor_timeouts[motor] < time.time() :
					self.update_port(motor, 0)
					#print("Updated motor!")

	def read_inputs(self) :
		while(self.is_active) :
			self.parse_commands(self.queue_in.get())

	def parse_commands(self, string) :
		i, action, value, length = 0, "", "", len(string)
		while i < length :
			if string[i] == ':' :
				i += 1
				while i < length and string[i] != ';':
					value += string[i]
					i += 1
				self.update_command(str(action), float(value))
				action, value = "", ""
			else :
				action = string[i]
			i += 1

	def resolve_queue(self) :
		while len(self.queue) :
			t_action = self.queue.popleft()
			self.update_command(t_action[0], t_action[1])

	def update_command(self, key, value) :
		if key == MODE_COMMAND :
			self.mode = value
		elif key == TURN_COMMAND :
			#do turning, depending on mode
			self.queue_out.put("Turn here")
		elif key == LEFT_DRIVE_COMMAND :
			self.update_port(FRONT_LEFT_WHEEL, value)
			self.update_port(BACK_LEFT_WHEEL, value)
		elif key == RIGHT_DRIVE_COMMAND :
			self.update_port(FRONT_RIGHT_WHEEL, value)
			self.update_port(BACK_RIGHT_WHEEL, value)
		elif key in self.motors :
			self.update_port(self.motors[key], value)
		else :
			self.queue_out.put("COMMAND NOT RECOGNIZED YOU DIMWIT")
			self.is_active = False

	def update_port(self,port, value) :
		if value :
			self.motor_timeouts[port] = time.time() + TIMEOUT
		else :
			self.motor_timeouts[port] = time.time() + 300
		self.queue_out.put(self.motors[port] + ":" + str(value) + ";")
		#send power to motors

	def shut_off(self) :
		self.is_active = False

def main():
	r = motorManager(Queue(), Queue())
	r.parse_commands("lel")

if __name__ == "__main__" :
	main()