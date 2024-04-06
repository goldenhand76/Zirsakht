#include <ArduinoJson.h>
#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <Arduino.h>
#include "DFRobotDFPlayerMini.h"
#include <OneWire.h>
#include <DallasTemperature.h>
#include <SoftwareSerial.h> // ESP8266
#include <PubSubClient.h>


const char* ssid = "Welcome To K.S.T.P";
const char* password = "";
const char* server = "217.219.81.130";
const int port = 1883;
const char* user = "leo";
const char* pass = "Goldenhand76";

const int oneWireBus = 4;  
const int relle = 5;  

char Temp[10];

bool relle_status = false;
boolean connection_flag = true;


OneWire oneWire(oneWireBus);
DallasTemperature sensors(&oneWire);

WiFiClient espClient;
PubSubClient client(espClient);

// 14 D5 is RX of ESP8266, connect to TX of DFPlayer
// 12 D6 is TX of ESP8266, connect to RX of DFPlayer module
SoftwareSerial dfPlaySerial(14, 12); // D5 D6

DFRobotDFPlayerMini myDFPlayer;

void printDetail(uint8_t type, int value);

void setup()
{
  dfPlaySerial.begin(9600);
  myDFPlayer.begin(dfPlaySerial);
  myDFPlayer.play(1);
  delay(2000);
  WiFi.begin(ssid, password);
  Serial.println("Connecting");
  myDFPlayer.play(2);
  
  while(WiFi.status() != WL_CONNECTED) {
    static unsigned long timer1 = millis();
    if (millis() - timer1 > 300000) {
      myDFPlayer.play(2);
      timer1 = millis();
      }
    delay(3000);
  }

  while (!client.connected()){
    Serial.println("Connecting To Server ...");
    if (client.connect("Thermostat_611", user, pass))
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
  
  myDFPlayer.play(3);
  delay(1000);

  Serial.print("Connected to WiFi network with IP Address: ");
  Serial.println(WiFi.localIP());
  
  Serial.begin(115200);
  Serial.println();
  Serial.println(F("DFRobot DFPlayer Mini Demo"));
  Serial.println(F("Initializing DFPlayer ... (May take 3~5 seconds)"));
  Serial.println(F("DFPlayer Mini online."));

  sensors.begin();
  pinMode(relle, OUTPUT);
  
  myDFPlayer.volume(30);  //Set volume value. From 0 to 30
}

void loop()
{
  
  relle_status = false;
  if (digitalRead(relle) == 1){
    relle_status = true;
  }
  
  static unsigned long timer2 = millis();
  static unsigned long timer3 = millis();
  
  if (millis() - timer2 > 5000) {
    sensors.requestTemperatures(); 
    float temp = sensors.getTempCByIndex(0);
    String temp_str = String(temp); 
    temp_str.toCharArray(Temp, temp_str.length() + 1);
    
    delay(50);

    StaticJsonDocument<256> doc;
    doc["room"] = "611";
    doc["temp"] = temp_str;
    doc["status"] = relle_status;

    char buffer[100];
    serializeJson(doc, buffer);

    
    if (client.publish("A1", buffer) == true) {
      Serial.println("Success sending message");
    } 

    if(WiFi.status()== WL_CONNECTED){
      if(connection_flag == false){
        myDFPlayer.play(3);
        connection_flag = true;
      }
    }
    else {
      Serial.println("WiFi Disconnected");
      if (millis() - timer3 > 300000) {
        myDFPlayer.play(4);
        timer3 = millis();
        connection_flag = false;
        }
      delay(1000);
    }
    timer2 = millis();
  }
  
  if (myDFPlayer.available()) {
    printDetail(myDFPlayer.readType(), myDFPlayer.read()); //Print the detail message from DFPlayer to handle different errors and states.
  }
}

void printDetail(uint8_t type, int value){
  switch (type) {
    case TimeOut:
      Serial.println(F("Time Out!"));
      break;
    case WrongStack:
      Serial.println(F("Stack Wrong!"));
      break;
    case DFPlayerCardInserted:
      Serial.println(F("Card Inserted!"));
      break;
    case DFPlayerCardRemoved:
      Serial.println(F("Card Removed!"));
      break;
    case DFPlayerCardOnline:
      Serial.println(F("Card Online!"));
      break;
    case DFPlayerUSBInserted:
      Serial.println("USB Inserted!");
      break;
    case DFPlayerUSBRemoved:
      Serial.println("USB Removed!");
      break;
    case DFPlayerPlayFinished:
      Serial.print(F("Number:"));
      Serial.print(value);
      Serial.println(F(" Play Finished!"));
      break;
    case DFPlayerError:
      Serial.print(F("DFPlayerError:"));
      switch (value) {
        case Busy:
          Serial.println(F("Card not found"));
          break;
        case Sleeping:
          Serial.println(F("Sleeping"));
          break;
        case SerialWrongStack:
          Serial.println(F("Get Wrong Stack"));
          break;
        case CheckSumNotMatch:
          Serial.println(F("Check Sum Not Match"));
          break;
        case FileIndexOut:
          Serial.println(F("File Index Out of Bound"));
          break;
        case FileMismatch:
          Serial.println(F("Cannot Find File"));
          break;
        case Advertise:
          Serial.println(F("In Advertise"));
          break;
        default:
          break;
      }
      break;
    default:
      break;
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
