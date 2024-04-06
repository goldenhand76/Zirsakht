#include <ESP8266WiFi.h>
/*
  IoT Manager mqtt device client
  https://play.google.com/store/apps/details?id=ru.esp8266.iotmanager
  https://itunes.apple.com/us/app/iot-manager/id1155934877
  Based on Basic MQTT example with Authentication
  Tested with Arduino IDE   1.6.12 http://arduino.cc
  ESP8266 Community Edition 2.3.0  https://github.com/esp8266/Arduino
  PubSubClient library      1.91.1 https://github.com/Imroy/pubsubclient
  sketch version : 2.0
  IoT Manager    : any version
  toggle, range and small-badge widgets demo
*/
#include <PubSubClient.h>

const char *ssid =  "IOT";            // cannot be longer than 32 characters!
const char *pass =  "Thereisnoway97";       // WiFi password

String prefix   = "/IoTmanager";      // global prefix for all topics - must be some as mobile device
String deviceId = "dev02-bedroom";    // thing Id - unique device id in our project

WiFiClient wclient;


const char* mqttServerName = "217.219.81.130";           
const int    mqttport = 1883;                       
const char* mqttuser =  "leo";                          
const char* mqttpass =  "Goldenhand76";                          
PubSubClient client(wclient);
#define PUB_DELAY 0


String val;
int timer10s, timer1s, timeNow;
int newValue;
bool needPubConfig, needControlGPIO;


String topic = "";
String message = "";

const String stat1 = "{\"status\":\"1\"}";
const String stat0 = "{\"status\":\"0\"}";

