import math
import RPi.GPIO as GPIO
import Pinmap
import time
import VL53L1X
import signal
import sys
import multiprocessing
import numpy as np



class RPI_controller():
    def __init__(self,timing_budget=25000,inter_measurement_time=30):
        self.timing_budget=timing_budget
        self.inter_measurement_time=inter_measurement_time
        self.roi_y=VL53L1X.VL53L1xUserRoi(6, 3, 9, 0)
        self.roi_x=VL53L1X.VL53L1xUserRoi(6, 3, 9, 0)
        
    def gpio_init(self):
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
        
    def pulse(channel,_durationsleep=1):
        _durationsleep=_durationsleep/1000000
        GPIO.output(channel,GPIO.HIGH)
        time.sleep(_durationsleep)
        GPIO.output(channel,GPIO.LOW)
        time.sleep(_durationsleep)
        
    def move_x_back(self):
        GPIO.output(Pinmap.dir_x,GPIO.HIGH)
        while(1):
            if(GPIO.input(Pinmap.x_limit_extra)==0):
                self.pulse(Pinmap.pulse_x,100)
            else:
                break
    
    def move_x_front(self):
        GPIO.output(Pinmap.dir_x,GPIO.LOW)
        while(1):
            if(GPIO.input(Pinmap.x_limit)==0):
                self.pulse(Pinmap.pulse_x,50)
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
    def move_y_right(self):
        GPIO.output(Pinmap.dir_y,GPIO.LOW)
        while(1):
            if(GPIO.input(Pinmap.y_limit_extra)==0):
                self.pulse(Pinmap.pulse_y,100)
            else:
                break