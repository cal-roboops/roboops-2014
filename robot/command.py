from collections import deque
from constants.constants import *
from Queue import Queue
from serialize import Serialize
import arduino.arduino as arduino
from time import sleep

class motorManager():
	def __init__(self, queue_in, queue_out, arm_port, drive_port) :
		self.arduino = {}
		self.translations = {}

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

	def emergency_stop(self) :
		for port in self.arduino:
			self.update_port(port, 0)

	def read_inputs(self) :
		while(self.is_active) :
			Serialize.run_robot(self.queue_in.get(), self)


	def update_port(self, port, value) :
		self.queue_out.put(Serialize.Motor(port, value).dump())
		if(self.arduino[port].active) :
			print("Attempting to write: " + self.translate(port, value) + " to arduino.")
			try:
				self.arduino[port].write(self.translate(port, value))
				print("Wrote to arduino! Reading from arduino: ")
				#print(self.arduino[port].read())
			except Exception as e:
				print("Error! " + e)
		else :
			print("Arduino not active!")

	def translate(self, port, value) :
		return str(port).zfill(2) + ((";1;" + str(-int(value)).zfill(3)) if value < 0 else (";0;" + str(int(value)).zfill(3))) + "!"

	def shut_off(self) :
		self.is_active = False

def main():
	r = motorManager(Queue(), Queue())

if __name__ == "__main__" :
	main()
