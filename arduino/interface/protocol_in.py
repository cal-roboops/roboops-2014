import serial

ser = serial.Serial(port='/dev/ttyACM0',timeout=3)

while True:
	print ser.read(100) #reading up to 100 bytes
	input()