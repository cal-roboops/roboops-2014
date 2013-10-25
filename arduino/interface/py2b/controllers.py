##########################################
###### PYTHON 2B #########################
##########################################
#                                        #
# MARCO SANGALLI marco.sangalli@ovas.it  #
# ALEX RIGAMONTI alex.rigamonti@ovas.it  #
#                                        #
##########################################

from ser import *
from threading import Thread
from portManager import *
import time
import math

#############################################
class pwmTest(Thread):
	def __init__(self,s,pin,resolution=10):
		Thread.__init__(self)
		#resolution
		self.wait=1.0/float(resolution)
		#
		self.pwmport=outPort(s,pin)		
		#cnt
		self.cnt=0.0
		self.maxpwm=255.0	
		#start		
		self.start()

	def run(self):
		while 1:
			self.cnt+=0.05
			#pass
			val=abs(int(math.sin(self.cnt)*self.maxpwm))
			#print "%s=========>" % (val)			
			#10 values at time
			self.pwmport.setValue(val)
			time.sleep(self.wait)
			
################################################
class analogPort(object):
	def __init__(self,s,pinIn,pinOut):
		self.portIn=inPort(s,pinIn,self.onData)
		self.portOut=outPort(s,pinOut)

	def onData(self,val):
		#@@print "A%s" % val
		self.portOut.setValue(val)
			
################################################
class digitalPort(object):
	def __init__(self,s,pinIn,pinOut):
		self.portIn=inPort(s,pinIn,self.onData)
		self.portOut=outPort(s,pinOut)

	def onData(self,val):
		#@@print "D%s" % val
		self.portOut.setValue(val)
		
#start serial manager####################
if __name__ == "__main__":
	s=ser()
	#DIGITAL
	dp=digitalPort(s,DIN0,DOUT2)
	#PWM
	pwm=pwmTest(s,PWM1,13)
	#ANALOG
	ap=analogPort(s,AIN1,PWM2)
