import serial
from time import sleep

#todo: make smarter decision
portNames = {};
portNames["ARM_CONTROLLER"] = '/dev/ttyACM1'
portNames["DRIVE_CONTROLLER"] = 'lel'


class Arduino:

	def __init__(self, port):
		try :
			self.serial = serial.Serial(port, 9600)
			self.active = True
		except serial.SerialException:
			self.serial = None
			self.active = False

	def write(self, message):
		self.serial.write(message)

	def read(self):
		return self.serial.readline().strip()

	def read(self):
		return self.serial.readline()

if __name__ == "__main__":
	interface = Arduino(portNames["ARM_CONTROLLER"])
	while True:
		x=raw_input("hey: ")
		interface.write(x)
		print("arduino says", interface.read())

		sleep(1)
