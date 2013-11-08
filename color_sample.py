from SimpleCV import Color, Image


COLORS = {Color.RED:"red", Color.YELLOW:"yellow", Color.BLUE:"blue", Color.VIOLET:"purple", Color.GREEN:"green", Color.ORANGE:"orange"}

def my_show(image, length=60):
	i = 0
	while i != length:
		image.show()
		i += 1

def halfsies(left,right): 
	return left.sideBySide(right)

def findColorBlob(img, color):
	# identify and outline a colored rock in an image
	justColor = img - img.colorDistance(color)
	blobs = justColor.findBlobs()
	if blobs:
		# To do: put in check for a reasonably sized blob. We don't want to return specks or the background/sky.
		return blobs[-1] # largest blob
	else:
		print "No {0} blobs found".format(color)




if __name__ == "__main__":
	#img = Image("mm.jpg")
	img = Image("rocks.png")
	for color in COLORS:
		print COLORS[color]
		color_distance = img - img.colorDistance(color)
		blobs = color_distance.findBlobs()[-1] #only biggest one
		half = halfsies(img,color_distance.binarize(10).invert().erode(1).dilate(1)) 
			#http://www.reedbushey.com/65Practical%20Computer%20Vision%20with%20Simplecv.pdf
			#http://my.safaribooksonline.com/book/programming/python/9781449337865/image-morphology/id2749011
		my_show(half,length=70)
		# img.addDrawingLayer(color_distance.dl())
		# my_show(color_distance.binarize(),length=50)


	# filename = sys.argv[1]
	# # testImg = Image("officialRocks.JPG")
	# testImg = Image(filename)
	# for color in colors:
	# 	print colors[color]
	# 	b = findColorBlob(testImg, color)
	# 	b.show()
	# 	time.sleep(2)


	# blue_distance = img.colorDistance(Color.BLUE).invert()  
	# blobs = blue_distance.findBlobs()  
	# blobs.draw(color=Color.HOTPINK, width=5)  
	# blue_distance.show()
	# img.addDrawingLayer(blue_distance.dl()) 
	# my_show(img)
	# my_show(blue_distance.binarize(), length=1000)

