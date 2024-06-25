import threading
from GPIO_controller import *

def exit_handler(signal, frame):
    global running
    controller.running = False
    
    print()
    sys.exit(0)

def offset(x_offset,y_offset,values):
    values[0]=values[0]+x_offset
    values[1]=values[1]+y_offset
    return values
    

# Attach a signal handler to catch SIGINT (Ctrl+C) and exit gracefully
signal.signal(signal.SIGINT, exit_handler)

def check_distance(result):
    # global running
    while(1):
        print("x = {} mm and y = {} mm".format(result[0],result[1]))
        time.sleep(.5)
        

if __name__=="__main__":
    result=multiprocessing.Array('i',[0]*2)
    controller=RPI_controller()
    controller.gpio_init()
    # controller.change_add_sensor()
    # GPIO.output(Pinmap.y_sensor_shut,GPIO.HIGH)
    controller.sensor_restart()
    x1=multiprocessing.Process(target=controller.measure_x,args=(result,))
    x1.start()
    y1=multiprocessing.Process(target=controller.measure_y,args=(result,))
    y1.start()
    time.sleep(3)  
    # # controller.move_z_up()
    # # controller.limit_switch_check()
    check_distance(result)
    # coordinates=[416,373,1302,422,261,1322]
    # for i in range(int(len(coordinates)/3)):
            
    #         values=[coordinates[i*3],coordinates[(i*3)+1],coordinates[(i*3)+2]]
    #         values=offset(-5,55,values)
    #         print(values)
    #         controller.move_to_position(values,result)
    #         time.sleep(1)
            
    #         controller.cut_operation(values[2])
            
    #         time.sleep(1)
            # controller.move_x_front()
            # time.sleep(1)
     
            
    

