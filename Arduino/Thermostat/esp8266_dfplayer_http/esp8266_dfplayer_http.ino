#include <ESP8266HTTPClient.h>
#include <Arduino_JSON.h>
#include <ESP8266WiFi.h>
#include <WiFiClient.h>

const char* ssid = "Welcome To K.S.T.P";
const char* password = "";

String roomGet = "http://iotcom.ir/monitor/room/subscribe";
String roomPost = "http://iotcom.ir/monitor/room/publish";
String sensorReadings;

#include <Arduino.h>
#include "DFRobotDFPlayerMini.h"
#include <OneWire.h>
#include <DallasTemperature.h>
#include <SoftwareSerial.h> // ESP8266

// GPIO where the DS18B20 is connected to
const int oneWireBus = 4;  
const int relle = 5;  

int temperatureC = 0;

boolean status_flag = true;
boolean connection_flag = true;


// Setup a oneWire instance to communicate with any OneWire devices
OneWire oneWire(oneWireBus);

// Pass our oneWire reference to Dallas Temperature sensor 
DallasTemperature sensors(&oneWire);

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
  static unsigned long timer2 = millis();
  static unsigned long timer3 = millis();
  
  if (millis() - timer2 > 5000) {
    sensors.requestTemperatures(); 
    temperatureC = sensors.getTempCByIndex(0);
    
    Serial.print(temperatureC);
    Serial.println("ºC");
    
    sensorReadings = httpGETRequest(roomGet);
    if (sensorReadings != "{}"){
      JSONVar myObject = JSON.parse(sensorReadings);
      if (JSON.typeof(myObject) == "undefined") {
        return;
      }
     
      JSONVar keys = myObject.keys();
      
      for (int i = 0; i < keys.length(); i++) {
        JSONVar value = myObject[keys[i]];
        const char* string = keys[i];
        String strkey(string);
        Serial.print(strkey);
        Serial.print(" = ");
        Serial.println(value);
      }

      boolean status = myObject["status"];
      
      if (status == true){
        digitalWrite(relle, HIGH);
        if (status_flag != status){
          status_flag = status;
        }
        
      }else if (status == false){
        digitalWrite(relle, LOW);
        if (status_flag != status){
          status_flag = status;
        }
      }
      
      
    }
    if(WiFi.status()== WL_CONNECTED){
      if(connection_flag == false){
        myDFPlayer.play(3);
        connection_flag = true;
      }
      HTTPClient http;
      http.begin(roomPost);

      http.addHeader("Content-Type", "application/json");
      String temp =  String(temperatureC);
      int httpResponseCode = http.POST("{\"number\":\"611\",\"temp\":\"" + temp + "\"}");
     
      Serial.print("HTTP Response code: ");
      Serial.println(httpResponseCode);
        
      http.end();
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

String httpGETRequest(String serverName) {
  HTTPClient http;

  // Your IP address with path or Domain name with URL path 
  http.begin(serverName);

  // Send HTTP POST request
  int httpResponseCode = http.GET();

  String payload = "{}"; 

  if (httpResponseCode>0) {
    Serial.print("HTTP Response code: ");
    Serial.println(httpResponseCode);
    payload = http.getString();
  }
  else {
    Serial.print("Error code: ");
    Serial.println(httpResponseCode);
  }
  // Free resources
  http.end();

  return payload;
}
