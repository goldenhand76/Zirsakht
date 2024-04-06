#include <ESP8266WiFi.h>
#include <SPI.h>
#include <Wire.h>
#include <OneWire.h>
#include <DallasTemperature.h>
#include <Adafruit_NeoPixel.h>
#include <PubSubClient.h>

unsigned long timer = millis();

char ssid[] = "IOT";
const char* pass = "Thereisnoway97";

const String prefix  = "/IoTmanager";      // global prefix for all topics - must be some as mobile device
const char* deviceId = "ROOM611";    // thing Id - unique device id in our project

const char* mqttServerName = "217.219.81.130";
const int   mqttport = 1883;         
const char* mqttuser =  "leo";
const char* mqttpass =  "Goldenhand76";

WiFiClient wclient;                        
PubSubClient client(wclient);

#define PUB_DELAY 50
#define LED_PIN   0
#define LED_COUNT 12
#define UP        12
#define relay     13
#define DOWN      14

static const unsigned char PROGMEM wifi_bmp[] ={ 
  B00000000, B00000000,
  B00000000, B00000000,
  B00000000, B00000000,
  B00000111, B11100000,
  B00011000, B00011000,
  B00100000, B00000100,
  B00000111, B11100000,
  B00011000, B00011000,
  B00000001, B10000000,
  B00000110, B01100000,
  B00000000, B00000000,
  B00000001, B10000000,
  B00000001, B10000000,
  B00000000, B00000000,
  B00000000, B00000000,
  B00000000, B00000000
  };

static const unsigned char PROGMEM sunny[] ={
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x03, 0xc0, 0x00, 0x00, 0x00, 0x00, 0x03, 0xc0, 0x00, 0x00, 0x00, 0x00, 
  0x03, 0xc0, 0x00, 0x00, 0x00, 0x00, 0x03, 0xc0, 0x00, 0x00, 0x00, 0x00, 0x03, 0xc0, 0x00, 0x00, 
  0x00, 0x60, 0x03, 0xc0, 0x06, 0x00, 0x00, 0xf0, 0x03, 0xc0, 0x0f, 0x00, 0x00, 0xf8, 0x01, 0x80, 
  0x1f, 0x00, 0x00, 0x7c, 0x00, 0x00, 0x3e, 0x00, 0x00, 0x3e, 0x00, 0x00, 0x7c, 0x00, 0x00, 0x1e, 
  0x00, 0x00, 0x78, 0x00, 0x00, 0x0e, 0x01, 0x80, 0x70, 0x00, 0x00, 0x00, 0x0f, 0xf0, 0x00, 0x00, 
  0x00, 0x00, 0x3f, 0xfc, 0x00, 0x00, 0x00, 0x00, 0x7f, 0xfe, 0x00, 0x00, 0x00, 0x00, 0xff, 0xff, 
  0x00, 0x00, 0x00, 0x00, 0xff, 0xff, 0x00, 0x00, 0x00, 0x01, 0xff, 0xff, 0x80, 0x00, 0x00, 0x01, 
  0xff, 0xff, 0x80, 0x00, 0x1f, 0xc1, 0xff, 0xff, 0x83, 0xf8, 0x1f, 0xe3, 0xff, 0xff, 0xc7, 0xf8, 
  0x1f, 0xe3, 0xff, 0xff, 0xc7, 0xf8, 0x1f, 0xc1, 0xff, 0xff, 0x83, 0xf8, 0x00, 0x01, 0xff, 0xff, 
  0x80, 0x00, 0x00, 0x01, 0xff, 0xff, 0x80, 0x00, 0x00, 0x00, 0xff, 0xff, 0x00, 0x00, 0x00, 0x00, 
  0xff, 0xff, 0x00, 0x00, 0x00, 0x00, 0x7f, 0xfe, 0x00, 0x00, 0x00, 0x00, 0x3f, 0xfc, 0x00, 0x00, 
  0x00, 0x04, 0x0f, 0xf0, 0x20, 0x00, 0x00, 0x0e, 0x01, 0x80, 0x70, 0x00, 0x00, 0x1e, 0x00, 0x00, 
  0x78, 0x00, 0x00, 0x3e, 0x00, 0x00, 0x7c, 0x00, 0x00, 0x7c, 0x00, 0x00, 0x3e, 0x00, 0x00, 0xf8, 
  0x01, 0x80, 0x1f, 0x00, 0x00, 0xf0, 0x03, 0xc0, 0x0f, 0x00, 0x00, 0x60, 0x03, 0xc0, 0x06, 0x00, 
  0x00, 0x00, 0x03, 0xc0, 0x00, 0x00, 0x00, 0x00, 0x03, 0xc0, 0x00, 0x00, 0x00, 0x00, 0x03, 0xc0, 
  0x00, 0x00, 0x00, 0x00, 0x03, 0xc0, 0x00, 0x00, 0x00, 0x00, 0x03, 0xc0, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
  };
  
