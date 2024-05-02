import RPi.GPIO as GPIO
import Pinmap
import time
import VL53L1X


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
    GPIO.output(Pinmap.dir_x,GPIO.LOW)
    while(1):
        if(GPIO.input(Pinmap.x_limit_extra)==0):
            pulse(Pinmap.x_limit_extra)
        else:
            break

def move_x_front():
    GPIO.output(Pinmap.dir_x,GPIO.HIGH)
    while(1):
        if(GPIO.input(Pinmap.x_limit)==0):
            pulse(Pinmap.x_limit)
        else:
            break


def move_y_right():
    GPIO.output(Pinmap.dir_y,GPIO.LOW)
    while(1):
        if(GPIO.input(Pinmap.y_limit_extra)==0):
            pulse(Pinmap.y_limit_extra)
        else:
            break

def move_y_left():
    GPIO.output(Pinmap.dir_x,GPIO.HIGH)
    while(1):
        if(GPIO.input(Pinmap.y_limit)==0):
            pulse(Pinmap.y_limit)
        else:
            break
        
def move_z_up():
    GPIO.output(Pinmap.dir_z,GPIO.HIGH)
    while(1):
        if(GPIO.input(Pinmap.z_limit)==0):
            pulse(Pinmap.z_limit,Pinmap.delay_z)
        else:
            break
    
        
def origin():
    move_z_up()
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
    tof_x.set_user_roi(VL53L1X.VL53L1xUserRoi(6, 9, 9, 6))
    tof_x.set_timing(50000,55)
    tof_x.start_ranging(1)
    
    while running:
        result[0] = tof_x.get_distance()
    

    
def measure_y(result):
    
    global running
    tof_y = VL53L1X.VL53L1X(i2c_bus=1, i2c_address=Pinmap.addr_desired)
    tof_y.open()
    tof_y.set_user_roi(VL53L1X.VL53L1xUserRoi(6, 9, 9, 6))
    tof_y.set_timing(50000,55)
    tof_y.start_ranging(1)
    
    while running:
        result[1] = tof_y.get_distance()    
        


def move_x(distance,result):
    if(result[0]>distance):
        GPIO.output(Pinmap.dir_x,GPIO.LOW)
    else:
        GPIO.output(Pinmap.dir_x,GPIO.HIGH)
        
    while 1:
        if((distance-Pinmap.reach_thresh)<=result[0]and result[0]<=(distance+Pinmap.reach_thresh)):
            print("reached position = {} mm".format(result[0]))
            break
        else:
            pulse(Pinmap.pulse_x,Pinmap.delay_x)


def move_y(distance,result):
    if(result[1]>distance):
        GPIO.output(Pinmap.dir_y,GPIO.LOW)
    else:
        GPIO.output(Pinmap.dir_y,GPIO.HIGH)
        
    while 1:
        if((distance-Pinmap.reach_thresh)<=result[0]and result[0]<=(distance+Pinmap.reach_thresh)):
            print("reached position = {} mm".format(result[1]))
            break
        else:
            pulse(Pinmap.pulse_y,Pinmap.delay_y)



