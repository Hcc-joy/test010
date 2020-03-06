/*
 * Moto.c
 *
 *  Created on: 2020年1月15日
 *      Author: Hcc
 */

#include "Motor.h"
#include "main.h"	

static  GPIO_TypeDef* s_paDsthGPIO_Pro[4] =
{
GPIOI,GPIOF,GPIOF,GPIOF
};

static  u16 s_aDsthGPIO_Pin[4] =
{
	GPIO_Pin_11,GPIO_Pin_0,GPIO_Pin_1,GPIO_Pin_2

};
/************************************************************************/
 /* Function Name:	    Dsth_MotoInit							            */
 /* Purpose:	        锁体电机 初始化							*/
 /*																		*/
 /* Resources:		    None		 							*/
 /* Params:			None.												*/
 /* Return:			None											*/
 /* Limitation:		None.												*/
 /************************************************************************/
void Dsth_MotorInit(void)
{
	GPIO_Out_Push_Pull(GPIOI, GPIO_Pin_10, 0);//IN2
	GPIO_Out_Push_Pull(GPIOI, GPIO_Pin_9, 0);//IN2
	GPIO_Out_Push_Pull(GPIOC, GPIO_Pin_13, 1);//nsleep
	GPIO_In_Float(GPIOI, GPIO_Pin_11);//op1  回拖
	GPIO_In_Up(GPIOF, GPIO_Pin_0);//op2      空打
	GPIO_In_Float(GPIOF, GPIO_Pin_1);//op3   锁舌
	GPIO_In_Up(GPIOF, GPIO_Pin_2);//SLOT     检测关门
	Dsth_MotorADCInit();
}
/************************************************************************/
/* Function Name:	Dsth_MotoADCInit							         */
/* Purpose:	    ADC 初始化											*/
/*																		*/
/* Resources:		None		 										*/
/* Params:			None.												*/
/* Return:			None												*/
/* Limitation:		None.												*/
/************************************************************************/
void Dsth_MotorADCInit(void)
{   //PF5 测试电机电压
	//PF3 计算电机动作电流
	GPIO_InitTypeDef  GPIO_InitStructure;
	ADC_CommonInitTypeDef ADC_CommonInitStructure;
	ADC_InitTypeDef       ADC_InitStructure;

	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOF, ENABLE);//使能GPIOF时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC3, ENABLE); //使能ADC3时钟

	//先初始化ADC1通道5 IO口
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5 | GPIO_Pin_3;//PF5
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AN;//模拟输入
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;//不带上下拉
	GPIO_Init(GPIOF, &GPIO_InitStructure);//初始化  

	RCC_APB2PeriphResetCmd(RCC_APB2Periph_ADC3, ENABLE);	  //ADC1复位
	RCC_APB2PeriphResetCmd(RCC_APB2Periph_ADC3, DISABLE);	//复位结束	 


	ADC_CommonInitStructure.ADC_Mode = ADC_Mode_Independent;//独立模式
	ADC_CommonInitStructure.ADC_TwoSamplingDelay = ADC_TwoSamplingDelay_5Cycles;//两个采样阶段之间的延迟5个时钟
	ADC_CommonInitStructure.ADC_DMAAccessMode = ADC_DMAAccessMode_Disabled; //DMA失能
	ADC_CommonInitStructure.ADC_Prescaler = ADC_Prescaler_Div4;//预分频4分频。ADCCLK=PCLK2/4=84/4=21Mhz,ADC时钟最好不要超过36Mhz 
	ADC_CommonInit(&ADC_CommonInitStructure);//初始化

	ADC_InitStructure.ADC_Resolution = ADC_Resolution_12b;//12位模式
	ADC_InitStructure.ADC_ScanConvMode = DISABLE;//非扫描模式	
	ADC_InitStructure.ADC_ContinuousConvMode = DISABLE;//关闭连续转换
	ADC_InitStructure.ADC_ExternalTrigConvEdge = ADC_ExternalTrigConvEdge_None;//禁止触发检测，使用软件触发
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;//右对齐	
	ADC_InitStructure.ADC_NbrOfConversion = 1;//1个转换在规则序列中 也就是只转换规则序列1 
	ADC_Init(ADC3, &ADC_InitStructure);//ADC初始化
	ADC_Cmd(ADC3, ENABLE);//开启AD转换器	
}


/************************************************************************/
 /* Function Name:	Get_Adc							            */
 /* Purpose:	    获取ADC值					*/
 /*																		*/
 /* Resources:		None		 							*/
 /* Params:			ADC_Channel_0~ADC_Channel_16												*/
 /* Return:			转换结果											*/
 /* Limitation:		None.												*/
 /************************************************************************/
u16 Get_Adc(ADC_TypeDef* ADCx, u8 ch)
{
	//设置指定ADC的规则组通道，一个序列，采样时间
	ADC_RegularChannelConfig(ADCx, ch, 1, ADC_SampleTime_480Cycles);	//ADC1,ADC通道,480个周期,提高采样时间可以提高精确度			    

	ADC_SoftwareStartConv(ADCx);		//使能指定的ADC1的软件转换启动功能	

	while (!ADC_GetFlagStatus(ADCx, ADC_FLAG_EOC));//等待转换结束

	return ADC_GetConversionValue(ADCx);	//返回最近一次ADC1规则组的转换结果
}
/************************************************************************/
 /* Function Name:	Get_Adc_Average							            */
 /* Purpose:	    获取ADC值					*/
 /*																		*/
 /* Resources:		None		 							*/
 /* Params:			ADC_Channel_0~ADC_Channel_16												*/
 /* Return:			转换结果											*/
 /* Limitation:		None.												*/
 /************************************************************************/
