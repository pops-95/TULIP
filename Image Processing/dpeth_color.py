import numpy as np
import cv2 as cv
import matplotlib.pyplot as plt
import pyrealsense2 as rs

img_name="E:\\share\\debu\\Cropped_Image\\Exposure_Image\\Color_Frame\\frame_32.jpg"
file_name="E:\\share\\debu\\Cropped_Image\\Exposure_Image\\Depth_Frame\\frame_32.txt"
thresh=1000



ending_dist=2000


dist_value=[]
number_pixel=[]




depth_data=np.loadtxt(file_name,dtype='float')

def plot_pixel():
    starting_dist=800
    pix_num=0
    while(starting_dist!=ending_dist):
        dist_value.append(str(starting_dist))
        for i in range(depth_data.shape[0]):
           for j in range(depth_data.shape[1]):
               diff=depth_data[i,j]-starting_dist
               if(-50<diff and  diff <50):
                   pix_num=pix_num+1
        
        pix_num=pix_num/10000
        number_pixel.append(pix_num)
        pix_num=0
        starting_dist=starting_dist+100



# color_map=rs.colorizer()
# img=cv.imread(img_name)
# img=cv.cvtColor(img,cv.COLOR_BGR2RGB)



print(depth_data[360,640])



# color_image=np.zeros((depth_data.shape[0],depth_data.shape[1],3),np.uint8)

# for i in range(depth_data.shape[0]):
#     for j in range(depth_data.shape[1]):
#         if(depth_data[i,j])==0:
#             continue
#         else:
#             diff=depth_data[i,j]-thresh
#             if(diff>0):
#                 color_image[i,j,0]=img[i,j,0]
#                 color_image[i,j,1]=img[i,j,1]
#                 color_image[i,j,2]=img[i,j,2]



# plt.imshow(color_image)
# plt.show()
plot_pixel()

print(dist_value)
print(number_pixel)

plt.bar(dist_value,number_pixel)
plt.show()