#include <Wire.h>
#include <WiFi.h>
#include <PubSubClient.h>
#include "MPU9250.h"
#include <KalmanFilter.h>

const char* ssid = "IOT";
const char* pass = "Thereisnoway97";
const char* server = "5.182.44.124";
const int port = 1883;
const char* username = "leo";
const char* password = "Goldenhand76";

char Pitch[10];
char Roll[10];

WiFiClient espClient;
PubSubClient client(espClient);

MPU9250 IMU(i2c0, 0x68);
int status;

KalmanFilter kalmanX(0.001,0.003,0.03);
KalmanFilter kalmanY(0.001,0.003,0.03);

float accPitch = 0 ; 
float accRoll = 0 ;

float kalPitch = 0 ;
float kalRoll = 0 ; 

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

  while (!client.connected()){
    Serial.println("Connecting To Server ...");
    if (client.connect("ESP32Client", username, password)){
      Serial.println("Connected To Server");
    } else {
      Serial.print("failed with state ");
      Serial.print(client.state());
      delay(2000);
    }
  }

  client.subscribe("test0");
  client.subscribe("test1");
  client.subscribe("test2");
  
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

  accPitch = -atan2(IMU.getAccelX_mss(),sqrt(IMU.getAccelY_mss()*IMU.getAccelY_mss() + IMU.getAccelZ_mss()*IMU.getAccelZ_mss()))*RAD_TO_DEG;
  accRoll = atan2(IMU.getAccelY_mss(), IMU.getAccelZ_mss())*RAD_TO_DEG;

  kalPitch = kalmanY.update(accPitch,IMU.getGyroY_rads());
  kalRoll = kalmanX.update(accRoll,IMU.getGyroX_rads());

  Serial.print("Roll = ");
  Serial.print(accRoll);

  Serial.print("   kalRoll = ");
  Serial.print(kalRoll);

  Serial.print("   Pitch = ");
  Serial.print(accPitch);

  Serial.print("   kalPitch = ");
  Serial.println(kalPitch);

  String pitch_str = String(accPitch); //converting ftemp (the float variable above) to a string
  pitch_str.toCharArray(Pitch, pitch_str.length() + 1); //packaging up the data to publish to mqtt whoa...

  String roll_str = String(accRoll); //converting Humidity (the float variable above) to a string
  roll_str.toCharArray(Roll, roll_str.length() + 1); //packaging up the data to publish to mqtt whoa...
    
  client.publish("Pitch", Pitch);
  client.publish("Roll", Roll);
  delay(20);
}

void callback(char* topic, byte* message, unsigned int length) {
  Serial.print("Message arrived on topic: ");
  Serial.print(topic);
  Serial.print(". Message: ");
  String messageTemp;
  
  for (int i = 0; i < length; i++) {
    Serial.print((char)message[i]);
    messageTemp += (char)message[i];
  }
  Serial.println();
}
