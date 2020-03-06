/*
 * Tim.h
 *
 *  Created on: 2020Äê1ÔÂ10ÈÕ
 *      Author: Hcc
 */

#ifndef HARDWARE_TIM_H_
#define HARDWARE_TIM_H_
#include "sys.h"
#include "WDog.h"

typedef struct TimeBase
{
	u8 Time1msFlg : 1;
	u8 Time10msFlg : 1;
	u8 Time100msFlg : 1;
	u8 Time500msFlg : 1;
	u8 Time1000Flg : 1;
	u8 Time1100Flg : 1;
	u8 WdogFlg : 1;
	u8 : 1;
	int Time1msCnt;
} TimeBase;


void MotorStopRun(void);
void TIM3_Int_Init(u16 arr,u16 psc);

#endif /* HARDWARE_TIM_H_ */
