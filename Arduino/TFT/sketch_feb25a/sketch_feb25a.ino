#include <ESP8266HTTPClient.h>
#include <Arduino_JSON.h>
#include <ESP8266WiFi.h>
#include <WiFiClient.h>

const char* ssid = "IOT";
const char* password = "Thereisnoway97";

String serverName = "http://iotcom.ir/monitor/motor";
String sensorReadings;

/**********************************************************************
 
 Interfacing ESP8266 NodeMCU with ILI9341 TFT display (240x320 pixel).
 This is a free software with NO WARRANTY.
 https://simple-circuit.com/
 
/**********************************************************************
/**********************************************************************
  This is our GFX example for the Adafruit ILI9341 Breakout and Shield
  ----> http://www.adafruit.com/products/1651
 
  Check out the links above for our tutorials and wiring diagrams
  These displays use SPI to communicate, 4 or 5 pins are required to
  interface (RST is optional)
  Adafruit invests time and resources providing this open source code,
  please support Adafruit and open-source hardware by purchasing
  products from Adafruit!
 
  Written by Limor Fried/Ladyada for Adafruit Industries.
  MIT license, all text above must be included in any redistribution
 **********************************************************************/
 
 
#include <Adafruit_GFX.h>       // include Adafruit graphics library
#include <Adafruit_ILI9341.h>   // include Adafruit ILI9341 TFT library
#include <SPI.h>
#include <SD.h>
#define TFT_CS    4     // TFT CS  pin is connected to NodeMCU pin D2
#define TFT_RST   0     // TFT RST pin is connected to NodeMCU pin D3
#define TFT_DC    2     // TFT DC  pin is connected to NodeMCU pin D4
// initialize ILI9341 TFT library with hardware SPI module
// SCK (CLK) ---> NodeMCU pin D5 (GPIO14)
// MOSI(DIN) ---> NodeMCU pin D7 (GPIO13)
Adafruit_ILI9341 tft = Adafruit_ILI9341(TFT_CS, TFT_DC, TFT_RST);

