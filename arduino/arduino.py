import serial
from time import sleep

portNames = ['/dev/ttyACM0', '/dev/ttyACM1', '/dev/ttyUSB0']
arduinoIDs = ["id_arm", "id_drive", "ARM_CONTROLLER"]

class Arduino:

	def __init__(self, uid):
		for port in portNames:
			try:
				arduino = serial.Serial(port, 9600)
				idString = arduino.readline().strip()
				print(idString, uid)
				if idString == uid:
					arduino.write(idString)
					self.serial = arduino
			except Exception as e:
				print(e)
			sleep(0.2)

	def write(self, message, arduino):
		self.serial.write(message)

	def read(self, arduino):
		return self.serial.readline().strip()

if __name__ == "__main__":
	interface = Arduino("ARM_CONTROLLER")
	while True:
		interface.write("HI", "ARM_CONTROLLER")
		print("arduino says", interface.read("ARM_CONTROLLER"))
		sleep(1)