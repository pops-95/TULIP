from mqtt_utils import Mqtt_Node
import threading
from GPIO import *


# global data
prev_size=0
size=0
# data=[]
broker = '192.168.137.58'
port = 1883


if __name__=="__main__":
        client=Mqtt_Node(port,broker)
        client.connect_mqtt()
        client.subscribe(client.coordinate_topic) 
        client.subscribe(client.ack_topic)
        client.subscribe(client.running_topic)
        client.start_listining()
        
        controller=RPI_controller()
        controller.change_add_sensor()
        
        result=multiprocessing.Array('i',[0]*2)
        
        x1=multiprocessing.Process(target=controller.measure_x,args=(result,))
        x1.start()
        y1=multiprocessing.Process(target=controller.measure_y,args=(result,))
        y1.start()
        time.sleep(3)      
        controller.origin()  
        
        #run until gui is on in jetson orin
        while(not client.ack_flag):
            # client.publish("Start",client.ack_topic)
            continue
              
        client.publish("Started Successfully",client.ack_topic)
        client.ack_flag=False            
        print("out of continuous loop")
        
        while(1):
            try:
                if(client.data>0):
                    coordinates=client.get_values(client.data)
                    while(not client.ack_flag):
                        continue
                    client.ack_flag=False
                    for i in range(int(len(coordinates)/3)):
                        values=[coordinates[i*3],coordinates[(i*3)+1],coordinates[(i*3)+2]]
                        controller.move_to_position(values,result)
                        if (client.demo_run_flag):
                            controller.demo_cut(values[2])
                        else:
                            controller.cut_operation(values[2])
                            
                    controller.origin()
  
            except Exception as e:
                print(e)
                if(e.args[0]=="Stop"):
                    controller.origin()
                client.publish("RPI Stopped",client.running_topic)
                client.ack_flag=False
            
            
            
            
            
        
            
    