void setup() {
  Serial.begin(9600);
  
  tft.begin();
  // read diagnostics (optional but can help debug problems)
  uint8_t x = tft.readcommand8(ILI9341_RDMODE);
  Serial.print("Display Power Mode: 0x"); Serial.println(x, HEX);
  x = tft.readcommand8(ILI9341_RDMADCTL);
  Serial.print("MADCTL Mode: 0x"); Serial.println(x, HEX);
  x = tft.readcommand8(ILI9341_RDPIXFMT);
  Serial.print("Pixel Format: 0x"); Serial.println(x, HEX);
  x = tft.readcommand8(ILI9341_RDIMGFMT);
  Serial.print("Image Format: 0x"); Serial.println(x, HEX);
  x = tft.readcommand8(ILI9341_RDSELFDIAG);
  Serial.print("Self Diagnostic: 0x"); Serial.println(x, HEX); 
  
  WiFi.begin(ssid, password);
  Serial.println("Connecting");
  tft.setRotation(3);
  tft.fillScreen(ILI9341_BLACK);
  tft.setCursor(0, 100);
  tft.setTextColor(ILI9341_YELLOW); tft.setTextSize(4);
  tft.println("CONNECTING...");
  delay(1000);
  while(WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  tft.fillScreen(ILI9341_BLACK);
  tft.setCursor(0, 100);
  tft.setTextColor(ILI9341_GREEN); tft.setTextSize(4);
  tft.println("CONNECTED");
  delay(1000);
  Serial.println("");
  Serial.print("Connected to WiFi network with IP Address: ");
  Serial.println(WiFi.localIP());
     
//  Serial.print(F("Screen fill              "));
//  Serial.println(testFillScreen());
//  delay(500);
// 
//  Serial.print(F("Text                     "));
//  Serial.println(testText());
//  delay(3000);
// 
//  Serial.print(F("Lines                    "));
//  Serial.println(testLines(ILI9341_CYAN));
//  delay(500);
// 
//  Serial.print(F("Horiz/Vert Lines         "));
//  Serial.println(testFastLines(ILI9341_RED, ILI9341_BLUE));
//  delay(500);
// 
//  Serial.print(F("Rectangles (outline)     "));
//  Serial.println(testRects(ILI9341_GREEN));
//  delay(500);
// 
//  Serial.print(F("Rectangles (filled)      "));
//  Serial.println(testFilledRects(ILI9341_YELLOW, ILI9341_MAGENTA));
//  delay(500);
// 
//  Serial.print(F("Circles (filled)         "));
//  Serial.println(testFilledCircles(10, ILI9341_MAGENTA));
// 
//  Serial.print(F("Circles (outline)        "));
//  Serial.println(testCircles(10, ILI9341_WHITE));
//  delay(500);
// 
//  Serial.print(F("Triangles (outline)      "));
//  Serial.println(testTriangles());
//  delay(500);
// 
//  Serial.print(F("Triangles (filled)       "));
//  Serial.println(testFilledTriangles());
//  delay(500);
// 
//  Serial.print(F("Rounded rects (outline)  "));
//  Serial.println(testRoundRects());
//  delay(500);
// 
//  Serial.print(F("Rounded rects (filled)   "));
//  Serial.println(testFilledRoundRects());
//  delay(500);
// 
//  Serial.println(F("Done!"));
 
}
 
 
void loop(void) {
//  testText();
  sensorReadings = httpGETRequest(serverName);
  Serial.print(sensorReadings);
  if (sensorReadings != "{}"){
    JSONVar myObject = JSON.parse(sensorReadings);
  
    if (JSON.typeof(myObject) == "undefined") {
      Serial.println("Parsing input failed!");
      return;
    }
   
    JSONVar keys = myObject.keys();
    
    tft.setRotation(3);
    tft.fillScreen(ILI9341_BLACK);
    tft.setCursor(0, 70);
    
    for (int i = 1; i < keys.length(); i++) {
      JSONVar value = myObject[keys[i]];
      const char* string = keys[i];
      String strkey(string);
      Serial.print(strkey);
      Serial.print(" = ");
      Serial.println(value);
      tft.setTextColor(ILI9341_WHITE);  tft.setTextSize(1);
      tft.print(strkey);
      tft.setTextColor(ILI9341_YELLOW); tft.setTextSize(1);
      tft.print(" = ");
      tft.println(value);
      tft.println();
    }
    delay(2500);
  }
  else {
    tft.fillScreen(ILI9341_BLACK);
    tft.setCursor(0, 70);
    tft.setTextColor(ILI9341_RED); tft.setTextSize(5);
    tft.println("DISCONNECTED");
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

unsigned long testFillScreen() {
  unsigned long start = micros();
  tft.fillScreen(ILI9341_BLACK);
  tft.fillScreen(ILI9341_RED);
  tft.fillScreen(ILI9341_GREEN);
  tft.fillScreen(ILI9341_BLUE);
  tft.fillScreen(ILI9341_BLACK);
  return micros() - start;
}
 
unsigned long testText() {
  tft.fillScreen(ILI9341_BLACK);
  unsigned long start = micros();
  tft.setCursor(0, 0);
  tft.setTextColor(ILI9341_WHITE);  tft.setTextSize(1);
  tft.println("Hello World!");
  tft.setTextColor(ILI9341_YELLOW); tft.setTextSize(2);
  tft.println(1234.56);
  tft.setTextColor(ILI9341_RED);    tft.setTextSize(3);
  tft.println(0xDEADBEEF, HEX);
  tft.println();
  tft.setTextColor(ILI9341_GREEN);
  tft.setTextSize(5);
  tft.println("Groop");
  tft.setTextSize(2);
  tft.println("I implore thee,");
  tft.setTextSize(1);
  tft.println("my foonting turlingdromes.");
  tft.println("And hooptiously drangle me");
  tft.println("with crinkly bindlewurdles,");
  tft.println("Or I will rend thee");
  tft.println("in the gobberwarts");
  tft.println("with my blurglecruncheon,");
  tft.println("see if I don't!");
  return micros() - start;
}
 
unsigned long testLines(uint16_t color) {
  unsigned long start, t;
  int           x1, y1, x2, y2,
                w = tft.width(),
                h = tft.height();
 
  tft.fillScreen(ILI9341_BLACK);
 
  x1 = y1 = 0;
  y2    = h - 1;
  start = micros();
  for(x2=0; x2<w; x2+=6) tft.drawLine(x1, y1, x2, y2, color);
  x2    = w - 1;
  for(y2=0; y2<h; y2+=6) tft.drawLine(x1, y1, x2, y2, color);
  t     = micros() - start; // fillScreen doesn't count against timing
 
  tft.fillScreen(ILI9341_BLACK);
 
  x1    = w - 1;
  y1    = 0;
  y2    = h - 1;
  start = micros();
  for(x2=0; x2<w; x2+=6) tft.drawLine(x1, y1, x2, y2, color);
  x2    = 0;
  for(y2=0; y2<h; y2+=6) tft.drawLine(x1, y1, x2, y2, color);
  t    += micros() - start;
 
  tft.fillScreen(ILI9341_BLACK);
 
  x1    = 0;
  y1    = h - 1;
  y2    = 0;
  start = micros();
  for(x2=0; x2<w; x2+=6) tft.drawLine(x1, y1, x2, y2, color);
  x2    = w - 1;
  for(y2=0; y2<h; y2+=6) tft.drawLine(x1, y1, x2, y2, color);
  t    += micros() - start;
 
  tft.fillScreen(ILI9341_BLACK);
 
  x1    = w - 1;
  y1    = h - 1;
  y2    = 0;
  start = micros();
  for(x2=0; x2<w; x2+=6) tft.drawLine(x1, y1, x2, y2, color);
  x2    = 0;
  for(y2=0; y2<h; y2+=6) tft.drawLine(x1, y1, x2, y2, color);
 
  return micros() - start;
}
 
unsigned long testFastLines(uint16_t color1, uint16_t color2) {
  unsigned long start;
  int           x, y, w = tft.width(), h = tft.height();
 
  tft.fillScreen(ILI9341_BLACK);
  start = micros();
  for(y=0; y<h; y+=5) tft.drawFastHLine(0, y, w, color1);
  for(x=0; x<w; x+=5) tft.drawFastVLine(x, 0, h, color2);
 
  return micros() - start;
}
 
unsigned long testRects(uint16_t color) {
  unsigned long start;
  int           n, i, i2,
                cx = tft.width()  / 2,
                cy = tft.height() / 2;
 
  tft.fillScreen(ILI9341_BLACK);
  n     = min(tft.width(), tft.height());
  start = micros();
  for(i=2; i<n; i+=6) {
    i2 = i / 2;
    tft.drawRect(cx-i2, cy-i2, i, i, color);
  }
 
  return micros() - start;
}
 
unsigned long testFilledRects(uint16_t color1, uint16_t color2) {
  unsigned long start, t = 0;
  int           n, i, i2,
                cx = tft.width()  / 2 - 1,
                cy = tft.height() / 2 - 1;
 
  tft.fillScreen(ILI9341_BLACK);
  n = min(tft.width(), tft.height());
  for(i=n; i>0; i-=6) {
    i2    = i / 2;
    start = micros();
    tft.fillRect(cx-i2, cy-i2, i, i, color1);
    t    += micros() - start;
    // Outlines are not included in timing results
    tft.drawRect(cx-i2, cy-i2, i, i, color2);
  }
 
  return t;
}
 
unsigned long testFilledCircles(uint8_t radius, uint16_t color) {
  unsigned long start;
  int x, y, w = tft.width(), h = tft.height(), r2 = radius * 2;
 
  tft.fillScreen(ILI9341_BLACK);
  start = micros();
  for(x=radius; x<w; x+=r2) {
    for(y=radius; y<h; y+=r2) {
      tft.fillCircle(x, y, radius, color);
    }
  }
 
  return micros() - start;
}
 
unsigned long testCircles(uint8_t radius, uint16_t color) {
  unsigned long start;
  int           x, y, r2 = radius * 2,
                w = tft.width()  + radius,
                h = tft.height() + radius;
 
  // Screen is not cleared for this one -- this is
  // intentional and does not affect the reported time.
  start = micros();
  for(x=0; x<w; x+=r2) {
    for(y=0; y<h; y+=r2) {
      tft.drawCircle(x, y, radius, color);
    }
  }
 
  return micros() - start;
}
 
unsigned long testTriangles() {
  unsigned long start;
  int           n, i, cx = tft.width()  / 2 - 1,
                      cy = tft.height() / 2 - 1;
 
  tft.fillScreen(ILI9341_BLACK);
  n     = min(cx, cy);
  start = micros();
  for(i=0; i<n; i+=5) {
    tft.drawTriangle(
      cx    , cy - i, // peak
      cx - i, cy + i, // bottom left
      cx + i, cy + i, // bottom right
      tft.color565(0, 0, i));
  }
 
  return micros() - start;
}
 
unsigned long testFilledTriangles() {
  unsigned long start, t = 0;
  int           i, cx = tft.width()  / 2 - 1,
                   cy = tft.height() / 2 - 1;
 
  tft.fillScreen(ILI9341_BLACK);
  start = micros();
  for(i=min(cx,cy); i>10; i-=5) {
    start = micros();
    tft.fillTriangle(cx, cy - i, cx - i, cy + i, cx + i, cy + i,
      tft.color565(0, i, i));
    t += micros() - start;
    tft.drawTriangle(cx, cy - i, cx - i, cy + i, cx + i, cy + i,
      tft.color565(i, i, 0));
  }
 
  return t;
}
 
unsigned long testRoundRects() {
  unsigned long start;
  int           w, i, i2,
                cx = tft.width()  / 2 - 1,
                cy = tft.height() / 2 - 1;
 
  tft.fillScreen(ILI9341_BLACK);
  w     = min(tft.width(), tft.height());
  start = micros();
  for(i=0; i<w; i+=6) {
    i2 = i / 2;
    tft.drawRoundRect(cx-i2, cy-i2, i, i, i/8, tft.color565(i, 0, 0));
  }
 
  return micros() - start;
}
 
unsigned long testFilledRoundRects() {
  unsigned long start;
  int           i, i2,
                cx = tft.width()  / 2 - 1,
                cy = tft.height() / 2 - 1;
 
  tft.fillScreen(ILI9341_BLACK);
  start = micros();
  for(i=min(tft.width(), tft.height()); i>20; i-=6) {
    i2 = i / 2;
    tft.fillRoundRect(cx-i2, cy-i2, i, i, i/8, tft.color565(0, i, 0));
  }
 
  return micros() - start;
}
