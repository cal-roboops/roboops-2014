import serial
from time import sleep

ser = serial.Serial(port='/dev/ttyACM0',timeout=3)
portNames = ['/dev/ttyACM0', '/dev/ttyACM1']
arduinoIDs = ["id_arm", "id_drive"]

class serialPorts(object):
	"""docstring for serialPorts"""
	def __init__(self):
		self.arduinos = {}
		for port in portNames:
			try:
				arduino = serial.Serial(device, 9600)
				arduino.write("id")
				sleep(0.2)
				idString = arduino.read(100)
				if idString in arduinoIDs:
					self.arduinos[idString] = arduino
			except:
				pass
	def write(self, message, arduino):
		self.arduinos[arduino].write(message)
	def read(self, message, arduino):
		self.arduinos[idString] = arduino
		
		