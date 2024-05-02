import multiprocessing
import os
import signal
import sys
import RPi.GPIO as GPIO
import VL53L1X
import time

running=True
# GPIO.setmode(GPIO.BCM)

# GPIO.setup(4,GPIO.OUT)
# GPIO.setup(17,GPIO.OUT)

# GPIO.output(4,GPIO.LOW)
# GPIO.output(17,GPIO.LOW)

# time.sleep(.05)
# addr_current=0x29
# addr_desired=0x44
# GPIO.output(17,GPIO.HIGH)

# tof = VL53L1X.VL53L1X(i2c_bus=1, i2c_address=addr_current)
# tof.open()
# tof.change_address(addr_desired)
# tof.close()

# GPIO.output(4,GPIO.HIGH)

def x_sensor(result):
    global running
    tof_x = VL53L1X.VL53L1X(i2c_bus=1, i2c_address=addr_current)
    tof_x.open()
    tof_x.set_user_roi(VL53L1X.VL53L1xUserRoi(6, 9, 9, 6))
    tof_x.set_timing(50000,55)
    tof_x.start_ranging(1)
    
    while running:
        result[0] = tof_x.get_distance()


def y_sensor(result):
    
    global running
    tof_y = VL53L1X.VL53L1X(i2c_bus=1, i2c_address=addr_desired)
    tof_y.open()
    tof_y.set_user_roi(VL53L1X.VL53L1xUserRoi(6, 9, 9, 6))
    tof_y.set_timing(50000,55)


    tof_y.start_ranging(1)
    
    while running:
        result[1] = tof_y.get_distance()
    
    
    


def exit_handler(signal, frame):
    global running
    running = False
    
    GPIO.cleanup()
    print()
    sys.exit(0)
    
# Attach a signal handler to catch SIGINT (Ctrl+C) and exit gracefully
signal.signal(signal.SIGINT, exit_handler)
# result = multiprocessing.Array('i', [0] * 2)
# for_x=multiprocessing.Process(target=x_sensor, args=(result,))
# for_x.start()

# for_y=multiprocessing.Process(target=y_sensor, args=(result,))
# for_y.start()


while running:
    for i in range (1000):
        print(i)
        if(i==500):
            break
    break
    print("x = {} mm  and  y = {} mm".format(result[0],result[1]))   
    
    # print("Distance y: {}mm".format(distance_in_mm))
    time.sleep(0.1)





