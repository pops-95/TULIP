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
    
    def move_y_left(self):
        GPIO.output(Pinmap.dir_x,GPIO.HIGH)
        while(1):
            if(GPIO.input(Pinmap.y_limit)==0):
                self.pulse(Pinmap.pulse_y,100)
            else:
                break
    
    def move_z_up(self):
        i=0
        GPIO.output(Pinmap.dir_z,GPIO.HIGH)
        while(1):
            # print("value limiz z = {}".format(GPIO.input(Pinmap.z_limit)))
            if(GPIO.input(Pinmap.z_limit)==1):
                self.pulse(Pinmap.pulse_z,Pinmap.delay_z)
                i=i+1
                
            else:
                # print("total z pulse= {} ".format(i))
                break
    
    def origin(self):
    # move_z_up()
        self.move_x_back()
        self.move_y_right() 
    
        
    def openclamp(self):
        GPIO.output(Pinmap.dir_c,GPIO.LOW)
        total_pulse=(int)((.3)*Pinmap.number_pulse_zc)
        for i in range(total_pulse):
            self.pulse(Pinmap.pulse_c,_durationsleep=Pinmap.delay_c)    
            
    def closeclamp(self):
        GPIO.output(Pinmap.dir_c,GPIO.HIGH)
        total_pulse=(int)((.8)*Pinmap.number_pulse_zc)
        for i in range(total_pulse):
            self.pulse(Pinmap.pulse_c,_durationsleep=Pinmap.delay_z)    

    def store_leaf(self):
        self.move_x_front()
        # move_z_down(350)
        time.sleep(.5)
        self.openclamp()
        time.sleep(1)
        self.closeclamp()
        time.sleep(.5)
        #  time.sleep(.5)
        # move_z_up()
    
    def move_x(self,distance,result):
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
                self.pulse(Pinmap.pulse_x,500)
                # print("current position x = {} mm ".format(result[0]))
                
            if(diff>60):
                self.pulse(Pinmap.pulse_x,10)
                # print("current position x = {} mm ".format(result[0]))
            if(diff<0):
                print("Exceeded position x = {} mm".format(result[0]))
                break
            # time.sleep(.0001)
        
        
    def move_y(self,distance,result):
        
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
                self.pulse(Pinmap.pulse_y,500)
                # print("current position y = {} mm ".format(result[1]))
                
            if(diff>60):
                pulse(Pinmap.pulse_y,50)
                # print("current position  y = {} mm ".format(result[1]))
            if(diff<0):
                self.print("Exceeded position y = {} mm".format(result[1]))
                break
            # time.sleep(.0001)
                
        
    def check_accuracy_x(self,distance,result):
        self.move_x(distance,result)            
    def check_accuracy_y(self,distance,result):
        self.move_y(distance,result)  
        
    def move_z_down(self,distance):
        GPIO.output(Pinmap.dir_z,GPIO.LOW)
        step=(distance-880)/(125*2)
        pulse_number=(int)(step*Pinmap.number_pulse_zc)
        for i in range(pulse_number):
            self.pulse(Pinmap.pulse_z,Pinmap.delay_z)
        
            
    def change_add_sensor(self):
        
        
        GPIO.output(Pinmap.x_sensor_shut,GPIO.LOW)
        GPIO.output(Pinmap.y_sensor_shut,GPIO.LOW)
        
        time.sleep(0.05)
        GPIO.output(Pinmap.y_sensor_shut,GPIO.HIGH)
        
        tof = VL53L1X.VL53L1X(i2c_bus=1, i2c_address=Pinmap.addr_current)
        tof.open()
        tof.change_address(Pinmap.addr_desired)
        tof.close()
        
        GPIO.output(Pinmap.x_sensor_shut,GPIO.HIGH)      
    
        
    def measure_x(self,result):
        global running
        tof_x = VL53L1X.VL53L1X(i2c_bus=1, i2c_address=Pinmap.addr_current)
        tof_x.open()
        tof_x.set_user_roi(self.roi_x)
        tof_x.set_timing(self.timing_budget,self.inter_measurement_time)
        tof_x.start_ranging(1)
        
        while running:
            result[0] = tof_x.get_distance()
            
            
    def measure_y(self,result):
        
        global running
        tof_y = VL53L1X.VL53L1X(i2c_bus=1, i2c_address=Pinmap.addr_desired)
        tof_y.open()
        tof_y.set_user_roi(self.roi)
        tof_y.set_timing(self.timing_budget,self.inter_measurement_time)
        tof_y.start_ranging(1)
        
        while running:
            result[1] = tof_y.get_distance()        
    