const int nWidgets = 8;
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
  page  [0] = "Bedroom1";
  pageId[0] = 1;                                           // pageId always start with 1
  descr [0] = "Bedroom light-0";
  widget[0] = "toggle";
  pin[0] = 4;                                              // GPIO4 - toggle
  defaultVal[0] = 1;                                       // defaultVal status
  inverted[0] = true;
  sTopic[0]   = prefix + "/" + deviceId + "/light0";
  color[0]   = "\"color\":\"red\"";                       // black, blue, green, orange, red, white, yellow (off - grey)

  id    [1] = "1";
  page  [1] = "Bedroom1";
  pageId[1] = 1;
  descr [1] = "Bedroom light-1";
  widget[1] = "toggle";
  pin[1] = 5;                                              // GPIO5 - toggle
  defaultVal[1] = 1;                                       // defaultVal status
  inverted[1] = true;
  sTopic[1] = prefix + "/" + deviceId + "/light1";
  color [1] = "\"color\":\"yellow\"";                      // black, blue, green, orange, red, white, yellow (off - grey)

  id    [2] = "2";
  page  [2] = "Bedroom1";
  pageId[2] = 1;
  descr [2] = "Bedroom light-2";
  widget[2] = "toggle";
  pin[2] = 0;                                              // GPIO0 - toggle
  defaultVal[2] = 1;                                       // defaultVal status
  inverted[1] = true;
  sTopic[2]   = prefix + "/" + deviceId + "/light2";
  color [2] = "\"color\":\"green\"";                       // black, blue, green, orange, red, white, yellow (off - grey)

  id    [3] = "3";
  page  [3] = "Bedroom2";
  pageId[3] = 2;
  descr [3] = "ADC";
  widget[3] = "small-badge";
  pin   [3] = A0;                                          // ADC
  sTopic[3] = prefix + "/" + deviceId + "/ADC";
  badge [3] = "\"badge\":\"badge-calm\"";                  // see http://ionicframework.com/docs/components/#colors
  style [3]   = "\"style\":\"font-size:150%;\"";

  id    [4] = "4";
  page  [4] = "Bedroom2";
  pageId[4] = 2;
  descr [4] = "Bedroom light-4";
  widget[4] = "toggle";
  pin   [4] = 0;                                            // GPIO0 - toggle
  defaultVal[4] = 1;                                        // defaultVal status
  inverted[4] = true;
  sTopic[4]   = prefix + "/" + deviceId + "/light4";
  color [4] = "\"color\":\"orange\"";                       // black, blue, green, orange, red, white, yellow (off - grey)

  // RED
  id    [5] = "5";
  page  [5] = "Bedroom3";
  pageId[5] = 3;
  descr [5] = "Bedroom RED";
  widget[5] = "range";
  pin   [5] = 15;                                          // GPIO15 - range
  defaultVal[5] = 0;                                       // defaultVal 0%, not inverted
  sTopic[5]   = prefix + "/" + deviceId + "/red";
  style[5]   = "\"style\":\"range-assertive\"";            // see http://ionicframework.com/docs/components/#colors
  badge[5]   = "\"badge\":\"badge-assertive\"";            // see http://ionicframework.com/docs/components/#colors

  // GREEN
  id    [6] = "6";
  page  [6] = "Bedroom3";
  pageId[6] = 3;
  descr [6] = "Bedroom GREEN";
  widget[6] = "range";
  pin   [6] = 12;                                      // GPIO12 - range
  defaultVal[6] = 0;                                       // defaultVal 0%
  sTopic[6]   = prefix + "/" + deviceId + "/green";
  style [6]   = "\"style\":\"range-balanced\"";         // see http://ionicframework.com/docs/components/#colors
  badge [6]   = "\"badge\":\"badge-balanced\"";          // see http://ionicframework.com/docs/components/#colors

  // BLUE
  id    [7] = "7";
  page  [7] = "Bedroom3";
  pageId[7] = 3;
  descr [7] = "Bedroom BLUE";
  widget[7] = "range";
  pin[7] = 13;                                          // GPIO13 - range
  defaultVal[7] = 0;                                    // defaultVal status 0%
  sTopic[7]  = prefix + "/" + deviceId + "/blue";
  style [7]   = "\"style\":\"range-calm\"";              // see http://ionicframework.com/docs/components/#colors
  badge [7]   = "\"badge\":\"badge-calm\"";              // see http://ionicframework.com/docs/components/#colors

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

  // room 1
  thing_config[0] = "{\"id\":\"" + id[0] + "\",\"page\":\"" + page[0] + "\",\"pageId\":\"" + pageId[0] + "\",\"descr\":\"" + descr[0] + "\",\"widget\":\"" + widget[0] + "\",\"topic\":\"" + sTopic[0] + "\"," + color[0] + "}"; // GPIO switched On/Off by mobile widget toggle
  thing_config[1] = "{\"id\":\"" + id[1] + "\",\"page\":\"" + page[1] + "\",\"pageId\":\"" + pageId[1] + "\",\"descr\":\"" + descr[1] + "\",\"widget\":\"" + widget[1] + "\",\"topic\":\"" + sTopic[1] + "\"," + color[1] + "}"; // GPIO switched On/Off by mobile widget toggle
  thing_config[2] = "{\"id\":\"" + id[2] + "\",\"page\":\"" + page[2] + "\",\"pageId\":\"" + pageId[2] + "\",\"descr\":\"" + descr[2] + "\",\"widget\":\"" + widget[2] + "\",\"topic\":\"" + sTopic[2] + "\"," + color[2] + "}"; // GPIO switched On/Off by mobile widget toggle

  // room 2
  thing_config[3] = "{\"id\":\"" + id[3] + "\",\"page\":\"" + page[3] + "\",\"pageId\":\"" + pageId[3] + "\",\"descr\":\"" + descr[3] + "\",\"widget\":\"" + widget[3] + "\",\"topic\":\"" + sTopic[3] + "\"," + badge[3] + "," + style[3] + "}"; // ADC
  thing_config[4] = "{\"id\":\"" + id[4] + "\",\"page\":\"" + page[4] + "\",\"pageId\":\"" + pageId[4] + "\",\"descr\":\"" + descr[4] + "\",\"widget\":\"" + widget[4] + "\",\"topic\":\"" + sTopic[4] + "\"," + color[4] + "}"; // GPIO switched On/Off by mobile widget toggle

  // room 3
  thing_config[5] = "{\"id\":\"" + id[5] + "\",\"page\":\"" + page[5] + "\",\"pageId\":\"" + pageId[5] + "\",\"descr\":\"" + descr[5] + "\",\"widget\":\"" + widget[5] + "\",\"topic\":\"" + sTopic[5] + "\"," + style[5] + "," + badge[5] + "}"; // GPIO15 R
  thing_config[6] = "{\"id\":\"" + id[6] + "\",\"page\":\"" + page[6] + "\",\"pageId\":\"" + pageId[6] + "\",\"descr\":\"" + descr[6] + "\",\"widget\":\"" + widget[6] + "\",\"topic\":\"" + sTopic[6] + "\"," + style[6] + "," + badge[6] + "}"; // GPIO12 G
  thing_config[7] = "{\"id\":\"" + id[7] + "\",\"page\":\"" + page[7] + "\",\"pageId\":\"" + pageId[7] + "\",\"descr\":\"" + descr[7] + "\",\"widget\":\"" + widget[7] + "\",\"topic\":\"" + sTopic[7] + "\"," + style[7] + "," + badge[7] + "}"; // GPIO13 B

  needPubConfig = true;
}

