#Before running the program, first go to the command line
#and run the command "python -m SimpleHTTPServer" (without quotes)
from SimpleCV import Camera, Display, Color, JpegStreamer, Image, DrawingLayer
import pygame

# Display settings
displayX = 1280
displayY = 480
display = Display((displayX, displayY))

# Initialize the camera
cam = Camera()

print cam.getAllProperties()

snapshotTaken = False
snapshot = cam.getImage()

doImageProcessing = False

# Change the IP address to match the IPv4 address (use ipconfig in cmd)
imgStreamer = JpegStreamer('10.142.4.72:8000')

########################################
#Howard's Code starts here (with edits)#
########################################

primColors = [Color.RED, Color.GREEN, Color.BLUE]
compColors = [Color.CYAN, Color.YELLOW, (255,0,255)]

#To check progress quickly
def my_show(camImage, loadedImage, length=60):
        i = 0
        while i != length:
                #camImage.sideBySide(loadedImage).save(display)
                #camImage.sideBySide(loadedImage).save(imgStreamer)
                loadedImage.save(display)
                loadedImage.save(imgStreamer)
                i += 1

def halfsies(left,right): 
    result = left
    # crop the right image to be just the right side.
    crop = right.crop(right.width/2.0,0,right.width/2.0,right.height)
    # now paste the crop on the left image.
    result = result.blit(crop,(left.width/2,0))
    # return the results.
    return result

def color_filter(camImage, image, targetColor):
        """Returns the difference between the image and image relative to target"""
        temp = image - image.colorDistance(targetColor)
        my_show(camImage, temp)
        return temp


def get_largestBlobs(image):
        """Returns the largest blobs by area, assumes that we want largest blobs"""
        return sorted(image.findBlobs(), key=lambda blob: blob.area(), reverse=True)

def diff(tup1, tup2, op="wpy"):
        """Returns difference between color tuples"""
        if len(tup1) != 3 or len(tup2) != 3:
                print tup1
                print tup2
                return "Invalid colors"
        if op == "naive":
                return abs(tup1[0]-tup2[0]) + abs(tup1[1]-tup2[1]) + abs(tup1[2]-tup2[2])
        if op == "py":
                return pow(tup1[0]-tup2[0], 2) + pow(tup1[1]-tup2[1],2) + pow(tup1[2]-tup2[2],2)
        if op == "wpy":
                return pow(tup1[0]-tup2[0], 2)*.3 + pow(tup1[1]-tup2[1],2)*.6 + pow(tup1[2]-tup2[2],2)*1


def calc_radius(area):
        return pow(area / 3.1415, .5)

def idByColor(image, targetColor, blob_list, num=5):
        """Circles ROI on the image based on similarity to target color"""
        cen_rad_list = []
        bestCentroid =  (0,0)
        bestColor = targetColor
        curr_diff = 999999999999
        for i in range(min(num, len(blob_list))):
                blob = blob_list[i]
                cen_rad_list.append((blob.centroid(), calc_radius(blob.area())))
                blob_diff = diff(targetColor, blob.meanColor())
                if blob_diff <= curr_diff:
                        bestCentroid = blob.centroid()
                        bestColor = blob.meanColor()

        print bestCentroid
        for c,r in cen_rad_list:
                if c == bestCentroid:
                        image.drawCircle(c,2*r,color=Color.HOTPINK, thickness=min(r,20))
                else:
                        image.drawCircle(c,2*r,color=Color.GREEN, thickness=min(r,10))
        return image, bestColor


######################
#End of Howard's code#
######################

# Loop to continuously get images
while display.isNotDone():   
    # Get Image from camera
    origImg = cam.getImage()

    if snapshotTaken:
        if doImageProcessing:
            #More of Howard's code (with edits)#
            target = Color.BLUE
            loaded = Image('F:\stuff\img.png')
            my_show(origImg, loaded)
            cf = color_filter(origImg, loaded, target)
            blobs = get_largestBlobs(cf)
            final, nextColor = idByColor(cf,target,blobs)
            my_show(origImg, final,length=70)


            #final = idByColor(cf,target,blobs)
            #origImg.sideBySide(final).save(display)
            #origImg.sideBySide(final).save(imgStreamer)
            #edgeMap = snapshot.edges()

            #origImg.sideBySide(edgeMap).save(display)
            #origImg.sideBySide(edgeMap).save(imgStreamer)

        else:
            origImg.sideBySide(snapshot).save(display)
            origImg.sideBySide(snapshot).save(imgStreamer)
    else:
        origImg.save(display)
        origImg.save(imgStreamer)

    # Take a snapshot and save it when you press Return (aka Enter)
    # Exit the display when you press Escape (for some reason, pressing the "X" to close the window doesn't work)
    # Run the Color Detection code when you press Space
    # Press Space again to end Color Detection code
    for event in pygame.event.get():
        if event.type == pygame.KEYDOWN:           
            if event.key == pygame.K_RETURN:
                snapshot = cam.getImage()

                #Change this directory if needed
                snapshot.save('F:\stuff\img.png')
                
                snapshotTaken = True
                print 'Took snapshot'
            if event.key == pygame.K_ESCAPE:
                display.quit()
            if event.key == pygame.K_SPACE and snapshotTaken:
                print 'Image Processing initiated'
                if not doImageProcessing:
                        doImageProcessing = True
                else:
                        doImageProcessing = False

