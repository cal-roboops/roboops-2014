import serial
from time import sleep

portNames = ['/dev/ttyACM0', '/dev/ttyACM1', '/dev/ttyUSB0']
arduinoIDs = ["id_arm", "id_drive", "ARM_CONTROLLER"]

ARM_CW = 'q'
ARM_STOP = 'a'
ARM_CCW = 'z'

CLAW_CLOSE = 'w'
CLAW_STOP = 's'
CLAW_OPEN = 'x'

BASE_CW = 'e'
BASE_STOP = 'd'
BASE_CCW = 'c'

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

	def write(self, message):
		self.serial.write(message)

	def read(self):
		return self.serial.readline().strip()

if __name__ == "__main__":
	interface = Arduino("ARM_CONTROLLER")
	while True:
		interface.write("HI")
		print("arduino says", interface.read())
		sleep(1)
