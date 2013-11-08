#!/usr/bin/python
'''
The rover (s) should be capable of selectively picking up irregularly shaped rocks
with maximum diameters ranging from 2 to 8 cm and masses ranging from 20 to 150 gm. 
The rover(s) must be capable of storing the rocks on the rover and/or daughter bots 
and transporting them throughout the course. The rover must also be capable of determining 
the color (red, purple, blue, green, yellow, or orange) of each of the rocks.
'''
from SimpleCV import *
import sys, time, socket

# In the default Colors, orange and red are too similar, and violet doesn't match the purple 
# rock in the example pic. Can we assume that if we hard-code in based on the sample image
# that colors will appear the same way IRL?

# cam = Camera() # I can't access my camera but I'm going to assue this works...

colors = {Color.RED:"red", Color.YELLOW:"yellow", Color.BLUE:"blue", Color.VIOLET:"purple", Color.GREEN:"green", Color.ORANGE:"orange"}

def findColorBlob(img, color):
	# identify and outline a colored rock in an image
	justColor = img - img.colorDistance(color)
	blobs = justColor.findBlobs()
	if blobs:
		# To do: put in check for a reasonably sized blob. We don't want to return specks or the background/sky.
		return blobs[-1] # largest blob
	else:
		print "No {0} blobs found".format(color)

def run():
	# modified copypaste from http://help.simplecv.org/question/42/finding-specific-color-circular-blobs-in-hsv-color-space/
	while True:
	   video = cam.getImage()
	   for color in colors:
	   	blob = findColor(video, color)
	   	if blob:
	   		blob.show()

	   blobs = video.findBlobs()
	   for b in blobs:
	      if b.isCircle(.1): b.draw()
	   video.show() #display the image
	   time.sleep(0.001) # Let the program sleep for 1 millisecond so the computer can do other things

def test():
	filename = sys.argv[1]
	# testImg = Image("officialRocks.JPG")
	testImg = Image(filename)
	for color in colors:
		print colors[color]
		b = findColorBlob(testImg, color)
		b.show()
		time.sleep(2)

test()