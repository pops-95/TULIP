from mqtt_utils import Mqtt_Node
import threading
from GPIO_controller import *


# global data
prev_size=0
size=0
# data=[]
broker = '192.168.206.204'
port = 1883

def offset(x_offset,y_offset,values):
    values[0]=values[0]+x_offset
    values[1]=values[1]+y_offset
    return values

def exit_handler(signal, frame):
    global running
    controller.running = False
    
    print()
    sys.exit(0)


# Attach a signal handler to catch SIGINT (Ctrl+C) and exit gracefully
signal.signal(signal.SIGINT, exit_handler)

if __name__=="__main__":
        client=Mqtt_Node(port,broker)
        client.connect_mqtt()
        client.subscribe(client.coordinate_topic) 
        client.subscribe(client.ack_topic)
        client.subscribe(client.running_topic)
        client.start_listining()
        
        controller=RPI_controller()
        controller.gpio_init()
        # controller.change_add_sensor()
        controller.sensor_restart()
        result=multiprocessing.Array('i',[0]*2)
        
        x1=multiprocessing.Process(target=controller.measure_x,args=(result,))
        x1.start()
        y1=multiprocessing.Process(target=controller.measure_y,args=(result,))
        y1.start()
        time.sleep(3)      
        # controller.origin()  
        
        #run until gui is on in jetson orin
        while(not client.ack_flag):
            # client.publish("Start",client.ack_topic)
            continue
              
        client.publish("Started Successfully",client.ack_topic)
        client.ack_flag=False            
        print("out of continuous loop")
        
        while(1):
            try:
                if(len(client.data)>0):
                    coordinates=client.get_values(client.data)
                    print(coordinates)
                    # while(not client.ack_flag):
                    #     continue
                    client.ack_flag=False
                    for i in range(int(len(coordinates)/3)):
                        values=[coordinates[i*3],coordinates[(i*3)+1],coordinates[(i*3)+2]]
                        values=offset(-5,55,values)
                        print(values)
                        controller.move_to_position(values,result)
                        # if (client.demo_run_flag):
                        #     controller.demo_cut(values[2],result)
                        # else:
                        time.sleep(1)
                        controller.cut_operation(values[2],result)
                        time.sleep(2)
                    client.publish("Operation Done. Going to origin",client.ack_topic)      
                    print("Operation Done. Going to origin")
                    # controller.origin()
                    client.data.clear()
  
            except Exception as e:
                print(e)
                if(e.args[0]=="Stop"):
                    # controller.origin()
                    client.publish("RPI Stopped",client.running_topic)
                    client.ack_flag=False
                    client.data.clear()
                
            
            
            
            
        
            
    