## License: Apache 2.0. See LICENSE file in root directory.
## Copyright(c) 2015-2017 Intel Corporation. All Rights Reserved.

###############################################
##      Open CV and Numpy integration        ##
###############################################

import pyrealsense2 as rs
import numpy as np
import matplotlib.pyplot as plt 
import cv2 as cv
import threading
from utils import *
import numpy


global pipe
global processed_frame
global stop
global point
global hsvImage

# Create a window and set the mouse callback function
# cv.namedWindow("Frame", cv.WINDOW_AUTOSIZE)



stop=False
color_map=rs.colorizer()
dec=rs.decimation_filter()
dec.set_option(rs.option.filter_magnitude,2)
depth2disparity=rs.disparity_transform()
disparity2depth=rs.disparity_transform(False)
spat=rs.spatial_filter()
spat.set_option(rs.option.holes_fill,5)
temp=rs.temporal_filter()
align_to=rs.align(rs.stream.color)


def post_processing_thread(lock):
    global pipe
    while(not stop):
        
        data=pipe.poll_for_frames()
        if(data):
            data=align_to.process(data)
            # print("ok")
            # data=data.get_depth_frame()
            data.as_frameset()
            # print(data.get_height())
            lock.acquire()
            data=depth2disparity.process(data)
            data=spat.process(data)
            data=temp.process(data)
            data=disparity2depth.process(data)
            processed_frame.enqueue(data)
            lock.release()

if __name__=="__main__":
    point=(400,200)
    pipe=rs.pipeline()
    cfg=rs.config()
    lock=threading.Lock()
    # windowName="HSV"
    # cv.namedWindow(windowName,cv.WINDOW_AUTOSIZE)
    
    cfg.enable_stream(rs.stream.depth, 1280,720, rs.format.z16, 6)
    cfg.enable_stream(rs.stream.color, 1280,720, rs.format.bgr8, 30)
    
    profile=pipe.start(cfg)
    sensor=profile.get_device().first_depth_sensor()
    sensor.set_option(rs.option.visual_preset,4)
    
    stream=profile.get_stream(rs.stream.depth).as_video_stream_profile()
    
    processed_frame=rs.frame_queue()
    threading.Thread(target=post_processing_thread,args=(lock,)).start()
    while(True):
        # cv2.setMouseCallback("Color Stream", mouse_callback)
        # print("Inside main")
        
        current_frameset=processed_frame.poll_for_frame().as_frameset()
        if(current_frameset.is_frameset()):
            depth=current_frameset.get_depth_frame()
            color=current_frameset.get_color_frame()
            
            img = np.asanyarray(color.get_data())
            # cv.imwrite("normal.jpg",img)
            gray = cv.cvtColor(img, cv.COLOR_RGB2GRAY)

            # Apply GaussianBlur to reduce noise and help contour detection
            blurred = cv.GaussianBlur(gray, (9, 9), 0)
            
            edges = cv.Canny(blurred,100,150,apertureSize=3)
            
            
            cnts = cv.findContours(edges, cv.RETR_EXTERNAL, cv.CHAIN_APPROX_SIMPLE)
            cnts = cnts[0] if len(cnts) == 2 else cnts[1]

            for cnt in cnts:
                
                x1,y1 = cnt[0][0]
                
                approx = cv.approxPolyDP(cnt, 0.01*cv.arcLength(cnt, True), True)
                if len(approx) == 4:
                    x, y, w, h = cv.boundingRect(cnt)
                    ratio = float(w)/h
                    area=cv.contourArea(cnt)
                    print("Area is = {} ".format(area))
                    print("ratio is = {:.3f} ".format(ratio))
                    if area >=10000:
                        img = cv.drawContours(img, [cnt], -1, (0,255,255), 3)
                        # cv.putText(img, 'Square', (x1, y1), cv.FONT_HERSHEY_SIMPLEX, 0.6, (255, 255, 0), 2)
                    
            # cv.drawContours(img, contours, -1, (0,255,0), 3)
            cv.imshow("frame",img)
            cv.imshow("edges",edges)
            # cv.imshow("thresh",thresh)

        if cv.waitKey(1) & 0xFF == ord('q'):
            stop=True
            pipe.stop()
            cv.destroyAllWindows() 
            break
    
    
    
    