static const unsigned char PROGMEM partly_sunny[] ={
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0x80, 0x00, 0x00, 0x00, 0x00, 0x03, 0x80, 0x00, 0x00, 0x00, 
  0x00, 0x03, 0x80, 0x00, 0x00, 0x00, 0x00, 0x03, 0x80, 0x00, 0x00, 0x00, 0x0c, 0x03, 0x80, 0x60, 
  0x00, 0x00, 0x1e, 0x03, 0x80, 0xf0, 0x00, 0x00, 0x0f, 0x00, 0x01, 0xe0, 0x00, 0x00, 0x0f, 0x01, 
  0x01, 0xe0, 0x00, 0x00, 0x07, 0x1f, 0xf1, 0xc0, 0x00, 0x00, 0x00, 0x3f, 0xf8, 0x00, 0x00, 0x00, 
  0x00, 0x0f, 0xfc, 0x00, 0x00, 0x00, 0x00, 0x03, 0xfe, 0x00, 0x00, 0x00, 0x3f, 0x80, 0xff, 0x00, 
  0x00, 0x00, 0xff, 0xf0, 0x7f, 0x00, 0x00, 0x03, 0xff, 0xf8, 0x3f, 0x00, 0x00, 0x07, 0xff, 0xfc, 
  0x3f, 0x3f, 0x00, 0x0f, 0xff, 0xfe, 0x1f, 0xbf, 0x00, 0x1f, 0xff, 0xff, 0x0f, 0xbf, 0x00, 0x3f, 
  0xff, 0xff, 0x83, 0x00, 0x00, 0x3f, 0xff, 0xff, 0x80, 0x00, 0x00, 0x7f, 0xff, 0xff, 0xe0, 0x00, 
  0x00, 0x7f, 0xff, 0xff, 0xfc, 0x00, 0x00, 0x7f, 0xff, 0xff, 0xfe, 0x00, 0x00, 0xff, 0xff, 0xff, 
  0xff, 0x00, 0x01, 0xff, 0xff, 0xff, 0xff, 0x80, 0x07, 0xff, 0xff, 0xff, 0xff, 0xc0, 0x1f, 0xff, 
  0xff, 0xff, 0xff, 0xe0, 0x3f, 0xff, 0xff, 0xff, 0xff, 0xe0, 0x7f, 0xff, 0xff, 0xff, 0xff, 0xe0, 
  0x7f, 0xff, 0xff, 0xff, 0xff, 0xe0, 0xff, 0xff, 0xff, 0xff, 0xff, 0xf0, 0xff, 0xff, 0xff, 0xff, 
  0xff, 0xf0, 0xff, 0xff, 0xff, 0xff, 0xff, 0xe0, 0xff, 0xff, 0xff, 0xff, 0xff, 0xe0, 0xff, 0xff, 
  0xff, 0xff, 0xff, 0xe0, 0xff, 0xff, 0xff, 0xff, 0xff, 0xe0, 0x7f, 0xff, 0xff, 0xff, 0xff, 0xc0, 
  0x7f, 0xff, 0xff, 0xff, 0xff, 0x80, 0x3f, 0xff, 0xff, 0xff, 0xff, 0x00, 0x1f, 0xff, 0xff, 0xff, 
  0xfe, 0x00, 0x0f, 0xff, 0xff, 0xff, 0xfc, 0x00, 0x03, 0xff, 0xff, 0xff, 0xe0, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};

static const unsigned char PROGMEM cloudy[] ={
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x7f, 0xe0, 0x00, 0x00, 0x00, 0x01, 0xff, 0xf8, 
  0x00, 0x00, 0x00, 0x03, 0xf0, 0x7c, 0x00, 0x00, 0x00, 0x0f, 0x80, 0x1e, 0x00, 0x00, 0x00, 0x1e, 
  0x00, 0x0f, 0xf0, 0x00, 0x00, 0x1c, 0x00, 0x07, 0xfc, 0x00, 0x00, 0x38, 0x00, 0x00, 0x3f, 0x00, 
  0x00, 0x38, 0x00, 0x00, 0x0f, 0x00, 0x00, 0x30, 0x00, 0x00, 0x03, 0x80, 0x00, 0x70, 0x00, 0x00, 
  0x01, 0xc0, 0x00, 0x70, 0x00, 0x00, 0x01, 0xc0, 0x0f, 0xf0, 0x00, 0x00, 0x00, 0xe0, 0x1f, 0xe0, 
  0x00, 0x00, 0x00, 0xe0, 0x3c, 0x00, 0x00, 0x00, 0x00, 0xe0, 0x38, 0x00, 0x00, 0x00, 0x00, 0x60, 
  0x70, 0x00, 0x00, 0x00, 0xfc, 0x60, 0x70, 0x00, 0x00, 0x03, 0xff, 0xe0, 0x60, 0x00, 0x00, 0x07, 
  0xcf, 0xe0, 0x60, 0x00, 0x00, 0x0f, 0x03, 0xf8, 0x70, 0x00, 0x00, 0x1e, 0x01, 0xfc, 0x70, 0x00, 
  0x00, 0x1c, 0x00, 0x1c, 0x38, 0x00, 0x00, 0x7c, 0x00, 0x0e, 0x3c, 0x00, 0x01, 0xf8, 0x00, 0x0e, 
  0x1f, 0xff, 0xff, 0xe0, 0x00, 0x06, 0x07, 0xff, 0xff, 0x80, 0x00, 0x06, 0x00, 0x00, 0x01, 0x80, 
  0x00, 0x0e, 0x00, 0x00, 0x01, 0x80, 0x00, 0x0e, 0x00, 0x00, 0x01, 0xc0, 0x00, 0x3c, 0x00, 0x00, 
  0x01, 0xff, 0xff, 0xf8, 0x00, 0x00, 0x00, 0xff, 0xff, 0xf0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
  };

