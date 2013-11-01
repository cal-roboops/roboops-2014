import time
from collections import deque
from constants import *
from Queue import Queue
from serialize import Serialize

class motorManager():
	def __init__(self, queue_in, queue_out) :
		self.motor_timeouts = {}
		self.motors = {}

		self.motor_timeouts[FRONT_LEFT_WHEEL] = time.time()
		self.motor_timeouts[FRONT_RIGHT_WHEEL] = time.time()
		self.motor_timeouts[BACK_LEFT_WHEEL] = time.time()
		self.motor_timeouts[BACK_RIGHT_WHEEL] = time.time()

		self.motor_timeouts[ARM_0] = time.time()
		self.motor_timeouts[ARM_1] = time.time()
		self.motor_timeouts[ARM_2] = time.time()

		self.motor_timeouts[CAM_X] = time.time()
		self.motor_timeouts[CAM_Y] = time.time()

		self.mode = TANK_DRIVE
		self.queue = deque()

		self.queue_in = queue_in
		self.queue_out = queue_out

		self.is_active = True

	def update(self) :
		self.read_inputs()
		self.check_timeouts()

	def check_timeouts(self) :
		while(self.is_active):
			for motor in self.motor_timeouts :
				if self.motor_timeouts[motor] < time.time() :
					self.update_port(motor, 0)
					#print("Updated motor!")

	def read_inputs(self) :
		while(self.is_active) :
			Serialize.run_robot(self.queue_in.get(), self)

	def update_port(self, port, value) :
		if value :
			self.motor_timeouts[port] = time.time() + TIMEOUT
		else :
			self.motor_timeouts[port] = time.time() + 300
		a = Serialize.Motor(port, value).dump()
		self.queue_out.put(a)

	def shut_off(self) :
		self.is_active = False

def main():
	r = motorManager(Queue(), Queue())

if __name__ == "__main__" :
	main()
