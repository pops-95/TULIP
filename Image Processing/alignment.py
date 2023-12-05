import cv2 as cv
import matplotlib.pyplot as plt
import numpy as np

img=cv.imread("image.jpg")

img=cv.cvtColor(img,cv.COLOR_BGR2HSV)

# Use canny edge detection
# edges = cv.Canny(img,50,150,apertureSize=3)
## Mask of green (36,25,25) ~ (86, 255,255)
# mask = cv2.inRange(hsv, (36, 25, 25), (86, 255,255))
mask = cv.inRange(img, (20, 25, 25), (80, 255,255))

## Slice the green
imask = mask>0
green = np.zeros_like(img, np.uint8)
green[imask] = img[imask]

green=cv.cvtColor(green,cv.COLOR_HSV2BGR)
green=cv.cvtColor(green,cv.COLOR_BGR2GRAY)
edges = cv.Canny(green,50,150,apertureSize=3)



f, (axarr1,axarr2) = plt.subplots(1,2,sharey=True)
axarr1.imshow(img)
axarr2.imshow(edges,cmap='gray')

# plt.imshow(img,cmap='gray')


plt.show()