u16 Get_Adc_Average(ADC_TypeDef* ADCx, u8 ch, u8 times)
{
	u32 temp_val = 0;
	u8 t;
	for (t = 0; t < times; t++)
	{
		temp_val += Get_Adc(ADCx, ch);
	}
	return temp_val / times;
}

/************************************************************************/
 /* Function Name:	Dsth_MotoADCInit							            */
 /* Purpose:	    ADC 初始化					*/
 /*																		*/
 /* Resources:		None		 							*/
 /* Params:			None.												*/
 /* Return:			None											*/
 /* Limitation:		None.												*/
 /************************************************************************/
int DsthMotorRunTime;
u32 DsthSensorState = 0;
u16 Dsth_Motor(u8 Flg, u8 DetectionSensorFlg)
{
	u16 temp = 0;
	u32 m_DataTempVualeM;
	DsthMotorRunTime = 200;
	if (Flg == MotorFwd)//开门
	{
		MotorForward();
	}
	else//关门
	{
      	MotorInvert();
	}
	delay_ms(100);
	temp = Get_Adc_Average(DsthMotorCurrnetADCx, DsthMotorCurrnetADCChannel, GetANum) * (3300.0 / 4096) / 0.5;//读电流
	while (DsthMotorRunTime)
	{

		if (GPIO_ReadInputDataBit(s_paDsthGPIO_Pro[BackDrag], s_aDsthGPIO_Pin[BackDrag]) == RESET && Flg == MotorRev)
		{
				delay_ms(5);
			
			if (GPIO_ReadInputDataBit(s_paDsthGPIO_Pro[BackDrag], s_aDsthGPIO_Pin[BackDrag]) == RESET)
			{
				DsthSensorState |= (0x01 << 8);//锁舌
				delay_ms(50);
				MotorStop();
				delay_ms(100);
				MotorForward();
				delay_ms(100);
				while (DsthMotorRunTime)
				{
					if (GPIO_ReadInputDataBit(s_paDsthGPIO_Pro[DoorLockTongue], s_aDsthGPIO_Pin[DoorLockTongue]) == RESET)
					{
						delay_ms(5);
						if (GPIO_ReadInputDataBit(s_paDsthGPIO_Pro[DoorLockTongue], s_aDsthGPIO_Pin[DoorLockTongue]) == RESET)
						{
							DsthSensorState |= (0x01 << 24);//回拖
							MotorStop();
							delay_ms(5);
							MotorSleep();
							DsthMotorRunTime=0;//退出
						}
					}
				}
			}
		}
		else if (Flg == MotorFwd && GPIO_ReadInputDataBit(s_paDsthGPIO_Pro[BackDrag], s_aDsthGPIO_Pin[BackDrag]) == SET)
		{
			delay_ms(100);
			if (GPIO_ReadInputDataBit(s_paDsthGPIO_Pro[BackDrag], s_aDsthGPIO_Pin[BackDrag]) == SET)
			{
			  delay_ms(100);
				MotorStop();
				delay_ms(100);
				MotorInvert();
				delay_ms(100);
        while (DsthMotorRunTime)
				{
					if (GPIO_ReadInputDataBit(s_paDsthGPIO_Pro[DoorLockTongue], s_aDsthGPIO_Pin[DoorLockTongue]) == RESET)
					{
						delay_ms(5);
						if (GPIO_ReadInputDataBit(s_paDsthGPIO_Pro[DoorLockTongue], s_aDsthGPIO_Pin[DoorLockTongue]) == RESET)
						{
							MotorStop();
							delay_ms(5);
							MotorSleep();
							DsthMotorRunTime=0;//退出
						}
					}
				}
			}

		}
	}
	MotorSleep();
	return  temp;
}

/************************************************************************/
 /* Function Name:	DsthSensorDetection							            */
 /* Purpose:	   读取锁体传感器状态					*/
 /*																		*/
 /* Resources:		None		 							*/
 /* Params:			None.												*/
 /* Return:			None											*/
 /* Limitation:		None.												*/
 /************************************************************************/
u32 DsthSensorDetection(void)
{
	  u8 abc[10];
 
	if (GPIO_ReadInputDataBit(s_paDsthGPIO_Pro[DoorLockTongue], s_aDsthGPIO_Pin[DoorLockTongue]) == RESET)
	{
		DsthSensorState |= (0x01 << 24);
	}
	if (GPIO_ReadInputDataBit(s_paDsthGPIO_Pro[LockClose], s_aDsthGPIO_Pin[LockClose]) == RESET)
	{
		DsthSensorState |= (0x01 << 16);
	}
	if (GPIO_ReadInputDataBit(s_paDsthGPIO_Pro[BackDrag], s_aDsthGPIO_Pin[BackDrag]) == RESET)
	{
		DsthSensorState |= (0x01 << 8);
	}
	if (GPIO_ReadInputDataBit(s_paDsthGPIO_Pro[SlottedTongue], s_aDsthGPIO_Pin[SlottedTongue]) == SET)
	{
		DsthSensorState |= 0x01;
	}
		abc[5] = DsthSensorState >> 24;
		abc[6] = DsthSensorState >> 16;
		abc[7] = DsthSensorState >> 8;
		abc[8] = DsthSensorState;
	 // Ack_SendData(abc, 5);
	return DsthSensorState;
}