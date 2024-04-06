#include <Wire.h>
#include "MPU9250.h"
#include <KalmanFilter.h>

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
  // serial to display data
  Serial.begin(9600);
  while (!Serial)
  {
  }
  // start communication with IMU
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
  // read the sensor
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

  
//  printf("\r\n ACC(%.6f, %.6f, %.6f) Gyro(%.6f, %.6f, %.6f) MAG(%.6f, %.6f, %.6f)", IMU.getAccelX_mss(), IMU.getAccelY_mss(), IMU.getAccelZ_mss(), IMU.getGyroX_rads(), IMU.getGyroY_rads(), IMU.getGyroZ_rads(), 
//  IMU.getMagX_uT(), IMU.getMagY_uT(), IMU.getMagZ_uT());
  
//   printf("\r\n Mag(%.6f, %.6f, %.6f) Yaw %.6f ", 
//     IMU.getAccelX_mss(), IMU.getAccelY_mss(), IMU.getAccelZ_mss(), IMU.getGyroX_rads(), IMU.getGyroY_rads(), IMU.getGyroZ_rads(), 
//     IMU.getMagX_uT(), IMU.getMagY_uT(), IMU.getMagZ_uT(), float(atan2(IMU.getMagY_uT(), IMU.getMagX_uT())) * RAD_TO_DEG
//   );

//  Serial.printf("\r\n Yaw %.6f ", 
//    float(atan2(IMU.getMagY_uT(), IMU.getMagX_uT())) * RAD_TO_DEG
//  );
  delay(20);
}
