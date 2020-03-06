/*
 * Moto.c
 *
 *  Created on: 2020��1��15��
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
 /* Purpose:	        ������ ��ʼ��							*/
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
	GPIO_In_Float(GPIOI, GPIO_Pin_11);//op1  ����
	GPIO_In_Up(GPIOF, GPIO_Pin_0);//op2      �մ�
	GPIO_In_Float(GPIOF, GPIO_Pin_1);//op3   ����
	GPIO_In_Up(GPIOF, GPIO_Pin_2);//SLOT     ������
	Dsth_MotorADCInit();
}
/************************************************************************/
/* Function Name:	Dsth_MotoADCInit							         */
/* Purpose:	    ADC ��ʼ��											*/
/*																		*/
/* Resources:		None		 										*/
/* Params:			None.												*/
/* Return:			None												*/
/* Limitation:		None.												*/
/************************************************************************/
void Dsth_MotorADCInit(void)
{   //PF5 ���Ե����ѹ
	//PF3 ��������������
	GPIO_InitTypeDef  GPIO_InitStructure;
	ADC_CommonInitTypeDef ADC_CommonInitStructure;
	ADC_InitTypeDef       ADC_InitStructure;

	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOF, ENABLE);//ʹ��GPIOFʱ��
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC3, ENABLE); //ʹ��ADC3ʱ��

	//�ȳ�ʼ��ADC1ͨ��5 IO��
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5 | GPIO_Pin_3;//PF5
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AN;//ģ������
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;//����������
	GPIO_Init(GPIOF, &GPIO_InitStructure);//��ʼ��  

	RCC_APB2PeriphResetCmd(RCC_APB2Periph_ADC3, ENABLE);	  //ADC1��λ
	RCC_APB2PeriphResetCmd(RCC_APB2Periph_ADC3, DISABLE);	//��λ����	 


	ADC_CommonInitStructure.ADC_Mode = ADC_Mode_Independent;//����ģʽ
	ADC_CommonInitStructure.ADC_TwoSamplingDelay = ADC_TwoSamplingDelay_5Cycles;//���������׶�֮����ӳ�5��ʱ��
	ADC_CommonInitStructure.ADC_DMAAccessMode = ADC_DMAAccessMode_Disabled; //DMAʧ��
	ADC_CommonInitStructure.ADC_Prescaler = ADC_Prescaler_Div4;//Ԥ��Ƶ4��Ƶ��ADCCLK=PCLK2/4=84/4=21Mhz,ADCʱ����ò�Ҫ����36Mhz 
	ADC_CommonInit(&ADC_CommonInitStructure);//��ʼ��

	ADC_InitStructure.ADC_Resolution = ADC_Resolution_12b;//12λģʽ
	ADC_InitStructure.ADC_ScanConvMode = DISABLE;//��ɨ��ģʽ	
	ADC_InitStructure.ADC_ContinuousConvMode = DISABLE;//�ر�����ת��
	ADC_InitStructure.ADC_ExternalTrigConvEdge = ADC_ExternalTrigConvEdge_None;//��ֹ������⣬ʹ���������
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;//�Ҷ���	
	ADC_InitStructure.ADC_NbrOfConversion = 1;//1��ת���ڹ��������� Ҳ����ֻת����������1 
	ADC_Init(ADC3, &ADC_InitStructure);//ADC��ʼ��
	ADC_Cmd(ADC3, ENABLE);//����ADת����	
}


/************************************************************************/
 /* Function Name:	Get_Adc							            */
 /* Purpose:	    ��ȡADCֵ					*/
 /*																		*/
 /* Resources:		None		 							*/
 /* Params:			ADC_Channel_0~ADC_Channel_16												*/
 /* Return:			ת�����											*/
 /* Limitation:		None.												*/
 /************************************************************************/
u16 Get_Adc(ADC_TypeDef* ADCx, u8 ch)
{
	//����ָ��ADC�Ĺ�����ͨ����һ�����У�����ʱ��
	ADC_RegularChannelConfig(ADCx, ch, 1, ADC_SampleTime_480Cycles);	//ADC1,ADCͨ��,480������,��߲���ʱ�������߾�ȷ��			    

	ADC_SoftwareStartConv(ADCx);		//ʹ��ָ����ADC1�����ת����������	

	while (!ADC_GetFlagStatus(ADCx, ADC_FLAG_EOC));//�ȴ�ת������

	return ADC_GetConversionValue(ADCx);	//�������һ��ADC1�������ת�����
}
/************************************************************************/
 /* Function Name:	Get_Adc_Average							            */
 /* Purpose:	    ��ȡADCֵ					*/
 /*																		*/
 /* Resources:		None		 							*/
 /* Params:			ADC_Channel_0~ADC_Channel_16												*/
 /* Return:			ת�����											*/
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
 /* Purpose:	    ADC ��ʼ��					*/
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
	if (Flg == MotorFwd)//����
	{
		MotorForward();
	}
	else//����
	{
      	MotorInvert();
	}
	delay_ms(100);
	temp = Get_Adc_Average(DsthMotorCurrnetADCx, DsthMotorCurrnetADCChannel, GetANum) * (3300.0 / 4096) / 0.5;//������
	while (DsthMotorRunTime)
	{

		if (GPIO_ReadInputDataBit(s_paDsthGPIO_Pro[BackDrag], s_aDsthGPIO_Pin[BackDrag]) == RESET && Flg == MotorRev)
		{
				delay_ms(5);
			
			if (GPIO_ReadInputDataBit(s_paDsthGPIO_Pro[BackDrag], s_aDsthGPIO_Pin[BackDrag]) == RESET)
			{
				DsthSensorState |= (0x01 << 8);//����
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
							DsthSensorState |= (0x01 << 24);//����
							MotorStop();
							delay_ms(5);
							MotorSleep();
							DsthMotorRunTime=0;//�˳�
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
							DsthMotorRunTime=0;//�˳�
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
 /* Purpose:	   ��ȡ���崫����״̬					*/
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