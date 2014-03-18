##########################################
###### PYTHON 2B #########################
##########################################
#                                        #
# MARCO SANGALLI marco.sangalli@ovas.it  #
# ALEX RIGAMONTI alex.rigamonti@ovas.it  #
#                                        #
##########################################

from threading import Thread
import time
############################################
class inPort(Thread):
	def __init__(self,s,pin,callback=None,resolution=50.0):
		Thread.__init__(self)
		#
		self.ser=s
		self.pin=pin
		#resolution
		self.wait=1.0/float(resolution)
		#hold val
		self.hold=-1
		self.val=-1
		#alive status
		self.alive=True
		#set callback if passed
		self.callback=None
		if callback:
			self.setCallback(callback)
		#autostart
		self.start()
	
	def setCallback(self,callback):
		#register pin to callback
		self.callback=callback
		self.ser.addListener(self.pin,self.onData)
		
	def onData(self,pin,val):
		#@print pin,val
		self.val=val
		
	def run(self):
		while self.alive:
			if self.hold!=self.val:
				#hold val
				self.hold=self.val
				#callback
				if self.callback is not None:
					self.callback(self.val)
			time.sleep(self.wait)
	
	def destroy(self):
		self.alive=False
		
############################################
class outPort(Thread):
	def __init__(self,s,pin,resolution=50.0):
		Thread.__init__(self)
		#
		self.ser=s
		self.pin=pin
		#resolution
		self.wait=1.0/float(resolution)
		#hold val
		self.hold=-1
		self.val=-1
		#alive status
		self.alive=True
		#autostart
		self.start()
		
	def setValue(self,value):
		self.val=value

	def run(self):
		while self.alive:
			if self.hold!=self.val:
				#hold val
				self.hold=self.val
				#send to arduino
				self.ser.sendMessage(self.pin,self.val)
			time.sleep(self.wait)
				
	def destroy(self):
		self.alive=False

#FROM ma_protocol ##########################
# ANALOG INPUT:     6    (pin 0..5)	
# DIGITAL INPUT:    5    (pin 2..6)	
# DIGITAL OUTPUT:   4    (pin 7,8,12,13)	
# PWM:              3    (pin 9..11)	
#PORTS ######################################
AIN0=20
AIN1=21
AIN2=22
AIN3=23
AIN4=24
AIN5=25
#-------
DIN0=2
DIN1=3
DIN2=4
DIN3=5
DIN4=6
#-------
DOUT0=7
DOUT1=8
DOUT2=12
DOUT3=13
#--------
PWM0=9
PWM1=10
PWM2=11

