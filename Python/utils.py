import math
import RPi.GPIO as GPIO
import Pinmap
import time
import VL53L1X
import signal
import sys
import multiprocessing
import numpy as np

running=True
timing_budget=50000
inter_measurement_time=60
roi=VL53L1X.VL53L1xUserRoi(6, 3, 9, 0)


# GPIO.cleanup()

GPIO.setmode(GPIO.BCM)

GPIO.setup(Pinmap.pulse_x,GPIO.OUT)
GPIO.setup(Pinmap.pulse_y,GPIO.OUT)
GPIO.setup(Pinmap.pulse_z,GPIO.OUT)
GPIO.setup(Pinmap.pulse_c,GPIO.OUT)

GPIO.setup(Pinmap.dir_x,GPIO.OUT)
GPIO.setup(Pinmap.dir_y,GPIO.OUT)
GPIO.setup(Pinmap.dir_z,GPIO.OUT)
GPIO.setup(Pinmap.dir_c,GPIO.OUT)

GPIO.setup(Pinmap.x_limit,GPIO.IN,pull_up_down=GPIO.PUD_DOWN)
GPIO.setup(Pinmap.y_limit,GPIO.IN,pull_up_down=GPIO.PUD_DOWN)
GPIO.setup(Pinmap.z_limit,GPIO.IN)
GPIO.setup(Pinmap.x_limit_extra,GPIO.IN,pull_up_down=GPIO.PUD_DOWN)
GPIO.setup(Pinmap.y_limit_extra,GPIO.IN,pull_up_down=GPIO.PUD_DOWN)

GPIO.setup(Pinmap.x_sensor_shut,GPIO.OUT)
GPIO.setup(Pinmap.y_sensor_shut,GPIO.OUT)
# 0.000001 means 1 microsecond

def pulse(channel,_durationsleep=1):
    _durationsleep=_durationsleep/1000000
    GPIO.output(channel,GPIO.HIGH)
    time.sleep(_durationsleep)
    GPIO.output(channel,GPIO.LOW)
    time.sleep(_durationsleep)
    
def move_x_back():
    GPIO.output(Pinmap.dir_x,GPIO.HIGH)
    while(1):
        if(GPIO.input(Pinmap.x_limit_extra)==0):
            pulse(Pinmap.pulse_x,100)
        else:
            break

def move_x_front():
    GPIO.output(Pinmap.dir_x,GPIO.LOW)
    while(1):
        if(GPIO.input(Pinmap.x_limit)==0):
            pulse(Pinmap.pulse_x,100)
        else:
            break


def move_y_right():
    GPIO.output(Pinmap.dir_y,GPIO.LOW)
    while(1):
        if(GPIO.input(Pinmap.y_limit_extra)==0):
            pulse(Pinmap.pulse_y,100)
        else:
            break

def move_y_left():
    GPIO.output(Pinmap.dir_x,GPIO.HIGH)
    while(1):
        if(GPIO.input(Pinmap.y_limit)==0):
            pulse(Pinmap.pulse_y,100)
        else:
            break
        
def move_z_up():
    GPIO.output(Pinmap.dir_z,GPIO.HIGH)
    while(1):
        if(GPIO.input(Pinmap.z_limit)==0):
            pulse(Pinmap.pulse_z,Pinmap.delay_z)
        else:
            break
    
        
def origin():
    # move_z_up()
    move_x_back()
    move_y_right()
    

def openclamp():
    GPIO.output(Pinmap.dir_c,GPIO.LOW)
    for i in range(.3*Pinmap.number_pulse_zc):
        pulse(Pinmap.pulse_c,_durationsleep=Pinmap.delay_c)
    
def closeclamp():
    GPIO.output(Pinmap.dir_c,GPIO.HIGH)
    for i in range(.6*Pinmap.number_pulse_zc):
        pulse(Pinmap.pulse_c,_durationsleep=Pinmap.delay_z)
        

def store_leaf():
    move_x_front()
    



