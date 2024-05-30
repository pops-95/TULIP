
from mqtt_utils import *


# global data
prev_size=0
size=0
# data=[]
broker = '10.240.9.11'
port = 1883




if __name__ == '__main__':
        # print('done')
        count=0
        values=[1]
        client=Mqtt_Node(port,broker)
        client.connect_mqtt()
        client.subscribe(client.coordinate_topic)
        client.start_listining()
        while 1:
            if(len(client.data)!=0):
                size=len(client.data)
               
                if(size > prev_size):
                    values=client.get_values()
                    print(values)
                    count=count+1
                    if(count%3==0):
                        client.publish("Error","error")
                    prev_size=0
                
            continue