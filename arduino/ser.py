##########################################
###### PYTHON 2B #########################
##########################################
#                                        #
# MARCO SANGALLI marco.sangalli@ovas.it  #
# ALEX RIGAMONTI alex.rigamonti@ovas.it  #
#                                        #
##########################################
##########################################

from serial import Serial
from threading import Thread
import time

class ser(Thread):
	def __init__(self):
		Thread.__init__(self)
		#port
		self.port=-1
		self.maxports=3	
		self.s=None
		#sync flag
		self.sync=False
		#sync byte
		self.syncbyte=100;
		#minimum sync count
		self.synccount=5;
		#listeners
		self.listeners=[]
		#reset
		self.reset()
		#start
		self.start()
		
	#try different port to connect to	
	def connect(self):
		#resync
		self.sync=False
		#close serial
		if self.s:
			self.s.close()
		#choose a different port 0..self.maxports
		self.port+=1
		if self.port>self.maxports:
			self.port=0
		p="/dev/ttyUSB%s" % self.port
		#try to connect
		try:
			self.s = Serial(p,19200,timeout=1)
		except:
			print "PORT ERROR %s>>>" % p
			time.sleep(1)
		else:
			print "CONNECTED TO %s>>>" % p

	#thread loop
	def run(self):
		while 1:
			try:
				#send queue message################
				res=self.s.read(2)
				#read reply 2 byte PIN|VALUE#######
				pin=ord(res[0])
				val=ord(res[1])		
			except:
				self.connect()
			else:
				#dispatch values ##################
				#@@print "[%s:%s]" % (pin,val)
				#sync byte ########################
				if pin==self.syncbyte:
					#print "SYNC BYTE DETECTED>>>"
					#resync on signal
					if val==self.syncbyte:
						#decrement sync counter
						if self.synccount>0:
							self.synccount-=1
						elif not self.sync:
							print "SYNCHRONIZED>>>"
							self.sync=True
					else:
					 	print "SHIFTED>>>"
						res=self.s.read(1)
					#send back dsync signal
					self.sendMessage(self.syncbyte,self.syncbyte)
				elif self.sync:
					#send to listeners
					#print "TO LISTENERS>>>"
					self.__toListeners(val,pin)
			#little sleep
			time.sleep(0.001)
		
	#private method to dispatch	
	def __toListeners(self,val,pin):
		for listener in self.listeners:
			if listener["pin"]==pin:
				#value to controller
				listener["cls"](pin,val)
		
	#send coneverted message to device	
	def sendMessage(self,pin,value):
		if value>=0 and value<=255 and self.sync:
			#convert to bytes
			msg="%s%s" % (chr(pin),chr(value))	
			try:
				self.s.write(msg)
			except:
				print "UNABLE TO WRITE>>>"
				self.connect()
			
	#reset input enable status to 0
	def reset(self):
		self.sendMessage(self.syncbyte,self.syncbyte)
		
	#add new listener
	def addListener(self,pin,cls):
		self.listeners.append({"pin":pin,"cls":cls})
		

if __name__ == "__main__":
	s=ser()	
		
