/*
 * InputDevice.c
 *
 *  Created on: 2020年2月22日
 *      Author: Hcc
 */

#include "InputDevice.h"


static  GPIO_TypeDef* s_paGPIO_Pro[50] =
{
GPIOI,GPIOI,GPIOI,GPIOI,GPIOA,GPIOC,GPIOC,
GPIOC,GPIOA,GPIOA,GPIOA,GPIOH,GPIOH,GPIOH,
GPIOG,GPIOG,GPIOG,GPIOG,GPIOC,GPIOC,GPIOC,
GPIOD,GPIOD,GPIOD,GPIOD,GPIOG,GPIOG,GPIOG,
GPIOI,
GPIOB,GPIOH,GPIOH,GPIOH,GPIOH,GPIOH,GPIOH

};

static  u16 s_aGPIO_Pin[50] =
{
	GPIO_Pin_0,GPIO_Pin_1,GPIO_Pin_2,GPIO_Pin_3,GPIO_Pin_15,GPIO_Pin_11,GPIO_Pin_11,
	GPIO_Pin_9,GPIO_Pin_8,GPIO_Pin_11,GPIO_Pin_12,GPIO_Pin_13,GPIO_Pin_14,GPIO_Pin_15,
	GPIO_Pin_5,GPIO_Pin_6,GPIO_Pin_7,GPIO_Pin_8,GPIO_Pin_6,GPIO_Pin_7,GPIO_Pin_8,
	GPIO_Pin_12,GPIO_Pin_13,GPIO_Pin_14,GPIO_Pin_15,GPIO_Pin_2,GPIO_Pin_3,GPIO_Pin_4,
	GPIO_Pin_8,
	GPIO_Pin_12,GPIO_Pin_12,GPIO_Pin_11,GPIO_Pin_10,GPIO_Pin_9,GPIO_Pin_8,GPIO_Pin_7
};


 /************************************************************************/
  /* Function Name:	InputDeviceGpioInit							            */
  /* Purpose:	    所有的输入设备GPIO初始化					*/
  /*																		*/
  /* Resources:		    None		 							*/
  /* Params:			None.												*/
  /* Return:			None											*/
  /* Limitation:		None.												*/
  /************************************************************************/
void InputDeviceGpioInit(void)
{
	int i = 0;
	for (i = 0; i < 36; i++)
	{
		GPIO_In_Up(s_paGPIO_Pro[i], s_aGPIO_Pin[i]);
	}
}
/************************************************************************/
 /* Function Name:	ReadInputDeviceSignal							            */
 /* Purpose:	     读取输入设备IO信号					*/
 /*																		*/
 /* Resources:		 None		 							*/
 /* Params:		   	 None.												*/
 /* Return:			   确认有输入为RESET 否则为SET											*/
 /* Limitation:		 None.												*/
 /************************************************************************/
extern TimeBase timebase;
u8  InputDeviceCnt[OL2];
u8 ReadInputDeviceSignal(u8 location)
{   
	if (timebase.Time10msFlg == SET)
	{
		timebase.Time10msFlg = RESET;
		if (GPIO_ReadInputDataBit(s_paGPIO_Pro[location], s_aGPIO_Pin[location]) == RESET)
		{
		InputDeviceCnt[location]++;
		}

	}
	if (InputDeviceCnt[location] > 10)
	{
		InputDeviceCnt[location] = 0;
		return RESET;
	}
	return SET;
}