static const unsigned char PROGMEM thunderstorm[] ={
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x07, 0xf0, 0x00, 0x00, 0x00, 0x00, 0x3f, 0xfc, 
  0x00, 0x00, 0x00, 0x00, 0xff, 0xff, 0x00, 0x00, 0x00, 0x01, 0xff, 0xff, 0xc0, 0x00, 0x00, 0x03, 
  0xf0, 0x1f, 0xe0, 0x00, 0x00, 0x0f, 0xc0, 0x07, 0xf0, 0x00, 0x00, 0x0f, 0x80, 0x01, 0xf0, 0x00, 
  0x00, 0x1f, 0x00, 0x00, 0xf8, 0x00, 0x00, 0x1e, 0x00, 0x00, 0x78, 0x00, 0x01, 0xfe, 0x00, 0x00, 
  0x7c, 0x00, 0x03, 0xfc, 0x00, 0x00, 0x3c, 0x00, 0x0f, 0xfc, 0x00, 0x00, 0x3e, 0x00, 0x1f, 0xe0, 
  0x00, 0x00, 0x3f, 0xc0, 0x1f, 0x00, 0x00, 0x00, 0x1f, 0xf0, 0x3e, 0x00, 0x00, 0x00, 0x1f, 0xf8, 
  0x3c, 0x00, 0x00, 0x00, 0x01, 0xf8, 0x3c, 0x00, 0x00, 0x00, 0x00, 0x7c, 0x78, 0x00, 0x00, 0x00, 
  0x00, 0x3c, 0x78, 0x00, 0x00, 0x00, 0x00, 0x1e, 0x78, 0x00, 0x00, 0x00, 0x00, 0x1e, 0x78, 0x00, 
  0x00, 0x00, 0x00, 0x1e, 0x3c, 0x00, 0x00, 0x00, 0x00, 0x1e, 0x3c, 0x00, 0x00, 0x00, 0x00, 0x3e, 
  0x3e, 0x00, 0x00, 0x00, 0x00, 0x3c, 0x1f, 0x00, 0x00, 0x00, 0x00, 0x7c, 0x1f, 0xe0, 0x00, 0x00, 
  0x03, 0xf8, 0x0f, 0xff, 0xff, 0xff, 0xff, 0xf8, 0x03, 0xff, 0xff, 0xff, 0xff, 0xe0, 0x01, 0xff, 
  0xff, 0xff, 0xff, 0xc0, 0x00, 0x00, 0x78, 0xf8, 0x00, 0x00, 0x00, 0x38, 0x78, 0xf0, 0x0e, 0x00, 
  0x00, 0x78, 0x78, 0xf0, 0x0e, 0x00, 0x00, 0x78, 0x78, 0xff, 0x1e, 0x00, 0x00, 0xf8, 0x70, 0xff, 
  0x1e, 0x00, 0x00, 0xf0, 0xf0, 0xff, 0x3c, 0x00, 0x01, 0xf0, 0xff, 0x3e, 0x3c, 0x00, 0x01, 0xe0, 
  0xff, 0x7c, 0x78, 0x00, 0x01, 0xe0, 0xff, 0x7c, 0x78, 0x00, 0x00, 0x1e, 0x07, 0xf8, 0x07, 0x80, 
  0x00, 0x1e, 0x07, 0xf0, 0x07, 0x80, 0x00, 0x3c, 0x07, 0xe0, 0x0f, 0x80, 0x00, 0x3c, 0x07, 0xe0, 
  0x0f, 0x00, 0x00, 0x78, 0x07, 0xc0, 0x0f, 0x00, 0x00, 0x78, 0x07, 0x80, 0x0e, 0x00, 0x00, 0x00, 
  0x07, 0x80, 0x00, 0x00, 0x00, 0x00, 0x07, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
  };

static const unsigned char PROGMEM snow[] ={
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0x00, 0x00, 0x00, 0x00, 0x1f, 0xf8, 0x00, 0x00, 0x00, 0x00, 
  0x7c, 0x3c, 0x00, 0x00, 0x00, 0x00, 0xe0, 0x07, 0x00, 0x00, 0x00, 0x01, 0x80, 0x03, 0x80, 0x00, 
  0x00, 0x03, 0x00, 0x01, 0xfc, 0x00, 0x00, 0x06, 0x00, 0x00, 0x1f, 0x00, 0x00, 0x06, 0x00, 0x00, 
  0x03, 0x80, 0x00, 0x0c, 0x00, 0x00, 0x00, 0xc0, 0x00, 0x0c, 0x00, 0x00, 0x00, 0x60, 0x00, 0x0c, 
  0x00, 0x00, 0x00, 0x60, 0x00, 0x78, 0x00, 0x00, 0x00, 0x30, 0x01, 0xf8, 0x00, 0x00, 0x00, 0x30, 
  0x07, 0x00, 0x00, 0x00, 0x00, 0x30, 0x06, 0x00, 0x00, 0x00, 0x00, 0x10, 0x0c, 0x00, 0x00, 0x00, 
  0x00, 0x10, 0x0c, 0x00, 0x00, 0x00, 0x00, 0x30, 0x08, 0x00, 0x00, 0x00, 0x00, 0x30, 0x08, 0x00, 
  0x00, 0x00, 0x00, 0x30, 0x08, 0x00, 0x00, 0x00, 0x00, 0x60, 0x0c, 0x00, 0x00, 0x00, 0x00, 0xc0, 
  0x0c, 0x00, 0x00, 0x00, 0x01, 0xc0, 0x07, 0x00, 0x00, 0x00, 0x03, 0x80, 0x03, 0xff, 0xff, 0xff, 
  0xfe, 0x00, 0x00, 0xff, 0xff, 0xff, 0xfc, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0x00, 0x00, 0x00, 0x00, 0x01, 0x80, 0x00, 0x00, 
  0x00, 0x60, 0x01, 0x80, 0x03, 0x00, 0x00, 0x60, 0x11, 0x84, 0x03, 0x00, 0x02, 0x64, 0x3d, 0x9c, 
  0x33, 0x60, 0x07, 0xfe, 0x0f, 0xf8, 0x3f, 0xe0, 0x01, 0xf8, 0x07, 0xe0, 0x0f, 0xc0, 0x01, 0xf8, 
  0x03, 0xe0, 0x0f, 0x80, 0x03, 0xfc, 0x0f, 0xf8, 0x3f, 0xe0, 0x03, 0x6e, 0x3d, 0xbc, 0x3b, 0x60, 
  0x00, 0x60, 0x11, 0x8c, 0x03, 0x00, 0x00, 0x60, 0x01, 0x80, 0x03, 0x00, 0x00, 0x00, 0x01, 0x80, 
  0x00, 0x00, 0x00, 0x00, 0x01, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
  };

