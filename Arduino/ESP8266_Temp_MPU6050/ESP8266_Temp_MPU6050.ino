#include <WiFi.h>
#include <WiFiClient.h>
#include <Wire.h>
#include <PubSubClient.h>
#include <OneWire.h>
#include <DallasTemperature.h>
#include <MPU6050.h>
#include <ArduinoJson.h>

const char* ssid = "IOT";
const char* pass = "Thereisnoway97";
const char* server = "217.219.81.130";
const int port = 1883;
const char* username = "leo";
const char* password = "Goldenhand76";

const int oneWireBus = 4;  
const int encoder = 2;
bool flag = false;
int count = 0;

char Pitch[10];
char Temp[10];
char Speed[10];

const int relle = 14;
bool relle_status = false;


MPU6050 mpu;

int SCL_PIN = 22;
int SDA_PIN = 21;

OneWire oneWire(oneWireBus);
DallasTemperature sensors(&oneWire);

WiFiClient espClient;
PubSubClient client(espClient);

void setup() 
{
  Serial.begin(115200);
  sensors.begin();
  WiFi.begin(ssid, pass);

  while (WiFi.status() != WL_CONNECTED){
    delay(500);
    Serial.println("Connecting To Wifi ...");
  }

  Serial.println("Connected To Wifi");
  client.setServer(server, port);
  client.setCallback(callback);

  while (!client.connected()){
    Serial.println("Connecting To Server ...");
    if (client.connect("ESP32Client", username, password))
    {
      Serial.println("Connected To Server");
      client.subscribe("test0", 1);
    } 
    else 
    {
      Serial.print("failed with state ");
      Serial.print(client.state());
      delay(2000);
    }
  }
  
  delay(1);
  
  Serial.println("Initialize MPU6050");

  while(!mpu.beginSoftwareI2C(SCL_PIN,SDA_PIN,MPU6050_SCALE_250DPS, MPU6050_RANGE_2G))
  {
    Serial.println("Could not find a valid MPU6050 sensor, check wiring!");
    delay(500);
  }
  pinMode(relle, OUTPUT);
}

void loop()
{
  relle_status = false;
  if (digitalRead(relle) == 1){
    relle_status = true;
  }
  
  static unsigned long timer = millis();
  
  client.loop();

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

  if (millis() - timer > 1000) {
    
    Vector normAccel = mpu.readNormalizeAccel();
    float pitch = atan2( - normAccel.XAxis, sqrt(normAccel.YAxis * normAccel.YAxis + normAccel.ZAxis * normAccel.ZAxis)) * 57.3;
    String pitch_str = String(pitch); 
    pitch_str.toCharArray(Pitch, pitch_str.length() + 1);
    delay(50);
    
    sensors.requestTemperatures(); 
    float temp = sensors.getTempCByIndex(0);
    String temp_str = String(temp); 
    temp_str.toCharArray(Temp, temp_str.length() + 1);
    delay(50);

    float speed = (count / ((millis() - timer) / 1000));
    String speed_str = String(speed); 
    speed_str.toCharArray(Speed, speed_str.length() + 1);
    delay(50);

    StaticJsonDocument<256> doc;
    doc["device"] = "A1";
    doc["temp"] = temp_str;
    doc["speed"] = speed_str;
    doc["angle"] = pitch_str;
    doc["status"] = relle_status;
    

    char buffer[100];
    serializeJson(doc, buffer);

    if (client.publish("AC", buffer) == true) {
      Serial.println("Success sending message");
    } 
    else {
      if (WiFi.status() != WL_CONNECTED){
        Serial.println("Internet Disconnected !");
        reconnect_wifi();
        timer = millis();
        count = 0;
      } 
      else if (!client.connected()){
        Serial.println("Broker Disconnected !");
        reconnect();
        timer = millis();
        count = 0;
      }
    }
    timer = millis();
    count = 0;
  }
}

void callback(char* topic, byte* message, unsigned int length) {
  Serial.print("Message arrived on topic: ");
  Serial.print(topic);
  Serial.print(". Message: ");
  StaticJsonDocument<256> doc2;
  deserializeJson(doc2, message, length);
  String test = doc2["test"];
  if (test == "ok"){
    digitalWrite(relle, HIGH);
  }
  else {
    digitalWrite(relle, LOW); 
  }
  Serial.println(test);
}

void reconnect() {
  while (!client.connected()) {
    if (WiFi.status() != WL_CONNECTED){
      reconnect_wifi();
    }
    Serial.print("Attempting MQTT connection...");
    if (client.connect("ESP32Client", username, password)){
       Serial.println("ReConnected To Server");
       client.subscribe("test0", 1);
    } 
    else {
      delay(1000);
    }
  }
}

void reconnect_wifi() {
  while (WiFi.status() != WL_CONNECTED){
      WiFi.disconnect();
      delay(100);
      WiFi.begin(ssid, pass);
      delay(1000);
   }
}
