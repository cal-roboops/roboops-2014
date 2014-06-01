import serial
from time import sleep

#todo: make smarter decision
portNames = {};
portNames["ARM_CONTROLLER"] = '/dev/ttyACM1'
portNames["DRIVE_CONTROLLER"] = 'lel'


class Arduino:

	def __init__(self, port):
		try :
			self.serial = serial.Serial(port, 9600, timeout=0.1)
			self.active = True
		except Exception as e:
			try:
				self.serial = serial.Serial(int(port), 9600, timeout=0.1)
				self.active = True
			except:
				print("Arduino for port " + port + " failed")
				print(e)
				self.serial = None
				self.active = False

	def write(self, message):
		self.serial.write(message)
		self.serial.flush()

	def read(self):
		return self.serial.readline().strip()

	def read(self):
		return self.serial.readline()

        def close(self):
                return self.serial.close()

        def open(self):
                return self.serial.open()

if __name__ == "__main__":
	interface = Arduino(portNames["ARM_CONTROLLER"])
	n=0;
	while n<100:
		interface.write("00;0;000!")
		print("arduino says", interface.read())