static const unsigned char PROGMEM moon[] ={
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x03, 0xc0, 0x00, 0x00, 0x00, 0x00, 0x07, 0xc0, 0x00, 0x00, 0x00, 0x00, 0x1f, 
  0xc0, 0x00, 0x00, 0x00, 0x00, 0x3f, 0xc0, 0x00, 0x00, 0x00, 0x00, 0x7f, 0xc0, 0x00, 0x00, 0x00, 
  0x00, 0xff, 0x80, 0x00, 0x00, 0x00, 0x01, 0xff, 0x80, 0x00, 0x00, 0x00, 0x01, 0xff, 0x80, 0x00, 
  0x00, 0x00, 0x03, 0xff, 0x80, 0x00, 0x00, 0x00, 0x07, 0xff, 0x80, 0x00, 0x00, 0x00, 0x07, 0xff, 
  0x80, 0x00, 0x00, 0x00, 0x0f, 0xff, 0x80, 0x00, 0x00, 0x00, 0x0f, 0xff, 0x80, 0x00, 0x00, 0x00, 
  0x0f, 0xff, 0x80, 0x00, 0x00, 0x00, 0x1f, 0xff, 0xc0, 0x00, 0x00, 0x00, 0x1f, 0xff, 0xc0, 0x00, 
  0x00, 0x00, 0x1f, 0xff, 0xc0, 0x00, 0x00, 0x00, 0x1f, 0xff, 0xe0, 0x00, 0x00, 0x00, 0x1f, 0xff, 
  0xe0, 0x00, 0x00, 0x00, 0x1f, 0xff, 0xf0, 0x00, 0x00, 0x00, 0x1f, 0xff, 0xf0, 0x00, 0x00, 0x00, 
  0x1f, 0xff, 0xf8, 0x00, 0x00, 0x00, 0x1f, 0xff, 0xfc, 0x00, 0x00, 0x00, 0x1f, 0xff, 0xfe, 0x00, 
  0x00, 0x00, 0x1f, 0xff, 0xff, 0x00, 0x00, 0x00, 0x1f, 0xff, 0xff, 0xc0, 0x00, 0x00, 0x0f, 0xff, 
  0xff, 0xf0, 0x00, 0x00, 0x0f, 0xff, 0xff, 0xfe, 0x00, 0xf8, 0x0f, 0xff, 0xff, 0xff, 0xff, 0xf8, 
  0x07, 0xff, 0xff, 0xff, 0xff, 0xf8, 0x07, 0xff, 0xff, 0xff, 0xff, 0xf8, 0x03, 0xff, 0xff, 0xff, 
  0xff, 0xf0, 0x03, 0xff, 0xff, 0xff, 0xff, 0xe0, 0x01, 0xff, 0xff, 0xff, 0xff, 0xe0, 0x00, 0xff, 
  0xff, 0xff, 0xff, 0xc0, 0x00, 0x7f, 0xff, 0xff, 0xff, 0x80, 0x00, 0x3f, 0xff, 0xff, 0xff, 0x00, 
  0x00, 0x1f, 0xff, 0xff, 0xfe, 0x00, 0x00, 0x0f, 0xff, 0xff, 0xf8, 0x00, 0x00, 0x03, 0xff, 0xff, 
  0xf0, 0x00, 0x00, 0x00, 0xff, 0xff, 0xc0, 0x00, 0x00, 0x00, 0x1f, 0xfe, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
  };

