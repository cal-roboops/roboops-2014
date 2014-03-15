import time
from collections import deque
from constants import *
from Queue import Queue
from serialize import Serialize
import arduino.arduino as arduino

class motorManager():
	def __init__(self, queue_in, queue_out, arm_port, drive_port) :
		self.motor_timeouts = {}
		self.arduino = {}
		self.translations = {}

		self.motor_timeouts[FRONT_LEFT_WHEEL] = time.time()
		self.motor_timeouts[FRONT_RIGHT_WHEEL] = time.time()
		self.motor_timeouts[BACK_LEFT_WHEEL] = time.time()
		self.motor_timeouts[BACK_RIGHT_WHEEL] = time.time()

		self.motor_timeouts[FRONT_LEFT_SWERVE] = time.time()
		self.motor_timeouts[FRONT_RIGHT_SWERVE] = time.time()
		self.motor_timeouts[BACK_LEFT_SWERVE] = time.time()
		self.motor_timeouts[BACK_RIGHT_SWERVE] = time.time()

		self.motor_timeouts[ARM_0] = time.time()
		self.motor_timeouts[ARM_1] = time.time()
		self.motor_timeouts[ARM_2] = time.time()
		self.motor_timeouts[CLAW] = time.time()

		self.motor_timeouts[CAM_X] = time.time()
		self.motor_timeouts[CAM_Y] = time.time()

		self.mode = TANK_DRIVE
		self.queue = deque()

		armArduino = arduino.Arduino(arm_port)
		driveArduino = arduino.Arduino(drive_port)

		self.arduino[ARM_0] = armArduino
		self.arduino[ARM_1] = armArduino
		self.arduino[ARM_2] = armArduino
		self.arduino[CLAW] = armArduino

		self.arduino[FRONT_LEFT_WHEEL] = driveArduino
		self.arduino[BACK_LEFT_WHEEL] = driveArduino
		self.arduino[FRONT_RIGHT_WHEEL] = driveArduino
		self.arduino[BACK_RIGHT_WHEEL] = driveArduino

		self.arduino[FRONT_LEFT_SWERVE] = driveArduino
		self.arduino[BACK_LEFT_SWERVE] = driveArduino
		self.arduino[FRONT_RIGHT_SWERVE] = driveArduino
		self.arduino[BACK_RIGHT_SWERVE] = driveArduino

		self.arduino[CAM_X] = driveArduino
		self.arduino[CAM_Y] = driveArduino

		self.queue_in = queue_in
		self.queue_out = queue_out

		self.is_active = True

	def update(self) :
		while(self.is_active):
			Serialize.run_robot(self.queue_in.get(), self)
			for motor in self.motor_timeouts :
				if self.motor_timeouts[motor] < time.time() :
					self.update_port(motor, 0)
			time.sleep(0.1)

	def check_timeouts(self) :
		while(self.is_active):
			for motor in self.motor_timeouts :
				if self.motor_timeouts[motor] < time.time() :
					self.update_port(motor, 0)
			time.sleep(0.1)

	def read_inputs(self) :
		while(self.is_active) :
			Serialize.run_robot(self.queue_in.get(), self)

	def update_port(self, port, value) :
		if value :
			self.motor_timeouts[port] = time.time() + TIMEOUT
		else :
			self.motor_timeouts[port] = time.time() + 30
		self.queue_out.put(Serialize.Motor(port, value).dump())
		if(self.arduino[port].active) :
			self.arduino[port].write(self.translate(port, value))
			print(self.arduino[port].read())

	def translate(self, port, value) :
		return str(port).zfill(2) + ((";1;" + str(-value).zfill(3)) if value < 0 else (";0;" + str(value).zfill(3))) + "!"

	def shut_off(self) :
		self.is_active = False

def main():
	r = motorManager(Queue(), Queue())

if __name__ == "__main__" :
	main()
