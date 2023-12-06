import math

import matplotlib.pyplot as plt
import numpy as np
from  utils import *



img=cv.imread("normal.jpg")

img,edges=get_aligned_img(img)

f, (axarr1,axarr2) = plt.subplots(1,2,sharey=True)
axarr1.imshow(img)
axarr2.imshow(edges,cmap='gray')

# plt.imshow(img,cmap='gray')


plt.show()