static const unsigned char PROGMEM cloudy_night[] ={
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0xc0, 0x00, 0x00, 0x00, 0x00, 0x0f, 0xc0, 0x00, 0x00, 0x00, 
  0x00, 0x1f, 0xc0, 0x00, 0x00, 0x00, 0x00, 0x3f, 0x80, 0x00, 0x00, 0x00, 0x00, 0x3f, 0x80, 0x00, 
  0x00, 0x00, 0x00, 0x7f, 0x80, 0x00, 0x00, 0x00, 0x00, 0x7f, 0x80, 0x00, 0x00, 0x00, 0x00, 0xff, 
  0x80, 0x00, 0x00, 0x00, 0x00, 0xff, 0x80, 0x00, 0x00, 0x00, 0x00, 0xff, 0xc0, 0x00, 0x00, 0x00, 
  0x00, 0x1f, 0xc0, 0x00, 0x00, 0x00, 0x00, 0x07, 0xe0, 0x00, 0x00, 0x00, 0x7f, 0xe1, 0xf0, 0x00, 
  0x00, 0x01, 0xff, 0xf8, 0xf8, 0x00, 0x00, 0x07, 0xff, 0xfc, 0x7e, 0x07, 0x00, 0x0f, 0xff, 0xfe, 
  0x3f, 0xff, 0x00, 0x1f, 0xff, 0xff, 0x1f, 0xff, 0x00, 0x1f, 0xff, 0xff, 0x9f, 0xff, 0x00, 0x3f, 
  0xff, 0xff, 0x87, 0xfe, 0x00, 0x7f, 0xff, 0xff, 0xc1, 0xfe, 0x00, 0x7f, 0xff, 0xff, 0xf0, 0x7c, 
  0x00, 0x7f, 0xff, 0xff, 0xfc, 0x30, 0x00, 0xff, 0xff, 0xff, 0xff, 0x00, 0x00, 0xff, 0xff, 0xff, 
  0xff, 0x80, 0x03, 0xff, 0xff, 0xff, 0xff, 0xc0, 0x0f, 0xff, 0xff, 0xff, 0xff, 0xc0, 0x1f, 0xff, 
  0xff, 0xff, 0xff, 0xe0, 0x3f, 0xff, 0xff, 0xff, 0xff, 0xe0, 0x7f, 0xff, 0xff, 0xff, 0xff, 0xf0, 
  0xff, 0xff, 0xff, 0xff, 0xff, 0xf0, 0xff, 0xff, 0xff, 0xff, 0xff, 0xf0, 0xff, 0xff, 0xff, 0xff, 
  0xff, 0xf0, 0xff, 0xff, 0xff, 0xff, 0xff, 0xf0, 0xff, 0xff, 0xff, 0xff, 0xff, 0xf0, 0xff, 0xff, 
  0xff, 0xff, 0xff, 0xe0, 0xff, 0xff, 0xff, 0xff, 0xff, 0xe0, 0xff, 0xff, 0xff, 0xff, 0xff, 0xc0, 
  0x7f, 0xff, 0xff, 0xff, 0xff, 0xc0, 0x7f, 0xff, 0xff, 0xff, 0xff, 0x80, 0x3f, 0xff, 0xff, 0xff, 
  0xff, 0x00, 0x1f, 0xff, 0xff, 0xff, 0xfe, 0x00, 0x07, 0xff, 0xff, 0xff, 0xf8, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
  };

const int LEN = 7;
const unsigned char* bitmap[LEN] = {
  sunny,
  partly_sunny,
  cloudy,
  thunderstorm,
  snow,
  moon,
  cloudy_night,
};
const unsigned char* weather_icon = bitmap[0];

bool down_flag = false;
bool up_flag = false;
bool setting_flag = false;
bool set_flag = false;

bool wipe = false;
bool modee = true;
bool season = true;
bool indoor = true;

bool needPubConfig, needControlGPIO;

unsigned int count = 0;
unsigned int page_number = 1;
unsigned int temp;
unsigned int outdoor_temp = 25;
unsigned int set_temp = 25;

String val;
int last_time = 0;
int newValue;

long lastReconnectAttempt = 0;

String topic = "";
String message = "";

const String stat1 = "{\"status\":\"1\"}";
const String stat0 = "{\"status\":\"0\"}";

const int nWidgets = 4;
String stats       [nWidgets];
String sTopic      [nWidgets];
String color       [nWidgets];
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
  String stats = " {\"status\":\"" + String(s) + "\"}";
  return stats;
}