def move_x(distance,result):
    print("current position = {} mm ".format(result[0]))
    if(result[0]>distance):
        GPIO.setup(Pinmap.dir_x,GPIO.OUT)
        time.sleep(0.001)
        GPIO.output(Pinmap.dir_x,GPIO.HIGH)
    else:
        GPIO.setup(Pinmap.dir_x,GPIO.OUT)
        time.sleep(0.001)
        GPIO.output(Pinmap.dir_x,GPIO.LOW)
        
    while 1:
        diff=result[0]-distance
        if((distance-Pinmap.reach_thresh)<=result[0]and result[0]<=(distance+Pinmap.reach_thresh)):
            print("reached position = {} mm".format(result[0]))
            break
        if(abs(diff)<=50):
            pulse(Pinmap.pulse_x,400)
            # print("current position = {} mm ".format(result[0]))
            
        if(abs(diff)>50):
            pulse(Pinmap.pulse_x,10)
            # print("current position = {} mm ".format(result[0]))


def move_y(distance,result):
    print("current position = {} mm ".format(result[1]))
    if(result[1]>distance):
        GPIO.output(Pinmap.dir_y,GPIO.LOW)
    else:
        GPIO.output(Pinmap.dir_y,GPIO.HIGH)
        
    while 1:
        diff=result[1]-distance
        
        if((distance-Pinmap.reach_thresh_y)<=result[1]and result[1]<=(distance+Pinmap.reach_thresh_y)):
            print("reached position = {} mm".format(result[1]))
            break
        if(abs(diff)<=60):
            pulse(Pinmap.pulse_y,400)
            # print("current position = {} mm ".format(result[1]))
            
        if(abs(diff)>60):
            pulse(Pinmap.pulse_y,50)
            # print("current position = {} mm ".format(result[1]))
            
            
def check_accuracy(distance,result):
    move_y(distance,result)
    
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
        

def calculate_xy(camera_values):
      cam_mat=np.array([[camera_values[0]],[camera_values[1]],[camera_values[2]],[1]])
      print(cam_mat)
      init_mat=np.array([[0,-1,0,187],[-1,0,0,427],[0,0,1,-723],[0,0,0,1]])
      print(init_mat)
      val=np.dot(init_mat,cam_mat)
      val[0]=val[0]+206-102-16+10
      distance=distance_measure(camera_values)
      val[1]=val[1]-(distance/10)-92+16
      return val[0],val[1],val[2]
  
def distance_measure(camera_values):
    x_val=458
    y_val=199
    z_val=732
    distance=math.sqrt(math.pow((x_val-camera_values[0]),2)+math.pow((y_val-camera_values[1]),2)+math.pow((z_val-camera_values[2]),2))
    return distance
        
def check_distance(result):
    global running
    while(running):
        print("x = {} mm and y = {} mm".format(result[0],result[1]))
        time.sleep(1)
def exit_handler(signal, frame):
    global running
    running = False

    GPIO.cleanup()
    print()
    sys.exit(0)
    
signal.signal(signal.SIGINT, exit_handler)





if __name__=="__main__":
    # GPIO.cleanup()
    # x_dis=580
    # y_dis=512
    part_values=[]
    camera_values=[-222,55,1348]
    # print(len(camera_values))
    for i in range(len(camera_values)/3):
        part_values[0]=camera_values[i*3]
        part_values[1]=camera_values[(i*3)+1]
        part_values[2]=camera_values[(i*3)+2]
        
        x_dis,y_dis,z_dis=calculate_xy(part_values)
        
    
    
     
    # origin()
    # change_add_sensor()
    # result=multiprocessing.Array('i',[0]*2)
    
    # x1=multiprocessing.Process(target=measure_x,args=(result,))
    # x1.start()
    # y1=multiprocessing.Process(target=measure_y,args=(result,))
    
    # y1.start()
    time.sleep(3)
    print("calculated x = {} mm , y= {} mm Distance= {} mm".format(x_dis,y_dis,distance))
    # check_distance(result)
    # move_x(x_dis,result)
    # time.sleep(0.5)
    # move_y(y_dis,result)
    # check_accuracy(y_dis,result)
    
