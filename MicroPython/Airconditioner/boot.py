try:
  import usocket as socket
except ImportError:
  import socket
import ubinascii
import urequests as request
import ujson
import network
import esp
esp.osdebug(None)
import gc
gc.collect()

ssid = 'Welcome To K.S.T.P'
password = ''

station = network.WLAN(network.STA_IF)

station.active(True)
station.connect(ssid, password)


while station.isconnected() == False:
  pass

print('Connection successful')
print(station.ifconfig())
mac = ubinascii.hexlify(station.config('mac'), ':').decode()
print(mac)
