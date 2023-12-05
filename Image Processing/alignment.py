import cv2 as cv
import matplotlib.pyplot as plt

img=cv.imread("image.jpg")
for_plt=cv.cvtColor(img,cv.COLOR_BGR2RGB)
img=cv.cvtColor(img,cv.COLOR_BGR2GRAY)

# Use canny edge detection
edges = cv.Canny(img,50,150,apertureSize=3)


# plt.imshow(img,cmap='gray')
plt.imshow(for_plt)
plt.show()