void initVar() {
  id    [0] = "0";
  page  [0] = "ROOM611";
  pageId[0] = 3;                                           // pageId always start with 1
  descr [0] = "Temperature";
  widget[0] = "anydata";
  defaultVal[0] = 0;
  inverted[0] = false;
  sTopic[0] = prefix + "/" + deviceId + "/temp";
  color [0] = "\"after\":\"°C\"";     
  icon  [0] = "\"icon\":\"thermometer\"";

  id    [1] = "1";
  page  [1] = "ROOM611";
  pageId[1] = 3;                                           // pageId always start with 1
  descr [1] = "Set Temperature";
  widget[1] = "anydata";
  defaultVal[1] = 0;
  inverted[1] = false;
  sTopic[1] = prefix + "/" + deviceId + "/set_temp";
  color [1] = "\"after\":\"°C\"";     
  icon  [1] = "\"icon\":\"thermometer-outline\"";

  id    [2] = "2";
  page  [2] = "ROOM611";
  pageId[2] = 3;                                           // pageId always start with 1
  descr [2] = "set";
  widget[2] = "range";
  defaultVal[2] = 25;
  inverted[2] = false;
  sTopic[2] = prefix + "/" + deviceId + "/range"; 
  color [2] = "\"after\":\"°C\"";  
  icon  [2] = "\"min\":\"10\", \"max\":\"40\", \"debounce\":\"200\"";

  id    [3] = "3";
  page  [3] = "ROOM611";
  pageId[3] = 3;                                           // pageId always start with 1
  descr [3] = "Set Season";
  widget[3] = "select";
  defaultVal[3] = 0;
  inverted[3] = false;
  sTopic[3] = prefix + "/" + deviceId + "/season";
  color [3] = "\"options\":[\"Winter\", \"Summer\"]";     
  icon  [3] = "\"icon\":\"contrast\"";

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

  thing_config[0] = "{\"id\":\"" + id[0] + "\",\"page\":\"" + page[0] + "\",\"pageId\":\"" + pageId[0] + "\",\"descr\":\"" + descr[0] + "\",\"widget\":\"" + widget[0] + "\",\"topic\":\"" + sTopic[0] + "\"," + color[0] + "," + icon[0] + "}";
  thing_config[1] = "{\"id\":\"" + id[1] + "\",\"page\":\"" + page[1] + "\",\"pageId\":\"" + pageId[1] + "\",\"descr\":\"" + descr[1] + "\",\"widget\":\"" + widget[1] + "\",\"topic\":\"" + sTopic[1] + "\"," + color[1] + "," + icon[1] + "}"; 
  thing_config[2] = "{\"id\":\"" + id[2] + "\",\"page\":\"" + page[2] + "\",\"pageId\":\"" + pageId[2] + "\",\"descr\":\"" + descr[2] + "\",\"widget\":\"" + widget[2] + "\",\"topic\":\"" + sTopic[2] + "\"," + color[2] + "," + icon[2] + "}";
  thing_config[3] = "{\"id\":\"" + id[3] + "\",\"page\":\"" + page[3] + "\",\"pageId\":\"" + pageId[3] + "\",\"descr\":\"" + descr[3] + "\",\"widget\":\"" + widget[3] + "\",\"topic\":\"" + sTopic[3] + "\"," + color[3] + "," + icon[3] + "}";

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
//    pubStatus(sTopic[i], stats[i]);
  }
  yield();
}
void getSensorData() {
  
  stats[0] = setStatus(temp);
  stats[1] = setStatus(set_temp);
  stats[2] = setStatus(set_temp);
  stats[3] = setStatus(!season);
  
  
  pubStatus(sTopic[0], stats[0]);
  pubStatus(sTopic[1], stats[1]);
  pubStatus(sTopic[2], stats[2]);
  pubStatus(sTopic[3], stats[3]);

  Serial.println();
  delay(50);

}

void controlGPIO() {

  needControlGPIO = false;
  
  Serial.println("Get data from subscribed topic " + topic + " => " + message);

  if (topic == prefix + "/" + deviceId + "/reset" + "/control"){
    temp = message.substring(0,2).toInt();
    set_temp = message.substring(3,5).toInt();
    season = !message.substring(6,7).toInt();

    stats[0] = setStatus(message.substring(0,2));
    stats[1] = setStatus(message.substring(3,5));
    stats[2] = setStatus(message.substring(3,5));
    stats[3] = setStatus(message.substring(6,7));

    pubStatus(sTopic[0], stats[0]);
    pubStatus(sTopic[1], stats[1]);
    pubStatus(sTopic[2], stats[2]);
    pubStatus(sTopic[3], stats[3]);
   
  }
  else if (topic == sTopic[2] + "/control") {
    set_temp = message.toInt();
    
    stats[1] = setStatus(message);
    stats[2] = setStatus(message);
    
    pubStatus(sTopic[1], stats[1]);
    pubStatus(sTopic[2], stats[2]);
    
    timer = millis();
    set_flag = true;
    
    while (set_flag == true){
      set_page();
    }
  }
  else if (topic == sTopic[3] + "/control"){
    season = !message.toInt();
    
    stats[3] = setStatus(message);
    pubStatus(sTopic[3], stats[3]);

    setting_flag = true ;
    wipe = false;
    timer = millis();
    page_number = 2;
  
    while (setting_flag == true){
      setting_page();
    }
  }
  else if (topic == prefix + "/zirsakht-server/weather/temp") {
    outdoor_temp = message.toInt();
  }
  else if (topic == prefix + "/zirsakht-server/weather/icon") {
    if (message == "sunny"){
      weather_icon = bitmap[0];
    }else if (message == "partly-sunny"){
      weather_icon = bitmap[1];
    }else if (message == "cloudy"){
      weather_icon = bitmap[2];
    }else if (message == "thunderstorm"){
      weather_icon = bitmap[3];
    }else if (message == "snow"){
      weather_icon = bitmap[4];
    }else if (message == "moon"){
      weather_icon = bitmap[5];
    }else if (message == "cloudy-night"){
      weather_icon = bitmap[6];
    }else{
      weather_icon = bitmap[0];
    }
  }
}

void onMessageArrived(char* t, byte* m, unsigned int length) {
 
  topic = String(t);
  message = String((char*)m);
  message = message.substring(0, length);

  if (message == "HELLO") {
    needPubConfig = true;
  } else {
    needControlGPIO = true;
  }

}


Adafruit_NeoPixel strip(LED_COUNT, LED_PIN, NEO_GRB + NEO_KHZ800);

#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Fonts/FreeSerif12pt7b.h>
#include <Fonts/FreeSerif18pt7b.h>
#include <Fonts/FreeSerif24pt7b.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET     -1

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

#define LOGO_HEIGHT   16
#define LOGO_WIDTH    16

bool connect_wifi_flag = false;

const int oneWireBus = 2;
OneWire oneWire(oneWireBus);
DallasTemperature sensors(&oneWire);

