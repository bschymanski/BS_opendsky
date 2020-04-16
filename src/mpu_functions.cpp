#include <Arduino.h>
#include<Wire.h>
#include <mpu_functions.h>
#include <neopixel.h>
#include <SevenSeg.h>
#include <globals.h>
#include <main.h>
#include <functions.h>

const int MPU_addr=0x69;  // I2C address of the MPU-6050

// IMU https://github.com/griegerc/arduino-gy521/blob/master/gy521-read-angle/gy521-read-angle.ino
const int ACCEL_OFFSET   = 200;
const int GYRO_OFFSET    = 151;  // 151
const int GYRO_SENSITITY = 131;  // 131 is sensivity of gyro from data sheet
const float GYRO_SCALE   = 0.2; //  0.02 by default - tweak as required
const float GYRO_TEMP_DRIFT   = 0.02; //  0.02 by default - tweak as required
const int GYRO_GRANGE = 2; // Gforce Range
const int ACCEL_SCALE = 16384; // Scalefactor of Accelerometer
const float LOOP_TIME    = 0.15; // 0.1 = 100ms
const int GYRO_OFFSET_X = 2; // change this to your system until gyroCorrX displays 0 if the DSKY sits still
const int GYRO_OFFSET_Y = 0; // change this to your system until gyroCorrY displays 0 if the DSKY sits still
const int GYRO_OFFSET_Z = 0; // change this to your system until gyroCorrZ displays 0 if the DSKY sits still
const int ACC_OFFSET_X = 2; // change this to your system until accAngleX displays 0 if the DSKY sits still
const int ACC_OFFSET_Y = 3; // change this to your system until accAngleY displays 0 if the DSKY sits still
const int ACC_OFFSET_Z = 0;  // change this to your system until accAngleZ displays 0 if the DSKY sits still

short Gyro = 1;
short Accel = 0;

short plus = 1;
short minus = 0;


void mpu_setup()
{
    Wire.begin();
    Wire.beginTransmission(MPU_addr);
    Wire.write(0x6B);  // PWR_MGMT_1 register
    Wire.write(0);     // set to zero (wakes up the MPU-6050)
    Wire.endTransmission(true);
}

