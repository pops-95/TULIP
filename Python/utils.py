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
timing_budget=25000
inter_measurement_time=30
roi=VL53L1X.VL53L1xUserRoi(6, 3, 9, 0)
roi_x=VL53L1X.VL53L1xUserRoi(6, 3, 9, 0)


# GPIO.cleanup()
# def gpio_init():
GPIO.setmode(GPIO.BCM)

GPIO.setup(Pinmap.pulse_x,GPIO.OUT)
GPIO.setup(Pinmap.pulse_y,GPIO.OUT)
GPIO.setup(Pinmap.pulse_z,GPIO.OUT)
GPIO.setup(Pinmap.pulse_c,GPIO.OUT)

GPIO.setup(Pinmap.dir_x,GPIO.OUT)
GPIO.setup(Pinmap.dir_y,GPIO.OUT)
GPIO.setup(Pinmap.dir_z,GPIO.OUT)
GPIO.setup(Pinmap.dir_c,GPIO.OUT)

GPIO.setup(Pinmap.x_limit,GPIO.IN)
GPIO.setup(Pinmap.y_limit,GPIO.IN)
GPIO.setup(Pinmap.z_limit,GPIO.IN)
GPIO.setup(Pinmap.x_limit_extra,GPIO.IN)
GPIO.setup(Pinmap.y_limit_extra,GPIO.IN)

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
            pulse(Pinmap.pulse_x,50)
        else:
            # print("limit value {}".format(GPIO.input(Pinmap.x_limit)))
            # if(GPIO.input(Pinmap.x_limit)==0):
            #     print("limit value {}".format(GPIO.input(Pinmap.x_limit)))
            #     continue
            # else:
            #     print("limit value {}".format(GPIO.input(Pinmap.x_limit)))
            #     break
            break
        # break


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
    i=0
    GPIO.output(Pinmap.dir_z,GPIO.HIGH)
    while(1):
        # print("value limiz z = {}".format(GPIO.input(Pinmap.z_limit)))
        if(GPIO.input(Pinmap.z_limit)==1):
            pulse(Pinmap.pulse_z,200)
            i=i+1
            
        else:
            # print("total z pulse= {} ".format(i))
            break
    
        
def origin():
    # move_z_up()
    move_x_back()
    move_y_right()
    

def openclamp():
    GPIO.output(Pinmap.dir_c,GPIO.HIGH)
    total_pulse=(int)((.4)*Pinmap.number_pulse_zc)
    for i in range(total_pulse):
        pulse(Pinmap.pulse_c,_durationsleep=Pinmap.delay_c)
    
def closeclamp():
    GPIO.output(Pinmap.dir_c,GPIO.LOW)
    total_pulse=(int)((.5)*Pinmap.number_pulse_zc)
    for i in range(total_pulse):
        pulse(Pinmap.pulse_c,_durationsleep=Pinmap.delay_z)
        

def store_leaf():
    move_x_front()
    # move_z_down(350)
    time.sleep(.5)
    openclamp()
    time.sleep(1)
    closeclamp()
    time.sleep(.5)
    #  time.sleep(.5)
    # move_z_up()



def move_x(distance,result):
    distance=distance-8
    print("current position x = {} mm ".format(result[0]))
    if(result[0]>distance):
        flag=True
        max_dis=distance-Pinmap.reach_thresh
        min_dis=distance+Pinmap.reach_thresh
        GPIO.setup(Pinmap.dir_x,GPIO.OUT)
        time.sleep(0.001)
        GPIO.output(Pinmap.dir_x,GPIO.HIGH)
    else:
        flag=False
        max_dis=distance+Pinmap.reach_thresh
        min_dis=distance-Pinmap.reach_thresh
        GPIO.setup(Pinmap.dir_x,GPIO.OUT)
        time.sleep(0.001)
        GPIO.output(Pinmap.dir_x,GPIO.LOW)
        
    while 1:
        if(flag):
            diff=result[0]-distance
            if(max_dis<=result[0]and result[0]<=min_dis):
                print("reached position x = {} mm".format(result[0]))
                break
        
        else:
            diff=distance-result[0]
            if(min_dis<=result[0]and result[0]<=max_dis):
                print("reached position x = {} mm".format(result[0]))
                break
        
        
        
        if(diff<=60):
            # if(result[0]>max_dis):
            #     break
            pulse(Pinmap.pulse_x,500)
            # print("current position x = {} mm ".format(result[0]))
            
        if(diff>60):
            pulse(Pinmap.pulse_x,75)
            # print("current position x = {} mm ".format(result[0]))
        if(diff<0):
            print("Exceeded position x = {} mm".format(result[0]))
            break
        # time.sleep(.0001)

