import serial
from time import sleep

#todo: make smarter decision
portNames = {};
portNames["ARM_CONTROLLER"] = 'blah'
portNames["DRIVE_CONTROLLER"] = 'lel'


class Arduino:

	def __init__(self, port):
		try :
			self.serial = serial.Serial(port, 9600)
		except serial.SerialException:
			pass

	def write(self, message):
		#self.serial.write(message)
		pass

	def read(self):
		#return self.serial.readline().strip()
		return "lel"

if __name__ == "__main__":
	interface = Arduino("ARM_CONTROLLER")
	while True:
		interface.write("HI")
		print("arduino says", interface.read())
		sleep(1)
