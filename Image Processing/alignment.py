import math
import cv2 as cv
import matplotlib.pyplot as plt
import numpy as np
from  utils import *



img=cv.imread("image.jpg")
img=cv.cvtColor(img,cv.COLOR_BGR2HSV)
mask = cv.inRange(img, (20, 25, 25), (80, 255,255)) # detecting only green color

## Slice the green
imask = mask>0
green = np.zeros_like(img, np.uint8)
green[imask] = img[imask]

green=cv.cvtColor(green,cv.COLOR_HSV2BGR)
green=cv.cvtColor(green,cv.COLOR_BGR2GRAY)

edges = cv.Canny(green,50,150,apertureSize=3)

 
# Apply HoughLinesP method to 
# to directly obtain line end points
lines_list =[]
lines = cv.HoughLinesP(
            edges, # Input edge image
            6, # Distance resolution in pixels
            np.pi/(180), # Angle resolution in radians
            threshold=150, # Min number of votes for valid line
            minLineLength=10, # Min allowed length of line
            maxLineGap=3 # Max allowed gap between line for joining them
            )
 # Make a copy of the original image.
img = np.copy(img)    # Create a blank image that matches the original in size.
line_img = np.zeros(
    (
        img.shape[0],
        img.shape[1],
        3
    ),
    dtype=np.uint8,
)    # Loop over all lines and draw them on the blank image.



(sloped_line_x,sloped_line_y)=line_segmentation(lines)

min_y = 0  # <-- Just below the horizon
max_y = img.shape[0] # <-- The bottom of the image

(sloped_x_start,sloped_x_end)=findpoints(sloped_line_x,sloped_line_y,min_y,max_y)




# Driver code
A = Point(img.shape[1]/2, 0)
B = Point(img.shape[1]/2,img.shape[1])
C = Point(sloped_x_start, max_y)
D = Point(sloped_x_end,min_y)

pnt=lineLineIntersection(A,B,C,D)
pnt.displayPoint(pnt)
img=cv.cvtColor(img,cv.COLOR_HSV2RGB)







cv.line(img, (int(img.shape[1]/2), 0), (int(img.shape[1]/2),img.shape[1]), (255,0,0), 6)
cv.line(img, (sloped_x_start, max_y), (sloped_x_end,min_y), (255,255,255), 6)


img=cv.circle(img,(int(pnt.x),int(pnt.y)),20,(255,0,0),-1)

f, (axarr1,axarr2) = plt.subplots(1,2,sharey=True)
axarr1.imshow(img)
axarr2.imshow(edges,cmap='gray')

# plt.imshow(img,cmap='gray')


plt.show()