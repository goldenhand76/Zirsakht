import paho.mqtt.client as mqtt  # import the client1
import time


############
def on_connect(client, userdata, flags, rc):
    print("Connected with result code", rc)
    client.publish('test', 'hello')

def on_message(client, userdata, msg):
    print(msg.topic, msg.payload)

def on_publish(client,userdata,result):
    print("data published \n", client, userdata, result)
    pass
########################################
broker_address = "iotcom.ir"
print("creating new instance")

client = mqtt.Client("P1", True, None, mqtt.MQTTv31)  # create new instance
client.on_connect = on_connect
client.on_message = on_message
client.on_publish = on_publish                          #assign function to callback

client.username_pw_set(username='leo', password='Goldenhand76')

print("connecting to broker")
client.connect(broker_address, port=8883)  # connect to broker
client.loop_start()  # start the loop
client.publish('test', 'hello')
time.sleep(4)  # wait
client.loop_stop()  # stop the loop



