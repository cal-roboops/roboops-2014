import time
from collections import deque
from constants import *
from Queue import Queue
from serialize import Serialize
import arduino.arduino as arduino

class motorManager():
	def __init__(self, queue_in, queue_out) :
		self.motor_timeouts = {}
		self.motors = {}
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

		self.motor_timeouts[CAM_X] = time.time()
		self.motor_timeouts[CAM_Y] = time.time()

		self.mode = TANK_DRIVE
		self.queue = deque()

		armArduino = arduino.Arduino("ARM_CONTROLLER")
		#driveArduino = arduino.Arduino("id_drive")

		self.motors[ARM_0] = armArduino
		self.motors[ARM_1] = armArduino
		self.motors[ARM_2] = armArduino

		self.motors[FRONT_LEFT_WHEEL] = driveArduino
		self.motors[BACK_LEFT_WHEEL] = driveArduino
		self.motors[FRONT_RIGHT_WHEEL] = driveArduino
		self.motors[BACK_RIGHT_WHEEL] = driveArduino

		self.motors[FRONT_LEFT_SWERVE] = driveArduino
		self.motors[BACK_LEFT_SWERVE] = driveArduino
		self.motors[FRONT_RIGHT_SWERVE] = driveArduino
		self.motors[BACK_RIGHT_SWERVE] = driveArduino

		self.motors[CAM_X] = driveArduino
		self.motors[CAM_Y] = driveArduino

		self.translations[ARM_0] = lambda x : arduino.ARM_CW if x > 0 else (arduino.ARM_CCW if x < 0 else arduino.ARM_STOP)
		self.translations[ARM_1] = lambda x : arduino.CLAW_OPEN if x > 0 else (arduino.CLAW_CLOSE if x < 0 else arduino.CLAW_STOP)
		self.translations[ARM_2] = lambda x : arduino.BASE_CW if x > 0 else (arduino.BASE_CCW if x < 0 else arduino.BASE_STOP)

		self.translations[FRONT_LEFT_WHEEL] = lambda x : x
		self.translations[BACK_LEFT_WHEEL] = lambda x : x
		self.translations[FRONT_RIGHT_WHEEL] = lambda x : x
		self.translations[BACK_RIGHT_WHEEL] = lambda x : x

		self.translations[FRONT_LEFT_SWERVE] = lambda x : x
		self.translations[BACK_LEFT_SWERVE] = lambda x : x
		self.translations[FRONT_RIGHT_SWERVE] = lambda x : x
		self.translations[BACK_RIGHT_SWERVE] = lambda x : x

		self.translations[CAM_X] = lambda x : x
		self.translations[CAM_Y] = lambda x : x


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

	def read_inputs(self) :
		while(self.is_active) :
			Serialize.run_robot(self.queue_in.get(), self)

	def update_port(self, port, value) :
		if value :
			self.motor_timeouts[port] = time.time() + TIMEOUT
		else :
			self.motor_timeouts[port] = time.time() + 30
		self.queue_out.put(Serialize.Motor(port, value).dump())
		if(port == ARM_0 or port == ARM_1 or port == ARM_2):
			self.motors[port].write(self.translations[port](value))
			print(self.motors[port].read())

	def shut_off(self) :
		self.is_active = False

def main():
	r = motorManager(Queue(), Queue())

if __name__ == "__main__" :
	main()
