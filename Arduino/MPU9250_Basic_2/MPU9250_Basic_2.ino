#include <Wire.h>
#include <WiFi.h>
#include <PubSubClient.h>
#include "MPU9250.h"
#include <ArduinoJson.h>

const char* ssid = "IOT";
const char* pass = "Thereisnoway97";
const char* server = "217.219.81.130";
const int port = 1883;
const char* username = "leo";
const char* password = "Goldenhand76";
const char* config   = "/IoTmanager/config";

char Roll[10];

WiFiClient espClient;
PubSubClient client(espClient);

MPU9250 IMU(i2c0, 0x68);
int status;

float accPitch = 0 ; 
float accRoll = 0 ;



void callback(char* topic, byte* message, unsigned int length) {
  Serial.print("Message arrived on topic: ");
  Serial.print(topic);
  Serial.print(". Message: ");
  String messageTemp;

  for (int i = 0; i < length; i++) 
  {
    Serial.print((char)message[i]);
    messageTemp += (char)message[i];
  }
  
  if ( messageTemp == "HELLO" ) {  // handshaking
    Serial.println("Shaking !");
    pubConfig();
  }
}



void setup()
{
  Serial.begin(115200);
  WiFi.begin(ssid, pass);

  while (WiFi.status() != WL_CONNECTED){
    delay(500);
    Serial.println("Connecting To Wifi ...");
  }

  
  Serial.println("Connected To Wifi");
  client.setServer(server, port);
  client.setCallback(callback);


  while (!client.connected())
  {
    Serial.println("Connecting To Server ...");
    if (client.connect("ESP32Client", username, password))
    {
      Serial.println("Connected To Server");
      client.subscribe("/IoTmanager");
      client.subscribe("A1");
    } 
    else 
    {
      Serial.print("failed with state ");
      Serial.print(client.state());
      delay(2000);
    }
  }

  
  status = IMU.begin();
  if (status < 0)
  {
    Serial.println("IMU initialization unsuccessful");
    Serial.println("Check IMU wiring or try cycling power");
    Serial.print("Status: ");
    Serial.println(status);
    while (1)
    {
    }
  }
  // setting the accelerometer full scale range to +/-8G
  IMU.setAccelRange(MPU9250::ACCEL_RANGE_2G);
  // setting the gyroscope full scale range to +/-500 deg/s
  IMU.setGyroRange(MPU9250::GYRO_RANGE_250DPS);
  // setting DLPF bandwidth to 20 Hz
  IMU.setDlpfBandwidth(MPU9250::DLPF_BANDWIDTH_41HZ);
  // setting SRD to 19 for a 50 Hz update rate
  // IMU.setSrd(19);
  IMU.calibrateMag();
}

#define RAD_TO_DEG 57.295779513082320876798154814105

void loop()
{
  client.loop();
  
  IMU.readSensor();

  accRoll = atan2(IMU.getAccelY_mss(), IMU.getAccelZ_mss())*RAD_TO_DEG;
  String roll_str = String(accRoll);
  roll_str.toCharArray(Roll, roll_str.length() + 1);

  Serial.println(roll_str);
  
  delay(100);
}

void pubConfig() {
  StaticJsonDocument<1024> doc;
  doc["widget"] = "anydata";
  doc["descr"] = "Anydata";
  doc["topic"] = "/IoTmanager/demo/anydata2";
  doc["after"] = "DEG";
  doc["status"] = 123;
  char buffer[100];
  serializeJson(doc, buffer);
  Serial.println(buffer);
  
  bool success;
  success = client.publish(config, buffer);
  if (success) {
     Serial.println("Publish config: Success ()");
  } else {
     Serial.println("Publish config FAIL! ()");
     delay(1000);
     ESP.restart();
  }
}
