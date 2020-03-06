/*
 * OutputDevice.h
 *
 *  Created on: 2020年2月22日
 *      Author: Hcc
 */

#ifndef HARDWARE_OUTPUTDEVICE_H_
#define HARDWARE_OUTPUTDEVICE_H_

#include "sys.h"
#include "gpiolib.h"


#define MV1 0
#define MV2 1
#define MV3 2
#define MV4 3
#define MV5 4  //锁舌电机气阀
#define MV6 5  //拨轮电机气阀
#define MV7 6  //叉舌气阀
#define MV8 7 
#define MV9 8
#define MV10 9
#define MV11 10
#define MV12 11
#define MV13 12
#define MV14 13
#define MV15 14
#define MV16 15
#define MV17 16
#define MV18 17
#define MV19 18
#define MV20 19
#define MV21 20
#define LDS1_P 21
#define LDS2_P 22
#define LDS3_P 23
#define LDS4_P 24
#define LED1 25

#define LED2 26
#define LED3 27
#define OUT1 28
#define OUT2 29
#define OUT3 30
#define OUT4 31 
#define OUT5 32
#define OUT6 33

void OutputDeviceGpioInit(void);
void OutputDeviceSet(u8 location, u8 SetFlg);
#endif /* HARDWARE_OUTPUTDEVICE_H_ */
