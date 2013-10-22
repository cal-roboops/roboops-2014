from SimpleCV import Camera, Display, Color, JpegStreamer
import time

def getColorBlobList(image, color):
    distanceFromColor = image.colorDistance(color)
    resultingImg = image - distanceFromColor
    return resultingImg.findBlobs()


#size of display
displayX = 640;
displayY = 480;

# Initialize the camera
cam = Camera()
display = Display((displayX, displayY))
imgStreamer = JpegStreamer('10.142.5.126:8000')
print imgStreamer.server

# Loop to continuously get images
while display.isNotDone():
    # Get Image from camera
    origImg = cam.getImage()
    img = origImg.dilate(10)

    blobListOrange = getColorBlobList(img, Color.ORANGE)
    blobListBlue = getColorBlobList(img, Color.BLUE)
    blobListPurple = getColorBlobList(img, Color.VIOLET)
    blobListYellow = getColorBlobList(img, Color.YELLOW)
    blobListGreen = getColorBlobList(img, Color.GREEN)
    blobListRed = getColorBlobList(img, Color.RED)

    blobLists = ((blobListOrange, 'O'),
                 (blobListBlue, 'B'),
                 (blobListPurple, 'P'),
                 (blobListYellow, 'Y'),
                 (blobListGreen, 'G'),
                 (blobListRed, 'R'))

##    for blobList in blobLists:
##        if(blobList[0] is not None):
##            origImg.drawText(blobList[1], blobList[0][-1].x, blobList[0][-1].y)

            
##            for blob in blobList[0]:
##                #img.drawText(blobList[1], blob.x, blob.y)
##                origImg.drawText(blobList[1], blob.x, blob.y)

##    if(blobList is not None):
##        img.drawText("O", blobList[-1].x, blobList[-1].y)
    #img.drawText("X", displayX / 2, displayY / 2)
                
    #img.save(display)
    
    origImg.drawCircle((displayX / 2, displayY / 2), 50, Color.GREEN, 5)
    origImg.save(display)
    origImg.save(imgStreamer)
    time.sleep(0.1)
cam.stop()



