from SimpleCV import Camera, Display, Image, Color, DrawingLayer

# primColors = {'RED':(255,0,0),'GREEN':(0,255,0), 'BLUE': (0,0,255)}
# compColors = {'CYAN':(0,255,255), 'YELLOW': (255,255,0), 'MAGENTA':(255,0,255)}

primColors = [Color.RED, Color.GREEN, Color.BLUE]
compColors = [Color.CYAN, Color.YELLOW, (255,0,255)]

# targetColors = [Color.PURPLE, Color.RED, Color.BLUE, Color.GREEN, Color.YELLOW, Color.ORANGE]

#To check progress quickly
def my_show(image, length=60):
    i = 0
    while i != length:
        image.show()
        i += 1

def halfsies(left,right): 
    result = left
    # crop the right image to be just the right side.
    crop   = right.crop(right.width/2.0,0,right.width/2.0,right.height)
    # now paste the crop on the left image.
    result = result.blit(crop,(left.width/2,0))
    # return the results.
    return result

def color_filter(image, targetColor):
    """Returns the difference between the image and image relative to target"""
    temp = image - image.colorDistance(targetColor)
    my_show(temp)
     return temp


def get_largestBlobs(image):
    """Returns the largest blobs by area, assumes that we want largest blobs"""
    return sorted(image.findBlobs(), key=lambda blob: blob.area(), reverse=True)


#Just a paranoia check, not necessary
# def check_mod(tup):
#     return (tup[0] == tup[0]%256) and (tup[1] == tup[1]%256) and (tup[2] == tup[2]%256)


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


if __name__ == "__main__":
     # cam = Camera()
     # display = Display((640,480))
     # while not display.isDone():
     #     img = cam.getImage().flipHorizontal()
     #     if display.mouseLeft:
     #         img.save("temp.png")
     #         break
     #     img.show()
     #target = (4.683625, 3.6336250000000003, 5.031350000000001)
     target = Color.YELLOW
     loaded = Image("parking-car.png")
     my_show(loaded)
     cf = color_filter(loaded, target)
     blobs = get_largestBlobs(cf)
     final, nextColor = idByColor(cf,target,blobs)
     my_show(final,length=70)


     # target = nextColor
     # loaded = Image("parking-car.png")
     # my_show(loaded)
     # cf = color_filter(loaded, target)
     # blobs = get_largestBlobs(cf)
     # final, nextColor = idByColor(cf,target,blobs)
     # my_show(final,length=70)