void readIMU(int imumode)
{  // reads the IMU Values mode Gyro or Accel
    /* 
    https://elektro.turanis.de/html/prj075/index.html
    https://github.com/griegerc/arduino-gy521/blob/master/gy521-read-angle/gy521-read-angle.ino
    const int ACCEL_OFFSET   = 200;
    const int GYRO_OFFSET    = 151;  // 151
    const int GYRO_SENSITITY = 131;  // 131 is sensivity of gyro from data sheet
    const float GYRO_SCALE   = 2; //  0.02 by default - tweak as required
    const float LOOP_TIME    = 0.15; // 0.1 = 100ms
    */
    Wire.beginTransmission(MPU_addr);
    Wire.write(0x3B);  // starting with register 0x3B (ACCEL_XOUT_H)
    Wire.endTransmission(false);
    Wire.requestFrom(MPU_addr,14,true);  // request a total of 14 registers
    int accValueX = 0;
    int accValueY = 0;
    int accValueZ = 0;
    int accCorrX = 0;
    int accCorrY = 0;
    int accCorrZ = 0;
    float accAngleX = 0.0;
    float accAngleY = 0.0;
    float accAngleZ = 0.0;
    int temp = 0;
    int gyroValueX = 0;
    int gyroValueY = 0;
    int gyroValueZ = 0;
    float gyroAngleX = 0.0;
    float gyroAngleY = 0.0;
    float gyroAngleZ = 0.0; 
    float gyroCorrX = 0.0;
    float gyroCorrY = 0.0;
    float gyroCorrZ = 0.0;
  
    accValueX = (Wire.read() << 8) | Wire.read();  // 0x3B (ACCEL_XOUT_H) & 0x3C (ACCEL_XOUT_L)
    accValueY = (Wire.read() << 8) | Wire.read();  // 0x3D (ACCEL_YOUT_H) & 0x3E (ACCEL_YOUT_L)
    accValueZ = (Wire.read() << 8) | Wire.read();  // 0x3F (ACCEL_ZOUT_H) & 0x40 (ACCEL_ZOUT_L)
    temp = (Wire.read() << 8) | Wire.read();  // 0x41 (TEMP_OUT_H) & 0x42 (TEMP_OUT_L)
    gyroValueX = (Wire.read() << 8) | Wire.read();  // 0x43 (GYRO_XOUT_H) & 0x44 (GYRO_XOUT_L)
    gyroValueY = (Wire.read() << 8) | Wire.read();  // 0x45 (GYRO_YOUT_H) & 0x46 (GYRO_YOUT_L)
    gyroValueZ = (Wire.read() << 8) | Wire.read();  // 0x47 (GYRO_ZOUT_H) & 0x48 (GYRO_ZOUT_L)
        
    temp = (temp / 340.00 + 36.53); //equation for temperature in degrees C from datasheet
        
        
    accCorrX = accValueX - ACCEL_OFFSET;
    accCorrX = map(accCorrX, -ACCEL_SCALE, ACCEL_SCALE, -90, 90);
    accAngleX = constrain(accCorrX, -90, 90);
    // our IMU sits upside down in the DSKY, so we have to flip the angle
    accAngleX = -accAngleX;
    accAngleX = accAngleX + ACC_OFFSET_X;

    accCorrY = accValueY - ACCEL_OFFSET;
    accCorrY = map(accCorrY, -ACCEL_SCALE, ACCEL_SCALE, -90, 90);

    accAngleY = constrain(accCorrY, -90, 90);
    accAngleY = accAngleY + ACC_OFFSET_Y;

    accCorrZ = accValueZ - ACCEL_OFFSET;
    accCorrZ = map(accCorrZ, -ACCEL_SCALE, ACCEL_SCALE, -90, 90);
    accAngleZ = constrain(accCorrZ, -90, 90);
        // our IMU sits upside down in the DSKY, so we have to flip the angle
    accAngleZ = -accAngleZ;
    accAngleZ = accAngleZ + ACC_OFFSET_Z;

    gyroCorrX = (float)((gyroValueX/GYRO_SENSITITY)+GYRO_OFFSET_X);
    gyroAngleX = (gyroCorrX * GYRO_GRANGE) * -LOOP_TIME;
    gyroCorrY = (float)((gyroValueY/GYRO_SENSITITY)+GYRO_OFFSET_Y);
    gyroAngleY = (gyroCorrY * GYRO_GRANGE) * -LOOP_TIME;
    gyroCorrZ = (float)((gyroValueZ/GYRO_SENSITITY)+GYRO_OFFSET_Z);
    gyroAngleZ = (gyroCorrZ * GYRO_GRANGE) * -LOOP_TIME;
    setLamp(off, lampNoAtt);
    if (imumode == Gyro)
        {
            printRegister(1,int(gyroAngleX*100));
            printRegister(2,int(gyroAngleY*100));
            printRegister(3,int(gyroAngleZ*100));
        }
    else if (imumode == Accel)
        {
            printRegister(1,int(accAngleX*100));
            printRegister(2,int(accAngleY*100));
            printRegister(3,int(accAngleZ*100));
    }
    
}

void actionReadIMU(int imumode)
{
    if ((toggle500 == true) && (imutoggle == true))
    {   // only every 600ms an imuupdate to avoid flickering
        imutoggle = false;
        //flashUplinkAndComputerActivityRandomly();
        readIMU(imumode);
        
        
    }
    else if ((toggle500 == false) && (imutoggle == false))
    {
        //flashUplinkAndComputerActivityRandomly();
        imutoggle = true;
        readIMU(imumode);
    }
}