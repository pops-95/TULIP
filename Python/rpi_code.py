from mqtt_utils import Mqtt_Node
import threading


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
        
        #run until gui is on in jetson orin
        while(not client.ack_flag):
            # client.publish("Start",client.ack_topic)
            continue
        
        client.publish("Started Successfully",client.ack_topic)
            
        print("out of continuous loop")
        
        while(1):
            try:
                print("runing")
            
            except Exception as e:
                client.publish("RPI Stopped",client.running_topic)
            
            
            
            
            
        
            
    