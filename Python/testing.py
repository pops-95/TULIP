import threading
from GPIO import *



if __name__=="__main__":
    result=multiprocessing.Array('i',[0]*2)
    controller=RPI_controller()
    controller.measure_x(result,True)
    