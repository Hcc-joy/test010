/*
 * OutputDevice.c
 *
 *  Created on: 2020年2月22日
 *      Author: Hcc
 */
#include "OutputDevice.h"


static  GPIO_TypeDef* s_paGPIO_Pro[50] =
{
GPIOC,GPIOD,GPIOD,GPIOD,GPIOD,GPIOD,GPIOD,
GPIOD,GPIOD,GPIOG,GPIOG,GPIOG,GPIOG,GPIOG,
GPIOG,GPIOG,GPIOB,GPIOB,GPIOB,GPIOI,
GPIOD,GPIOD,GPIOD,GPIOD,GPIOB,GPIOB,GPIOB,
GPIOE,GPIOE,GPIOB,GPIOB,GPIOB,GPIOB

};

static  u16 s_aGPIO_Pin[50] =
{
	GPIO_Pin_12,GPIO_Pin_0,GPIO_Pin_1,GPIO_Pin_2,GPIO_Pin_3,GPIO_Pin_4,GPIO_Pin_5,
	GPIO_Pin_6,GPIO_Pin_7,GPIO_Pin_9,GPIO_Pin_10,GPIO_Pin_11,GPIO_Pin_12,GPIO_Pin_13,
	GPIO_Pin_14,GPIO_Pin_15,GPIO_Pin_3,GPIO_Pin_4,GPIO_Pin_5,GPIO_Pin_4,
	GPIO_Pin_11,GPIO_Pin_10,GPIO_Pin_9,GPIO_Pin_8,GPIO_Pin_15,GPIO_Pin_14,GPIO_Pin_13,
	GPIO_Pin_1,GPIO_Pin_0,GPIO_Pin_9,GPIO_Pin_8,GPIO_Pin_7,GPIO_Pin_6

};

/************************************************************************/
 /* Function Name:	OutputDeviceGpioInit							            */
 /* Purpose:	    所有的输出设备GPIO初始化					*/
 /*																		*/
 /* Resources:		    None		 							*/
 /* Params:			None.												*/
 /* Return:			None											*/
 /* Limitation:		None.												*/
 /************************************************************************/
void OutputDeviceGpioInit(void)
{
	int i = 0;
	for (i = 0; i <34 ; i++)
	{
		GPIO_Out_Push_Pull(s_paGPIO_Pro[i], s_aGPIO_Pin[i],RESET);
	}
}
/************************************************************************/
 /* Function Name:	OutputDeviceSet							            */
 /* Purpose:	    输出设备IO置位				*/
 /*																		*/
 /* Resources:		None location IO位置		 							*/
 /* Params:			None.												*/
 /* Return:			None											*/
 /* Limitation:		None.												*/
 /************************************************************************/
void OutputDeviceSet(u8 location, u8 SetFlg)
{
	if (SetFlg == SET)
	{
		GPIO_SetBits(s_paGPIO_Pro[location], s_aGPIO_Pin[location]);
	}
	else
	{
		GPIO_ResetBits(s_paGPIO_Pro[location], s_aGPIO_Pin[location]);
	}


}