extern State BitState;
void SignalDetection(void)
{
	if (ReadInputDeviceSignal(Key1) == RESET && ReadInputDeviceSignal(Key2) == RESET)
	{
	  UsartSendStartCom();//发送开始命令
		
	}
	if (ReadInputDeviceSignal(SLC) == RESET)
	{
		BitState.SLCFlg = SET;	//安全光栅
	}
	else
	{
		BitState.SLCFlg = RESET;
	}
	if (ReadInputDeviceSignal(OL1) == RESET)//产品在位1
	{
		BitState.OL1Flg = SET;
	}
	else
	{
		BitState.OL1Flg = RESET;
	}
	if (ReadInputDeviceSignal(OL2) == RESET)//产品在位2
	{
		BitState.OL2Flg = SET;
	}
	else
	{
		BitState.OL2Flg = RESET;
	}
	if (ReadInputDeviceSignal(Key3) == RESET)//急停
	{
		BitState.EmergencySwFlg = SET;
	}
	else
	{
		BitState.EmergencySwFlg = RESET;
	}
	if (ReadInputDeviceSignal(MP1) == RESET)
	{
		BitState.MP1Flg = SET;
	}
	else
	{
		BitState.MP1Flg = RESET;
	}
	if (ReadInputDeviceSignal(MP2) == RESET)
	{
		BitState.MP2Flg = SET;
	}
	else
	{
		BitState.MP2Flg = RESET;
	}
	if (ReadInputDeviceSignal(MP3) == RESET)
	{
		BitState.MP3Flg = SET;
	}
	else
	{
		BitState.MP3Flg = RESET;
	}
	if (ReadInputDeviceSignal(MP4) == RESET)
	{
		BitState.MP4Flg = SET;
	}
	else
	{
		BitState.MP4Flg = RESET;
	}
	if (ReadInputDeviceSignal(MP5) == RESET)
	{
		BitState.MP5Flg = SET;
	}
	else
	{
		BitState.MP5Flg = RESET;
	}
	if (ReadInputDeviceSignal(MP6) == RESET)
	{
		BitState.MP6Flg = SET;
	}
	else
	{
		BitState.MP6Flg = RESET;
	}
	if (ReadInputDeviceSignal(MP7) == RESET)
	{
		BitState.MP7Flg = SET;
	}
	else
	{
		BitState.MP7Flg = RESET;
	}
	if (ReadInputDeviceSignal(MP8) == RESET)
	{
		BitState.MP8Flg = SET;
	}
	else
	{
		BitState.MP8Flg = RESET;
	}
	if (ReadInputDeviceSignal(MP9) == RESET)
	{
		BitState.MP9Flg = SET;
	}
	else
	{
		BitState.MP9Flg = RESET;
	}
	if (ReadInputDeviceSignal(MP10) == RESET)
	{
		BitState.MP10Flg = SET;
	}
	else
	{
		BitState.MP10Flg = RESET;
	}
	if (ReadInputDeviceSignal(MP11) == RESET)
	{
		BitState.MP11Flg = SET;
	}
	else
	{
		BitState.MP11Flg = RESET;
	}
	if (ReadInputDeviceSignal(MP12) == RESET)
	{
		BitState.MP12Flg = SET;
	}
	else
	{
		BitState.MP12Flg = RESET;
	}
	if (ReadInputDeviceSignal(MP13) == RESET)
	{
		BitState.MP13Flg = SET;
	}
	else
	{
		BitState.MP13Flg = RESET;
	}
	if (ReadInputDeviceSignal(MP14) == RESET)
	{
		BitState.MP14Flg = SET;
	}
	else
	{
		BitState.MP14Flg = RESET;
	}
	if (ReadInputDeviceSignal(MP15) == RESET)
	{
		BitState.MP15Flg = SET;
	}
	else
	{
		BitState.MP15Flg = RESET;
	}
	if (ReadInputDeviceSignal(MP16) == RESET)
	{
		BitState.MP16Flg = SET;
	}
	else
	{
		BitState.MP16Flg = RESET;
	}
	if (ReadInputDeviceSignal(MP17) == RESET)
	{
		BitState.MP17Flg = SET;
	}
	else
	{
		BitState.MP17Flg = RESET;
	}
	if (ReadInputDeviceSignal(MP18) == RESET)
	{
		BitState.MP18Flg = SET;
	}
	else
	{
		BitState.MP18Flg = RESET;
	}
	if (ReadInputDeviceSignal(MP19) == RESET)
	{
		BitState.MP19Flg = SET;
	}
	else
	{
		BitState.MP19Flg = RESET;
	}
	if (ReadInputDeviceSignal(MP20) == RESET)
	{
		BitState.MP20Flg = SET;
	}
	else
	{
		BitState.MP20Flg = RESET;
	}
	if (ReadInputDeviceSignal(MP21) == RESET)
	{
		BitState.MP21Flg = SET;
	}
	else
	{
		BitState.MP21Flg = RESET;
	}
	if (ReadInputDeviceSignal(MP22) == RESET)
	{
		BitState.MP22Flg = SET;
	}
	else
	{
		BitState.MP22Flg = RESET;
	}
	if (ReadInputDeviceSignal(MP23) == RESET)
	{
		BitState.MP23Flg = SET;
	}
	else
	{
		BitState.MP23Flg = RESET;
	}
	if (ReadInputDeviceSignal(MP24) == RESET)
	{
		BitState.MP24Flg = SET;
	}
	else
	{
		BitState.MP24Flg = RESET;
	}
	if (ReadInputDeviceSignal(MP25) == RESET)
	{
		BitState.MP25Flg = SET;
	}
	else
	{
		BitState.MP25Flg = RESET;
	}

}

