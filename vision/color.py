from SimpleCV import Image, Color

target = (4.683625, 3.6336250000000003, 5.031350000000001)
#target = Color.YELLOW
image = Image("parking-car.png")
targetDistance = image.hueDistance(target).invert()
blobs = targetDistance.findBlobs()
blobs.draw(color=Color.HOTPINK,width=3)
targetDistance.show()
image.addDrawingLayer(targetDistance.dl())

i = 0
while  i != 100:
	image.show()
	i +=1