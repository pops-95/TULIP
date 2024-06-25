import math
import RPi.GPIO as GPIO
import Pinmap
import time
import VL53L1X
import signal
import sys
import multiprocessing




class RPI_controller():
    def __init__(self,timing_budget=25000,inter_measurement_time=30):
        self.timing_budget=timing_budget
        self.running=True
        self.inter_measurement_time=inter_measurement_time
        self.roi_y=VL53L1X.VL53L1xUserRoi(6, 3, 9, 0)
        self.roi_x=VL53L1X.VL53L1xUserRoi(6, 3, 9, 0)
        self.z_pulse_number=0
        
        
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
        
    def pulse(self,channel,_durationsleep=1):
        _durationsleep=_durationsleep/1000000
        GPIO.output(channel,GPIO.HIGH)
        time.sleep(_durationsleep)
        GPIO.output(channel,GPIO.LOW)
        time.sleep(_durationsleep)
        
    def move_x_back(self):
        count=0
        GPIO.output(Pinmap.dir_x,GPIO.LOW)
        while(1):
            if(GPIO.input(Pinmap.x_limit_extra)==0):
                self.pulse(Pinmap.pulse_x,300)
            else:
                count=count+1
                if(count >500):
                    break
                else:
                    continue
    
    def move_x_front(self):
        print("inside")
        count=0
        GPIO.output(Pinmap.dir_x,GPIO.HIGH)
        time.sleep(0.01)
        while(1):
            if(GPIO.input(Pinmap.x_limit)==0):
                self.pulse(Pinmap.pulse_x,300)
            else:
                count=count+1
                if(count >2000):
                    break
                else:
                    continue
                # print("limit value {}".format(GPIO.input(Pinmap.x_limit)))
                # break
                
                # time.sleep(0.01)
                # print("limit value {}".format(GPIO.input(Pinmap.x_limit)))
                # if(GPIO.input(Pinmap.x_limit_extra)==0):
                #     continue
                # else:
                #     break
                # print("limit value {}".format(GPIO.input(Pinmap.x_limit)))
                # if(GPIO.input(Pinmap.x_limit)==0):
                #     print("limit value {}".format(GPIO.input(Pinmap.x_limit)))
                #     continue
                # else:
                #     print("limit value {}".format(GPIO.input(Pinmap.x_limit)))
                #     break
            # break
            # break
    def move_y_right(self):
        count=0
        GPIO.output(Pinmap.dir_y,GPIO.LOW)
        while(1):
            if(GPIO.input(Pinmap.y_limit_extra)==0):
                self.pulse(Pinmap.pulse_y,500)
            else:
                count=count+1
                if(count >1000):
                    break
                else:
                    continue
    
    def move_y_left(self):
        count=0
        GPIO.output(Pinmap.dir_y,GPIO.HIGH)
        while(1):
            if(GPIO.input(Pinmap.y_limit)==0):
                self.pulse(Pinmap.pulse_y,500)
            else:
                count=count+1
                if(count >400):
                    break
                else:
                    continue
    
    def move_z_up(self):
        i=0
        GPIO.output(Pinmap.dir_z,GPIO.LOW)
        
        for i in range(self.z_pulse_number):
            self.pulse(Pinmap.pulse_z,500)
            # print("value limiz z = {}".format(GPIO.input(Pinmap.z_limit)))
            # if(GPIO.input(Pinmap.z_limit)==1):
            #     self.pulse(Pinmap.pulse_z,500)
            #     i=i+1
                
            # else:
            #     # print("total z pulse= {} ".format(i))
            #     break
    
    def origin(self):
        # self.move_z_up()
        self.move_x_front()
        self.move_y_left() 
    
        
    def openclamp(self):
        GPIO.output(Pinmap.dir_c,GPIO.LOW)
        # total_pulse=(int)((.3)*Pinmap.number_pulse_zc)
        total_pulse=300
        for i in range(total_pulse):
            self.pulse(Pinmap.pulse_c,_durationsleep=Pinmap.delay_c)    
            
    def closeclamp(self):
        GPIO.output(Pinmap.dir_c,GPIO.HIGH)
        # total_pulse=(int)((.8)*Pinmap.number_pulse_zc)
        total_pulse=300
        for i in range(total_pulse):
            self.pulse(Pinmap.pulse_c,_durationsleep=Pinmap.delay_z)    

    def store_leaf(self,result):
        self.move_x(650,result)
        # move_z_down(350)
        time.sleep(.5)
        self.openclamp()
        time.sleep(1)
        self.closeclamp()
        time.sleep(.5)
        #  time.sleep(.5)
        # move_z_up()
    
    def move_x(self,distance,result):
        distance=distance-0
        print("current position x = {} mm ".format(result[0]))
        if(result[0]>distance):
            flag=True
            max_dis=distance-Pinmap.reach_thresh
            min_dis=distance+Pinmap.reach_thresh
            GPIO.setup(Pinmap.dir_x,GPIO.OUT)
            time.sleep(0.001)
            GPIO.output(Pinmap.dir_x,GPIO.LOW)
        else:
            flag=False
            max_dis=distance+Pinmap.reach_thresh
            min_dis=distance-Pinmap.reach_thresh
            GPIO.setup(Pinmap.dir_x,GPIO.OUT)
            time.sleep(0.001)
            GPIO.output(Pinmap.dir_x,GPIO.HIGH)
            
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
                self.pulse(Pinmap.pulse_x,1000)
                # print("current position x = {} mm ".format(result[0]))
                
            if(diff>60):
                self.pulse(Pinmap.pulse_x,200)
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
                self.pulse(Pinmap.pulse_y,1000)
                # print("current position y = {} mm ".format(result[1]))
                
            if(diff>60):
                self.pulse(Pinmap.pulse_y,300)
                # print("current position  y = {} mm ".format(result[1]))
            if(diff<0):
                print("Exceeded position y = {} mm".format(result[1]))
                break
            # time.sleep(.0001)
                
        
    def check_accuracy_x(self,distance,result):
        self.move_x(distance,result)            
    def check_accuracy_y(self,distance,result):
        self.move_y(distance,result)  
        
    def move_z_down(self,distance):
        GPIO.output(Pinmap.dir_z,GPIO.HIGH)
        step=(distance-880)/(125*2)
        pulse_number=(int)(step*Pinmap.number_pulse_zc)
        self.z_pulse_number=pulse_number
        for i in range(pulse_number):
            self.pulse(Pinmap.pulse_z,500)
        
            
    def change_add_sensor(self):
        
        
        GPIO.output(Pinmap.x_sensor_shut,GPIO.LOW)
        GPIO.output(Pinmap.y_sensor_shut,GPIO.LOW)
        
        time.sleep(0.05)
        GPIO.output(Pinmap.y_sensor_shut,GPIO.HIGH)
        
        tof = VL53L1X.VL53L1X(i2c_bus=1, i2c_address=Pinmap.addr_current)
        tof.open()
        tof.change_address(Pinmap.addr_desired)
        tof.close()
        time.sleep(0.5)
        GPIO.output(Pinmap.x_sensor_shut,GPIO.HIGH)      
    
        
    def measure_x(self,result,show_flag=False):
        global running
        tof_x = VL53L1X.VL53L1X(i2c_bus=1, i2c_address=Pinmap.addr_current)
        tof_x.open()
        tof_x.set_user_roi(self.roi_x)
        tof_x.set_timing(self.timing_budget,self.inter_measurement_time)
        tof_x.start_ranging(1)
        
        while self.running:
            result[1] = tof_x.get_distance()
            if(show_flag):
                print(f"Distance X = {result[0]}")
            time.sleep(0.3)
        tof_x.stop_ranging()
            
            
    def measure_y(self,result,show_flag=False):
        
        global running
        tof_y = VL53L1X.VL53L1X(i2c_bus=1, i2c_address=Pinmap.addr_desired)
        tof_y.open()
        tof_y.set_user_roi(self.roi_y)
        tof_y.set_timing(self.timing_budget,self.inter_measurement_time)
        tof_y.start_ranging(1)
        try:
            while self.running:
                result[0] = tof_y.get_distance()
                if(show_flag):
                    print(f"Distance Y = {result[1]}")
                time.sleep(0.3)
            tof_y.stop_ranging()
        except Exception as e:
            print(e)        
    
    def demo_cut(self,z_dis,delay_time=0.5):
        self.move_z_up()
        self.openclamp()
        time.sleep(delay_time)
        self.move_z_down(z_dis)
        time.sleep(delay_time)
        self.move_z_up()
        time.sleep(delay_time)
        self.closeclamp()
        # closeclamp()
        time.sleep(delay_time)
        self.store_leaf()
        # GPIO.cleanup()
        time.sleep(delay_time)
    
    def cut_operation(self,z_dis,result,delay_time=0.5):
        # self.move_z_up()
        self.openclamp()
        time.sleep(delay_time)
        self.move_z_down(z_dis)
        time.sleep(delay_time)
        self.closeclamp()
        time.sleep(delay_time)
        self.move_z_up()
        time.sleep(delay_time)       
        self.store_leaf(result)
        # GPIO.cleanup()
        time.sleep(delay_time)
        
        
    def move_to_position(self,values,result):
        self.move_x(values[0],result)
        time.sleep(0.5)
        self.check_accuracy_x(values[0],result)
        time.sleep(0.5)
        self.move_y(values[1],result)
        time.sleep(0.5)
        self.check_accuracy_y(values[1],result)
        
    def __del__(self):
        print("RPI Controller object desctructed")
        self.running=False
        GPIO.cleanup()
        
    def limit_switch_check(self):
        while(1):
            print("Front limit switch value = " + str(GPIO.input(Pinmap.x_limit)))
            print("Rear limit switch value = " + str(GPIO.input(Pinmap.x_limit_extra)))
            print("Left limit switch value = " + str(GPIO.input(Pinmap.y_limit)))
            print("Right limit switch value = " + str(GPIO.input(Pinmap.y_limit_extra)))
            print("Z limit switch value = " + str(GPIO.input(Pinmap.z_limit)))
            print("-------------------")
            time.sleep(.5)
    
    def sensor_restart(self):
        GPIO.output(Pinmap.x_sensor_shut,GPIO.LOW)
        GPIO.output(Pinmap.y_sensor_shut,GPIO.LOW)
        time.sleep(2)
        GPIO.output(Pinmap.x_sensor_shut,GPIO.HIGH)
        GPIO.output(Pinmap.y_sensor_shut,GPIO.HIGH) 
             
    
     
        
    