def trapezoidal_x(distance,result):
    pulse_width=200
    distance=distance-8
    print("current position x = {} mm ".format(result[0]))
    if(result[0]>distance):
        flag=True
        max_dis=distance-Pinmap.reach_thresh
        min_dis=distance+Pinmap.reach_thresh
        GPIO.setup(Pinmap.dir_x,GPIO.OUT)
        time.sleep(0.001)
        GPIO.output(Pinmap.dir_x,GPIO.HIGH)
    else:
        flag=False
        max_dis=distance+Pinmap.reach_thresh
        min_dis=distance-Pinmap.reach_thresh
        GPIO.setup(Pinmap.dir_x,GPIO.OUT)
        time.sleep(0.001)
        GPIO.output(Pinmap.dir_x,GPIO.LOW)
        
    while 1:
        if(flag):
            diff=result[0]-distance
            if(max_dis<=result[0]and result[0]<=min_dis):
                print("reached position x = {} mm".format(result[0]))
                break
        
        else:
            diff=distance-result[0]
            if(min_dis<=result[0]and result[0]<=max_dis):
                print("reached position x = {} mm".format(result[0]))
                break
            
        if(diff<=60):
            # if(result[0]>max_dis):
            #     break
            pulse(Pinmap.pulse_x,500)
            # print("current position x = {} mm ".format(result[0]))
        if(diff>60):
            
            pulse(Pinmap.pulse_x,pulse_width)
            pulse_width=pulse_width-10
            if(pulse_width<=10):
                pulse_width=10    
        
        if(diff<0):
            print("Exceeded position x = {} mm".format(result[0]))
            break
        
    
def move_y(distance,result):
    
    print("current position y = {} mm ".format(result[1]))
    if(result[1]>distance):
        flag=True
        max_dis=distance-Pinmap.reach_thresh_y
        min_dis=distance+Pinmap.reach_thresh_y
        GPIO.output(Pinmap.dir_y,GPIO.LOW)
    else:
        flag=False
        max_dis=distance+Pinmap.reach_thresh_y
        min_dis=distance-Pinmap.reach_thresh_y
        GPIO.output(Pinmap.dir_y,GPIO.HIGH)
        
    while 1:
        if(flag):
            diff=result[1]-distance
            if(max_dis<=result[1]and result[1]<=min_dis):
                print("reached position y = {} mm".format(result[1]))
                break
        else:
            diff=distance-result[1]
            if(min_dis<=result[1]and result[1]<=max_dis):
                print("reached position y = {} mm".format(result[1]))
                break
        
        
        
        if(diff<=60):
            # if(result[1]>max_dis):
            #     break
            pulse(Pinmap.pulse_y,500)
            # print("current position y = {} mm ".format(result[1]))
            
        if(diff>60):
            pulse(Pinmap.pulse_y,50)
            # print("current position  y = {} mm ".format(result[1]))
        if(diff<0):
            print("Exceeded position y = {} mm".format(result[1]))
            break
        # time.sleep(.0001)
            

def check_accuracy_x(distance,result):
    trapezoidal_x(distance,result)            
def check_accuracy_y(distance,result):
    move_y(distance,result)
    
    
def move_z_down(distance):
    GPIO.output(Pinmap.dir_z,GPIO.LOW)
    step=(distance-880)/(125*2)
    pulse_number=(int)(step*Pinmap.number_pulse_zc)
    for i in range(pulse_number):
        pulse(Pinmap.pulse_z,300)
    
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
    tof_x.set_user_roi(roi_x)
    tof_x.set_timing(timing_budget,inter_measurement_time)
    tof_x.start_ranging(1)
    
    while running:
        result[0] = tof_x.get_distance() 
        # sum=0
        # for i in range(10):
        #     val=tof_x.get_distance() 
        #     sum=sum+val

        # result[0] = (int)(sum/10) 
    

    
