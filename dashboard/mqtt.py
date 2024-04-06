import json
from json.decoder import JSONDecodeError
from datetime import datetime, timedelta
from django import template
from Zirsakht.mqtt import client
from Zirsakht.influx import connect_database
from requests.exceptions import ReadTimeout

register = template.Library()


def on_connect(client, userdata, flags, rc):
    print("Connected with result code: " + str(rc))


def on_message(client, userdata, msg):
    db = connect_database()
    try:
        if not msg.topic == '/IoTmanager':
            device = msg.topic.split('/')[2]
            measurement = msg.topic.split('/')[3]
            mode = msg.topic.split('/')[4]
            print(mode)
            if mode == 'status':
                status = eval(msg.payload.decode("UTF-8"))["status"]
                print(device, '>>> ', measurement, '>>> ', status)

                query = [f'{measurement},device={device} status={status}']
                write = db.write_points(query, time_precision='s', database='ZIRSAKHT', protocol='line')
                if not write:
                    print("error")

    except JSONDecodeError:
        print("Json Decode Error!")
        pass


client.on_message = on_message
client.on_connect = on_connect
