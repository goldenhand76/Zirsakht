import paho.mqtt.client as mqtt
from django import template
register = template.Library()

MQTT_USERNAME = 'leo'
MQTT_PASSWORD = 'Goldenhand76'
MQTT_HOST = 'localhost'
MQTT_PORT = 1883
MQTT_KEEP_ALIVE = 60

prefix = "/IoTmanager"
deviceId = ["AC1", "AC2", "AC3", "ROOM611"]
topics = ['angle', 'temp', 'set_temp', 'season', 'speed', 'motor']

client = mqtt.Client()
client.username_pw_set(username=MQTT_USERNAME, password=MQTT_PASSWORD)
client.connect(host=MQTT_HOST, port=MQTT_PORT, keepalive=MQTT_KEEP_ALIVE)
client.subscribe(prefix)
for d in deviceId:
    for t in topics:
        client.subscribe(prefix + '/' + d + '/' + t + '/status')
        client.subscribe(prefix + '/' + d + '/' + t + '/control')