def measure_y(result):
    
    global running
    tof_y = VL53L1X.VL53L1X(i2c_bus=1, i2c_address=Pinmap.addr_current)
    tof_y.open()
    tof_y.set_user_roi(roi)
    tof_y.set_timing(timing_budget,inter_measurement_time)
    tof_y.start_ranging(1)
    
    while running:
        result[1] = tof_y.get_distance() 
        # sum=0
        # for i in range(10):
        #     val=tof_y.get_distance() 
        #     sum=sum+val

        # result[1] = (int)(sum/10) 
        

def calculate_xy(camera_values):
      cam_mat=np.array([[camera_values[0]],[camera_values[1]],[camera_values[2]],[1]])
    #   print(cam_mat)
      init_mat=np.array([[0,-1,0,187],[-1,0,0,427],[0,0,1,-723],[0,0,0,1]])
    #   print(init_mat)
      val=np.dot(init_mat,cam_mat)
      print(val)
      val[0]=val[0]+206-102
      distance=distance_measure(camera_values)
      print(distance)
      val[1]=val[1]-(distance/10)-92
    #   val[1]=val[1]-92-46-20
      return val[0],val[1],val[2]
  
def distance_measure(camera_values):
    x_val=460
    y_val=202
    z_val=733
    distance=math.sqrt(math.pow((x_val-camera_values[0]),2)+math.pow((y_val-camera_values[1]),2)+math.pow((z_val-camera_values[2]),2))
    return distance
        
def check_distance(result):
    global running
    while(running):
        print("x = {} mm and y = {} mm".format(result[0],result[1]))
        time.sleep(.5)
        
        

            
def exit_handler(signal, frame):
    global running
    running = False

    GPIO.cleanup()
    print()
    sys.exit(0)
    
signal.signal(signal.SIGINT, exit_handler)



# GPIO.output(Pinmap.x_sensor_shut,GPIO.HIGH)
# GPIO.output(Pinmap.y_sensor_shut,GPIO.HIGH)

if __name__=="__main__":
    # GPIO.cleanup()
    # x_dis=580
    # y_dis=512
    # gpio_init()
    # openclamp()
    # time.sleep(2)
    # closeclamp()
    # change_add_sensor()
    # GPIO.cleanup()
    result=multiprocessing.Array('i',[0]*2)
    
    x1=multiprocessing.Process(target=measure_x,args=(result,))
    x1.start()
    # y1=multiprocessing.Process(target=measure_y,args=(result,))
    
    # y1.start()
    # time.sleep(3)
    # move_z_up()
    # origin()
    # while (1):
    #     print(GPIO.input(Pinmap.y_limit))
    #     time.sleep(0.5)
    # check_distance()
    # part_values=[0,0,0]
    # # closeclamp()
    # # time.sleep(2)
    # openclamp()
    # time.sleep(2)
    # closeclamp()
    # # move_z_down(1230)
    # # time.sleep(2)
    # # move_z_up()
    # # camera_values=[188,-16,1348,14,-104,1348,-122,-141,1348,50,-292,1348]
    # camera_values=[507,261,1168,272,334,1226,341,209,1244,228,396,1200]
    # # camera_values=[490,422,1230]
    # print(len(camera_values)/3)
    # for i in range(int(len(camera_values)/3)):
        
    #     # time.sleep(1)
        
    #     x_dis=camera_values[i*3]
    #     y_dis=camera_values[(i*3)+1]
    #     z_dis=camera_values[(i*3)+2]
        
    #     # x_dis,y_dis,z_dis=calculate_xy(part_values)
        
    #     # print("calculated x = {} mm , y= {} mm".format(x_dis,y_dis))
    #     # check_distance(result)
        
        
    #     trapezoidal_x(x_dis,result)
    #     time.sleep(0.5)
    #     check_accuracy_x(x_dis,result)
    #     time.sleep(0.5)
    #     move_y(y_dis,result)
    #     time.sleep(0.5)
    #     check_accuracy_y(y_dis,result)
    #     time.sleep(0.5)
        
    #     # move_z_up()
    #     openclamp()
    #     time.sleep(1)
    #     move_z_down(z_dis)
    #     time.sleep(0.5)
    #     closeclamp()
    #     time.sleep(0.5)
    #     move_z_up()
    #     time.sleep(0.5)
    #     openclamp()
    #     time.sleep(1)
    #     closeclamp()
    #     # closeclamp()
    #     # time.sleep(1)
    #     # store_leaf()
    #     # GPIO.cleanup()
    #     time.sleep(.5)
        
    # running=False
       
        
    
    
   
