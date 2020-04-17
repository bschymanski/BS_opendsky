#ifndef MPUFUNCTIONS_H
#define MPUFUNCTIONS_H

extern short Gyro;
extern short Accel;

extern void mpu_setup();
extern void actionReadIMU(int imumode);
extern void readIMU(int imumode);
#endif