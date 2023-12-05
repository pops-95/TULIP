import math
import numpy as np

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
            if(1135<=x1<=2400 and 1135<=x2<=2400 and 753<=y1<=2730 and  753<=y2<=2730 ):
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
