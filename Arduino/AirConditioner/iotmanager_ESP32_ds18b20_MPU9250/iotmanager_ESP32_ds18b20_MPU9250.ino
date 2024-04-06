#include <Wire.h>
#include <WiFi.h>
#include <PubSubClient.h>
#include "MPU9250.h"
#include <OneWire.h>
#include <DallasTemperature.h>

char Roll[10];

MPU9250 IMU(i2c0, 0x68);
int status;

float accPitch = 0 ; 
float accRoll = 0 ;

const char *ssid =  "Welcome To K.S.T.P";            // cannot be longer than 32 characters!
const char *pass =  "";       // WiFi password

const String prefix   = "/IoTmanager";      // global prefix for all topics - must be some as mobile device
const char*  deviceId = "AC1";    // thing Id - unique device id in our project

const char* mqttServerName = "217.219.81.130";
const int   mqttport = 1883;         
const char* mqttuser =  "leo";
const char* mqttpass =  "Goldenhand76";

unsigned long timer0 = millis();

WiFiClient wclient;                        
PubSubClient client(wclient);
#define PUB_DELAY 0


const int oneWireBus = 4;  
OneWire oneWire(oneWireBus);
DallasTemperature sensors(&oneWire);

const int encoder = 2;
bool flag = false;
int count = 0;

String val;
int last_time = 0;
int newValue;
bool needPubConfig, needControlGPIO;

long lastReconnectAttempt = 0;

String topic = "";
String message = "";

const String stat1 = "{\"status\":\"1\"}";
const String stat0 = "{\"status\":\"0\"}";

const int nWidgets = 4;
String stats       [nWidgets];
String sTopic      [nWidgets];
String color       [nWidgets];
String style       [nWidgets];
String badge       [nWidgets];
String widget      [nWidgets];
String descr       [nWidgets];
String page        [nWidgets];
String pageId      [nWidgets];
String thing_config[nWidgets];
String id          [nWidgets];
String icon        [nWidgets];
int    pin         [nWidgets];
int    defaultVal  [nWidgets];
bool   inverted    [nWidgets];

String setStatus ( String s ) {
  String stats = "{\"status\":\"" + s + "\"}";
  return stats;
}
String setStatus ( int s ) {
  String stats = "{\"status\":\"" + String(s) + "\"}";
  return stats;
}


void initVar() {
  id    [0] = "0";
  page  [0] = "AC1";
  pageId[0] = 1;                                           // pageId always start with 1
  descr [0] = "Angle";
  widget[0] = "anydata";
  defaultVal[0] = 0;
  inverted[0] = false;
  sTopic[0] = prefix + "/" + deviceId + "/angle";
  color [0] = "\"after\":\"DEG\"";     
  icon  [0] = "\"icon\":\"triangle\"";

  id    [1] = "1";
  page  [1] = "AC1";
  pageId[1] = 1;
  descr [1] = "Temp";
  widget[1] = "anydata";
  defaultVal[1] = 0;
  inverted[1] = false;
  sTopic[1] = prefix + "/" + deviceId + "/temp";
  color [1] = "\"after\":\"°C\"";   
  icon  [1] = "\"icon\":\"thermometer\"";

  id    [2] = "2";
  page  [2] = "AC1";
  pageId[2] = 1;
  descr [2] = "Speed";
  widget[2] = "anydata";
  defaultVal[2] = 0;
  inverted[2] = false;
  sTopic[2]   = prefix + "/" + deviceId + "/speed";
  color [2] = "\"after\":\"RPS\"";  
  icon  [2] = "\"icon\":\"speedometer\"";

  id    [3] = "3";
  page  [3] = "AC1";
  pageId[3] = 1;
  descr [3] = "Motor";
  widget[3] = "toggle";
  pin[3] = 0;                                              // GPIO0 - toggle
  defaultVal[3] = 1;                                       // defaultVal status
  inverted[3] = true;
  sTopic[3]   = prefix + "/" + deviceId + "/motor";
  color [3] = "\"color\":\"green\""; 
  icon  [3] = "\"icon\":\"power\", \"iconOff\":\"power-outline\"";


  for (int i = 0; i < nWidgets; i++) {
    if (inverted[i]) {
      if (defaultVal[i] > 0) {
        stats[i] = setStatus(0);
      } else {
        stats[i] = setStatus(1);
      }
    } else {
      stats[i] = setStatus(defaultVal[i]);
    }
  }
  
  thing_config[0] = "{\"id\":\"" + id[0] + "\",\"page\":\"" + page[0] + "\",\"pageId\":\"" + pageId[0] + "\",\"descr\":\"" + descr[0] + "\",\"widget\":\"" + widget[0] + "\",\"topic\":\"" + sTopic[0] + "\"," + color[0] + "," + icon[0] + "}"; // GPIO switched On/Off by mobile widget toggle
  thing_config[1] = "{\"id\":\"" + id[1] + "\",\"page\":\"" + page[1] + "\",\"pageId\":\"" + pageId[1] + "\",\"descr\":\"" + descr[1] + "\",\"widget\":\"" + widget[1] + "\",\"topic\":\"" + sTopic[1] + "\"," + color[1] + "," + icon[1] + "}"; // GPIO switched On/Off by mobile widget toggle
  thing_config[2] = "{\"id\":\"" + id[2] + "\",\"page\":\"" + page[2] + "\",\"pageId\":\"" + pageId[2] + "\",\"descr\":\"" + descr[2] + "\",\"widget\":\"" + widget[2] + "\",\"topic\":\"" + sTopic[2] + "\"," + color[2] + "," + icon[2] + "}"; // GPIO switched On/Off by mobile widget toggle
  thing_config[3] = "{\"id\":\"" + id[3] + "\",\"page\":\"" + page[3] + "\",\"pageId\":\"" + pageId[3] + "\",\"descr\":\"" + descr[3] + "\",\"widget\":\"" + widget[3] + "\",\"topic\":\"" + sTopic[3] + "\"," + color[3] + "," + icon[3] + "}"; // ADC

  needPubConfig = true;
}

