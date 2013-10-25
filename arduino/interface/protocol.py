import serial

ser = serial.Serial(port='/dev/ttyACM0',timeout=3)

def on():
	ser.write('w')

def off():
	ser.write('s')

while True:
	on() if raw_input() == "w" else off()