void setup() {
  Serial.begin(115200);

  sensors.begin();

  strip.begin();   
  strip.show();
  strip.setBrightness(50);
  
  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) { 
    Serial.println(F("SSD1306 allocation failed"));
    for(;;);
    }
  display.clearDisplay();

  WiFi.begin(ssid, pass);
  
  while (WiFi.status() != WL_CONNECTED){
    if (connect_wifi_flag == false){connect_wifi(); connect_wifi_flag = true;}
    colorWipe(strip.Color(  0,   255, 255), 100);
    colorWipe(strip.Color(  0,   0, 0), 100);

  }
  display.clearDisplay();

  client.setServer(mqttServerName, mqttport);
  client.setCallback(onMessageArrived);

  while (!client.connected()){
    Serial.println("Connecting To Server ...");
    if (client.connect(deviceId, mqttuser, mqttpass))
    {
      Serial.println("Connected To Server");
      client.subscribe(toCharArray(prefix));                                   // for receiving HELLO messages from IoT Manager
      client.subscribe(toCharArray(prefix + "/" + deviceId + "/+/control")); 
      client.subscribe(toCharArray(prefix + "/zirsakht-server/weather/temp")); 
      client.subscribe(toCharArray(prefix + "/zirsakht-server/weather/icon")); 
      client.publish(toCharArray(prefix + "/" + deviceId + "/reset"), toCharArray("reset")); 
    } 
    else 
    {
      Serial.print("failed with state ");
      Serial.print(client.state());
      delay(2000);
    }
  }
  
  initVar();
  
  pinMode(DOWN, INPUT);
  pinMode(UP, INPUT);
  pinMode(relay, OUTPUT);

}

