import math
import numpy as np
import cv2 as cv

class Point:
    def __init__(self, x, y):
        self.x = x
        self.y = y
 
    # Method used to display X and Y coordinates
    # of a point
    def displayPoint(self, p):
        print(f"({p.x}, {p.y})")
 
def lineLineIntersection(A, B, C, D):
    # Line AB represented as a1x + b1y = c1
    a1 = B.y - A.y
    b1 = A.x - B.x
    c1 = a1*(A.x) + b1*(A.y)
 
    # Line CD represented as a2x + b2y = c2
    a2 = D.y - C.y
    b2 = C.x - D.x
    c2 = a2*(C.x) + b2*(C.y)
 
    determinant = a1*b2 - a2*b1
 
    if (determinant == 0):
        # The lines are parallel. This is simplified
        # by returning a pair of FLT_MAX
        return 0
    else:
        x = (b2*c1 - b1*c2)/determinant
        y = (a1*c2 - a2*c1)/determinant
        return Point(x, y)


def line_segmentation(lines):
    sloped_line_x=[]
    sloped_line_y=[]



    for line in lines:
        for x1, y1, x2, y2 in line:
            
            if(392<=x1<=916 and 392<=x2<=916 and 255<=y1<=596 and  255<=y2<=596 ):
                slope=(y2 - y1) / (x2 - x1)
                if(math.isinf(slope)):
                    continue
                else:
                    sloped_line_x.extend([x1,x2])
                    sloped_line_y.extend([y1,y2])
    
    return (sloped_line_x,sloped_line_y)


def findpoints(sloped_line_x,sloped_line_y,min_y,max_y):
    
    poly_left = np.poly1d(np.polyfit(
        sloped_line_y,
        sloped_line_x,
        deg=1
    ))
    sloped_x_start = int(poly_left(max_y))
    sloped_x_end = int(poly_left(min_y))
    
    return (sloped_x_start,sloped_x_end)


def get_aligned_img(img):
    img=cv.cvtColor(img,cv.COLOR_BGR2HSV)
    mask = cv.inRange(img, (66, 49, 71), (145, 238,255)) # detecting only green color

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
    return (img,edges)