char* toCharArray(String str) {
  return &str[0];
}


void pubStatus(String t, String payload) {
   
  if (client.publish(toCharArray(t + "/status") , toCharArray(payload))) {
    Serial.println("Publish new status for " + t + ", status: " + payload);
  } else {
    Serial.println("Publish new status for " + t + " FAIL!");
  }

  delay(PUB_DELAY);
} 

void pubConfig() {

  needPubConfig = false;

  bool success;
  for (int i = 0; i < nWidgets; i = i + 1) {

    success = client.publish(toCharArray(prefix + "/" + deviceId + "/config"), toCharArray(thing_config[i]));
    if (success) {
      Serial.println("Publish config: Success (" + thing_config[i] + ")");
    } else {
      Serial.println("Publish config FAIL! ("    + thing_config[i] + ")");
    }

    delay(PUB_DELAY);
    pubStatus(sTopic[i], stats[i]);

  }
  yield();
}

void getSensorData() {

  IMU.readSensor();
  accRoll = atan2(IMU.getAccelY_mss(), IMU.getAccelZ_mss())*RAD_TO_DEG;
  stats[0] = setStatus(accRoll);
  pubStatus(sTopic[0], stats[0]);
  

  sensors.requestTemperatures(); 
  float temp = sensors.getTempCByIndex(0);
  stats[1] = setStatus(temp);
  pubStatus(sTopic[1], stats[1]);
  

  float speed = (count / ((millis() - last_time) / 1000));
  last_time = millis();
  stats[2] = setStatus(speed);
  pubStatus(sTopic[2], stats[2]);

  pubStatus(sTopic[3], stats[3]);
  
  Serial.println();
  delay(50);
  count = 0;

}

void controlGPIO() {

  needControlGPIO = false;
  
  Serial.println("Get data from subscribed topic " + topic + " => " + message);

  if (topic == sTopic[3] + "/control") {

    if (message.substring(0,1) == "0") {
      newValue = 1;
      stats[3] = stat0;
    } 
    else if (message.substring(0,1) == "1") {
      newValue = 0;
      stats[3] = stat1;
    }
    digitalWrite(pin[3], newValue);
    pubStatus(sTopic[3], stats[3]);

  } 
}

void onMessageArrived(char* t, byte* m, unsigned int length) {
 
  topic = String(t);
  message = String((char*)m);

  if (message.substring(0,5) == "HELLO") {
    needPubConfig = true;
  } else {
    needControlGPIO = true;
  }

}

void setup() {
  
  Serial.begin(115200);
  sensors.begin();

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
  IMU.setAccelRange(MPU9250::ACCEL_RANGE_2G);
  IMU.setGyroRange(MPU9250::GYRO_RANGE_250DPS);
  IMU.setDlpfBandwidth(MPU9250::DLPF_BANDWIDTH_41HZ);
  // IMU.setSrd(19);
  IMU.calibrateMag();

  WiFi.begin(ssid, pass);

  while (WiFi.status() != WL_CONNECTED){
    delay(500);
    Serial.println("Connecting To Wifi ...");
  }
  Serial.println("Connected To Wifi");
  WiFi.setAutoReconnect(true);
  WiFi.persistent(true);
  
  client.setServer(mqttServerName, mqttport);
  client.setCallback(onMessageArrived);

  while (!client.connected()){
    Serial.println("Connecting To Server ...");
    if (client.connect(deviceId, mqttuser, mqttpass))
    {
      Serial.println("Connected To Server");
      client.subscribe(toCharArray(prefix));                                   // for receiving HELLO messages from IoT Manager
      client.subscribe(toCharArray(prefix + "/" + deviceId + "/+/control")); 
    } 
    else 
    {
      Serial.print("failed with state ");
      Serial.print(client.state());
      delay(2000);
    }
  }
 
  initVar();

}

#define RAD_TO_DEG 57.295779513082320876798154814105

void loop() {
  static unsigned long timer1 = millis();
  static unsigned long timer2 = millis();

  if (!client.connected()) {
    if (millis() - timer1 > 2000) {
      if (reconnect()) {
        pubConfig();
        timer1 = millis();
      }
      else if (millis() - timer1 > 30000) {
        timer0 = millis();
        if (reconnect_wifi()) {
          timer1 = millis();
        }
      }
    }
  } else {
    timer1 = millis();
    
    if (needPubConfig) {
      pubConfig();
    }
    
    if (needControlGPIO) {
      controlGPIO();
    }
    
    client.loop();
    if (millis() - timer2 > 10000) { // every 10 sec
      getSensorData();
      timer2 = millis();
    }
  }
   
  if (digitalRead(encoder) == HIGH){
    if (flag == false){
        count += 1;
        flag = true;
    }
  }
  else{
    if (flag == true){
        flag = false;
    }
  }
  
}

boolean reconnect() {
  if (client.connect(deviceId, mqttuser, mqttpass)) {
    client.setCallback(onMessageArrived);
    client.subscribe(toCharArray(prefix));
    client.subscribe(toCharArray(prefix + "/" + deviceId + "/+/control"));
  }
  return client.connected();
}

boolean reconnect_wifi(){
  WiFi.disconnect();
  WiFi.begin(ssid, pass);
  while (WiFi.status() != WL_CONNECTED && millis() - timer0 < 5000){
    delay(500);
  }
  return WiFi.status() == WL_CONNECTED;
}