char* toCharArray(String str) {
  return &str[0];
}

// send confirmation - status message
void pubStatus(String t, String payload) {
   
  if (client.publish(toCharArray(t + "/status") , toCharArray(payload))) {
    Serial.println("Publish new status for " + t + ", value: " + payload);
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

  // ADC

  int x = analogRead(pin[3]);
  val = "{\"status\":\"" + String(x) + "\"}";

  String topic = sTopic[3] + "/status";
  client.publish(toCharArray(topic), toCharArray(val) );  // widget 3

  Serial.println();
  Serial.print("Publish: " + topic + " => ");
  Serial.println(val);

}

void controlGPIO () {

  needControlGPIO = false;

  Serial.println("Get data from subscribed topic " + topic + " => " + message);

  if (topic == sTopic[0] + "/control") {

    if (message.substring(0,1) == "0") {
      newValue = 1;       // inverted
      stats[0] = stat0;
    } else {
      newValue = 0;
      stats[0] = stat1;
    }
    digitalWrite(pin[0], newValue);
    pubStatus(sTopic[0], stats[0]);

  } else if (topic == sTopic[1] + "/control") {

    if (message.substring(0,1) == "0") {
      newValue = 1; // inverted
      stats[1] = stat0;
    } else {
      newValue = 0; // inverted
      stats[1] = stat1;
    }

    digitalWrite(pin[1], newValue);
    pubStatus(sTopic[1], stats[1]);

  } else if (topic == sTopic[2] + "/control") {

    if (message.substring(0,1) == "0") {
      newValue = 1; // inverted
      stats[2] = stat0;
    } else {
      newValue = 0; // inverted
      stats[2] = stat1;
    }
    digitalWrite(pin[2], newValue);
    pubStatus(sTopic[2], stats[2]);

  } else if (topic == sTopic[3] + "/control") {

    // ADC : nothing, display only

  } else if (topic == sTopic[4] + "/control") {

    // nothing, display only

  } else if (topic == sTopic[5] + "/control") {

    stats[5] = setStatus(message.substring(0,3));
    pubStatus(sTopic[5], stats[5]);

  } else if (topic == sTopic[6] + "/control") {

    stats[6] = setStatus(message.substring(0,3));
    pubStatus(sTopic[6], stats[6]);

  } else if (topic == sTopic[7] + "/control") {

    stats[7] = setStatus(message.substring(0,3));
    pubStatus(sTopic[7], stats[7]);

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

  client.setServer(mqttServerName, mqttport);
 
  initVar();

  // Setup console
  Serial.begin(115200);
  delay(10);
  Serial.println();
  Serial.println();
  Serial.println("MQTT client started.");
}

void loop() {
  if (WiFi.status() != WL_CONNECTED) {
    Serial.print("Connecting via WiFi to ");
    Serial.print(ssid);
    Serial.println("...");

    WiFi.begin(ssid, pass);

    if (WiFi.waitForConnectResult() != WL_CONNECTED) {
      return;
    }

    Serial.println("");
    Serial.println("WiFi connect: Success");
    Serial.print("IP address: ");
    Serial.println(WiFi.localIP());
  }

  if (WiFi.status() == WL_CONNECTED) {

    if (!client.connected()) {

      Serial.println("Connecting to MQTT server ...");
      bool success;
      
      success = client.connect("ESP32Client", mqttuser, mqttpass);

      if (success) {

        Serial.println("Connect to MQTT server: Success");

        pubConfig();

        client.setCallback(onMessageArrived);

        client.subscribe(toCharArray(prefix));                                   // for receiving HELLO messages from IoT Manager
        client.subscribe(toCharArray(prefix + "/" + deviceId + "/+/control"));   // for receiving GPIO control messages for ALL widgets

        Serial.println("Subscribe: Success");

      } else {
        Serial.println("Connect to MQTT server: FAIL");
        delay(1000);
      }
    }

    if (client.connected()) {

      if (needPubConfig) {
        pubConfig();
      }

      yield();

      if (needControlGPIO) {
        controlGPIO();
      }

      timeNow = millis();

      if (timeNow - timer1s > 1000) { // every 1 sec

        Serial.print("."); // print programm activity
        timer1s = timeNow;

      }

      if (timeNow - timer10s > 10000) { // every 10 sec

        getSensorData();
        timer10s = timeNow;

      }

      client.loop();

    }
  }
  yield();
}
