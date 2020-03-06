/*
 * Moto.h
 *
 *  Created on: 2020Äê1ÔÂ15ÈÕ
 *      Author: Hcc
 */

#ifndef HARDWARE_MOTO_H_
#define HARDWARE_MOTO_H_
#include "sys.h"
#include "GPIOlib.h"
#include "delay.h"
#include "stm32f4xx_adc.h"
#include "string.h"	


#define DsthMotorVoltageADCx  ADC3
#define DsthMotorVoltageADCChannel ADC_Channel_15
#define GetVNum 10

#define DsthMotorCurrnetADCx  ADC3
#define DsthMotorCurrnetADCChannel ADC_Channel_9
#define GetANum 100

#define MotorFwd 0
#define MotorRev 1

#define DoorLockTongue 0
#define LockClose	1
#define BackDrag	2  
#define SlottedTongue 3

#define DsthMotorIN1(n) (n==0?GPIO_ResetBits(GPIOI,GPIO_Pin_9):GPIO_SetBits(GPIOI,GPIO_Pin_9))
#define DsthMotorIN2(n) (n==0?GPIO_ResetBits(GPIOI,GPIO_Pin_10):GPIO_SetBits(GPIOI,GPIO_Pin_10))

#define MotorStop()		DsthMotorIN1(1);DsthMotorIN2(1)
#define MotorForward()  DsthMotorIN1(1);DsthMotorIN2(0)
#define MotorInvert()	DsthMotorIN1(0);DsthMotorIN2(1)
#define MotorSleep()	DsthMotorIN1(0);DsthMotorIN2(0)

void Dsth_MotorInit(void);
void Dsth_MotorADCInit(void);

u16 Get_Adc_Average(ADC_TypeDef* ADCx, u8 ch, u8 times);
u16 Dsth_Motor(u8 Flg, u8 DetectionSensorFlg);
u32 DsthSensorDetection(void);
#endif /* HARDWARE_MOTO_H_ */