void loop() {

  static unsigned long indoor_timer = millis();

  static unsigned long timer1 = millis();
  static unsigned long timer2 = millis();

  if (!client.connected()) {
    Serial.println("Disconnected");
    if (millis() - timer1 > 2000) {
      if (reconnect()) {
        pubConfig();
        timer1 = millis();
      }
      timer1 = millis();
      delay(50);
    }
    delay(50);
  } 
  else {
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
    
  sensors.requestTemperatures(); 
  temp = sensors.getTempCByIndex(0);
  delay(10);
  
  count = 0;
  
  while (digitalRead(UP) == 1 || digitalRead(DOWN) == 1){
    count += 1;
    delay(1);
  }

  if (count > 2000){
    setting_flag = true ;
    timer = millis();
    page_number = 1;
  }
  else if (count >= 1 && count <=200){
    set_flag = true;
    timer = millis();
  }
  
  while (setting_flag == true){
    setting_page();
  }
  
  while (set_flag == true){
    set_page();
  }



  if (millis() - indoor_timer > 5000){
    indoor = !indoor;
    indoor_timer = millis();
  }

  display.clearDisplay();
  colorWipe(strip.Color(  0,   0, 0), 5);
  home_page(temp);
  delay(10);
  
  display.display();

  control_fancoil(season, temp, set_temp);
  delay(10);

}


void home_page(int temp) {
  if (indoor == true){
    display.stopscroll();
    display.setFont();
    display.drawBitmap(0, 0, wifi_bmp, LOGO_WIDTH, LOGO_HEIGHT, 1);
    display.setTextSize(2); 
    display.setCursor(32,0);
    display.print("INDOOR");
    display.setTextSize(1); 
    display.setCursor(24,50);
    display.setFont(&FreeSerif24pt7b);
    display.print(temp);
    display.setCursor(74,55);
    display.print("C");
  }
  else{
    display.stopscroll();
    display.setFont();
    display.drawBitmap(0, 0, wifi_bmp, LOGO_WIDTH, LOGO_HEIGHT, 1);
    display.setTextSize(2); 
    display.setCursor(26,0);
    display.print("OUTDOOR");
    display.setTextSize(1); 
    display.setCursor(62,50);
    display.setFont(&FreeSerif18pt7b);
    display.print(outdoor_temp);
    display.setCursor(98,55);
    display.print("C");
    display.drawBitmap(8, 16, weather_icon, 48, 48, 1);
  }
}


void setting_page(void){
  
  if (millis() - timer > 5000) {
    setting_flag = false;
  }

  if (digitalRead(UP) == 1){
    if (up_flag == false){
        timer = millis();
        if (page_number == 1){
          page_number = 2;
        }
        else { 
          page_number = 1;
          }
        up_flag = true;
    }
  }
  else{
    if (up_flag == true){
        up_flag = false;
    }
  }

  if (page_number == 1){
    
    if (digitalRead(DOWN) == 1){
      if (down_flag == false){
          timer = millis();
          modee = !modee;
          down_flag = true;
      }
    }
    else{
      if (down_flag == true){
          down_flag = false;
      }
    }
    
    if (modee == true){
      display.clearDisplay();
      display.setFont();
      display.drawBitmap(0, 0, wifi_bmp, LOGO_WIDTH, LOGO_HEIGHT, 1);
      display.setTextSize(2);
      display.setTextColor(SSD1306_WHITE); 
      display.setCursor(42,0);
      display.print("MODE");
      display.setTextSize(1);
      display.setFont(&FreeSerif24pt7b);
      display.setCursor(0,60);
      display.print("AUTO");
      display.display();
      delay(100);
    }
    else{
      display.clearDisplay();
      display.setFont();
      display.drawBitmap(0, 0, wifi_bmp, LOGO_WIDTH, LOGO_HEIGHT, 1);
      display.setTextSize(2);
      display.setTextColor(SSD1306_WHITE); 
      display.setCursor(42,0);
      display.print("MODE");
      display.setTextSize(1);
      display.setFont(&FreeSerif24pt7b);
      display.setCursor(32,60);
      display.print("IOT");
      display.display();
      delay(100);
    }
  } 
  
  else {
   
    if (digitalRead(DOWN) == 1){
      if (down_flag == false){
          timer = millis();
          season = !season;
          down_flag = true;
          wipe = false;
      }
    }
    else{
      if (down_flag == true){
          down_flag = false;
      }
    }
    
    if (season == true){
      display.clearDisplay();
      display.setFont();
      display.drawBitmap(0, 0, wifi_bmp, LOGO_WIDTH, LOGO_HEIGHT, 1);
      display.setTextSize(2);
      display.setTextColor(SSD1306_WHITE); 
      display.setCursor(36,0);
      display.print("SEASON");
      display.setTextSize(1);
      display.setFont(&FreeSerif18pt7b);
      display.setCursor(12,60);
      display.print("Winter");
      display.display();
      delay(100);
      if (wipe == false){
        colorWipe(strip.Color(  255,   0, 0), 25);
        colorWipe(strip.Color(  0,   0, 0), 25);
        wipe = true;
      }
    }
    else{
      display.clearDisplay();
      display.setFont();
      display.drawBitmap(0, 0, wifi_bmp, LOGO_WIDTH, LOGO_HEIGHT, 1);
      display.setTextSize(2);
      display.setTextColor(SSD1306_WHITE); 
      display.setCursor(36,0);
      display.print("SEASON");
      display.setTextSize(1);
      display.setFont(&FreeSerif18pt7b);
      display.setCursor(8,60);
      display.print("Summer");
      display.display();
      delay(100);
      if (wipe == false){
        colorWipe(strip.Color(  0,   0, 255), 25);
        colorWipe(strip.Color(  0,   0, 0), 25);
        wipe = true;
      }
    }
  }

}

void set_page(void){
  
  if (millis() - timer > 2000) {
    set_flag = false;
  }

  setColor(set_temp);
  display.clearDisplay();
  display.setFont();
  display.drawBitmap(0, 0, wifi_bmp, LOGO_WIDTH, LOGO_HEIGHT, 1);
  display.setTextSize(2);
  display.setTextColor(SSD1306_WHITE); 
  display.setCursor(22,0);
  display.print("SET TEMP");
  display.setTextSize(1);
  display.setCursor(24,50);
  display.setFont(&FreeSerif24pt7b);
  display.print(set_temp);
  display.setCursor(74,55);
  display.print("C");
  display.display();
  delay(100);

  if (digitalRead(UP) == 1){
    if (up_flag == false){
      timer = millis();
      set_temp += 1;
      setColor(set_temp);
      up_flag = true;
    }
  }
  else{
    if (up_flag == true){
        up_flag = false;
    }
  }
    
  if (digitalRead(DOWN) == 1){
    if (down_flag == false){
      timer = millis();
      set_temp -= 1;
      setColor(set_temp);
      down_flag = true;
    }
  }
  else{
    if (down_flag == true){
      down_flag = false;
    }
  }
}


void connect_wifi(void){
  display.setFont(&FreeSerif12pt7b);
  display.setTextColor(SSD1306_WHITE); 
  display.setCursor(0,32);
  display.println(F(" Connecting "));
  display.display();
  display.startscrollright(0x00, 0x0F);
};


void colorWipe(uint32_t color, int wait) {
  for(int i=0; i<strip.numPixels(); i++) { // For each pixel in strip...
    strip.setPixelColor(i, color);         //  Set pixel's color (in RAM)
    strip.show();                          //  Update strip to match
    delay(wait);                           //  Pause for a moment
  }
}

void setColor(int set_temp){
  
  if (set_temp > 25){
    for (int i=25; i <= set_temp; i++){
      strip.setPixelColor(i - 25, strip.Color(  255,   0, 0));  
    }
    for (int j=set_temp-25; j < strip.numPixels(); j++){
      strip.setPixelColor(j, strip.Color(  0,   0, 0));
    }
  }
  
  else if (set_temp < 25){
    for (int i=25; i >= set_temp; i--){
      strip.setPixelColor(strip.numPixels() - (25 - i), strip.Color(  0,   0, 255));  
    }
    
    if ((25 - set_temp) == 1){ strip.setPixelColor(0, strip.Color(  0,   0, 255)); }
    
    for (int j=25-set_temp; j < strip.numPixels(); j++){
      strip.setPixelColor(strip.numPixels() - j, strip.Color(  0,   0, 0));
    }
  }
  
  else {
    strip.setPixelColor(0, strip.Color(  0,   255, 0));}
  strip.show(); 
  
}

void control_fancoil(bool season, int temp, int set_temp){
  if (season == true){
    if (temp > set_temp){
      digitalWrite(relay, 0);
    }
    if (temp < set_temp){
      digitalWrite(relay, 1);
    }
  }
  else{
    if (temp < set_temp){
      digitalWrite(relay, 0);
    }
    if (temp > set_temp){
      digitalWrite(relay, 1);
    }
  }
}

boolean reconnect() {
  if (client.connect(deviceId, mqttuser, mqttpass)) {
    Serial.println("reconnecting");
    client.setCallback(onMessageArrived);
    client.subscribe(toCharArray(prefix));
    client.subscribe(toCharArray(prefix + "/" + deviceId + "/+/control"));
    client.subscribe(toCharArray(prefix + "/zirsakht-server/weather/temp"));
    client.subscribe(toCharArray(prefix + "/zirsakht-server/weather/icon"));
  }
  return client.connected();
}
