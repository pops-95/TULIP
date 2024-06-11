import random
import json
from paho.mqtt import client as mqtt_client


class Mqtt_Node():
    def __init__(self,port,broker):
        self.port=port
        self.broker=broker
        self.client_id=f'publish-{random.randint(0, 1000)}'
        self.coordinate_topic="tulip/coordinate"
        self.error_topic="tulip/error"
        self.ack_topic="tulip/ack"
        self.running_topic="tulip/running"
        self.data=[]
        self.ack_flag=False
        self.demo_run_flag=False
        self.data_received_flag=False

    def connect_mqtt(self):
        def on_connect(client, userdata, flags, rc,properties):
            if rc == 0:
                print("Connected to MQTT Broker!")
            else:
                print("Failed to connect, return code %d\n", rc)
        self.client = mqtt_client.Client(mqtt_client.CallbackAPIVersion.VERSION2, self.client_id)
        # client.username_pw_set(username, password)
        self.client.on_connect = on_connect
        self.client.connect(self.broker, self.port)
        
    def publish(self,value,topic):
        if(topic==self.coordinate_topic):
            msg=f"coordinates:{value[0]},{value[1]},{value[2]}"
            
        if(topic==self.error_topic):
            msg=f"Error: {value}"
        
        if(topic==self.ack_topic):
            msg=f"Ack:{value}"
           

        result = self.client.publish(topic, msg)
        # result: [0, 1]
        status = result[0]
        if status == 0:
            print(f"Send `{msg}` to topic `{topic}`")
        else:
            print(f"Failed to send message to topic {topic}")
        

    def start_listining(self):
        self.client.loop_start()
    
    def stop_listining(self):
        self.client.loop_stop()


    def subscribe(self,topic):
        
        def on_message(client, userdata, msg):
            # if(len(self.data)>0):
            #     self.data.clear()
            self.check_value(msg.payload.decode())
            # self.data.append(msg.payload.decode())
            # data.append(msg.payload.decode())

        self.client.subscribe(topic)
        self.client.on_message = on_message

    def get_values(self,cor_values):
        print(self.data)
        # print(split_string[1])
        number_list = json.loads(cor_values)
        # Print the list
        # print(number_list)
        return number_list
    
    def check_value(self,msg):
        split_string=msg.split(':')
        if(split_string[0]=="coordinates"):
            print("ok")
            if(len(self.data)>0):
                self.data.clear()
            self.data.append(split_string[1])
            #do something
        if(split_string[0]=="Ack"):
            # print("Ack")
            if(split_string[1]=="yes"):
                self.ack_flag=True
            else:
                self.ack_flag=False
        
        if(split_string[0]=="Run"):
            # print("Ack")
            if(split_string[1]=="yes"):
                self.demo_run_flag=True
            else:
                self.demo_run_flag=False
        
                #do something
        if(split_string[0]=="Running"):
            if(split_string[1]=="stop"):
                raise Exception("Stop")
        
        