import signal
import sys
from utils import *
import multiprocessing
import VL53L1X

running=True
timing_budget=50000
inter_measurement_time=60
roi=VL53L1X.VL53L1xUserRoi(6, 3, 9, 0)
def change_add_sensor():
    
    
    GPIO.output(Pinmap.x_sensor_shut,GPIO.LOW)
    GPIO.output(Pinmap.y_sensor_shut,GPIO.LOW)
    
    time.sleep(0.05)
    GPIO.output(Pinmap.y_sensor_shut,GPIO.HIGH)
    
    tof = VL53L1X.VL53L1X(i2c_bus=1, i2c_address=Pinmap.addr_current)
    tof.open()
    tof.change_address(Pinmap.addr_desired)
    tof.close()
    
    GPIO.output(Pinmap.x_sensor_shut,GPIO.HIGH)
    
    
def measure_x(result):
    global running
    tof_x = VL53L1X.VL53L1X(i2c_bus=1, i2c_address=Pinmap.addr_current)
    tof_x.open()
    tof_x.set_user_roi(roi)
    tof_x.set_timing(timing_budget,inter_measurement_time)
    tof_x.start_ranging(1)
    
    while running:
        result[0] = tof_x.get_distance()
    

    
def measure_y(result):
    
    global running
    tof_y = VL53L1X.VL53L1X(i2c_bus=1, i2c_address=Pinmap.addr_desired)
    tof_y.open()
    tof_y.set_user_roi(roi)
    tof_y.set_timing(timing_budget,inter_measurement_time)
    tof_y.start_ranging(1)
    
    while running:
        result[1] = tof_y.get_distance()    
        

def exit_handler(signal, frame):
    global running
    running = False

    GPIO.cleanup()
    print()
    sys.exit(0)
    
signal.signal(signal.SIGINT, exit_handler)


if __name__=="__main__":
    GPIO.cleanup()
    print("hello")
    change_add_sensor()
    result=multiprocessing.Array('i',[0]*2)
    
    x1=multiprocessing.Process(target=measure_x,args=(result,))
    x1.start()
    y1=multiprocessing.Process(target=measure_y,args=(result,))
    
    
    y1.start()
    move_x(300,result)
    # while(running):
    #     print("x = {} mm and y = {} mm".format(result[0],result[1]))
    

    