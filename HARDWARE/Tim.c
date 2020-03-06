/*
 * Tim.c
 *
 *  Created on: 2020��1��10��
 *      Author: Hcc
 */
#include "Tim.h"
#include "Motor.h"
#include "string.h"	
//ͨ�ö�ʱ��3�жϳ�ʼ��
//arr���Զ���װֵ��
//psc��ʱ��Ԥ��Ƶ��
//��ʱ�����ʱ����㷽��:Tout=((arr+1)*(psc+1))/Ft us.
//Ft=��ʱ������Ƶ��,��λ:Mhz
//����ʹ�õ��Ƕ�ʱ��3!
void TIM3_Int_Init(u16 arr,u16 psc)
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3,ENABLE);  ///ʹ��TIM3ʱ��
	
  TIM_TimeBaseInitStructure.TIM_Period = arr; 	//�Զ���װ��ֵ
	TIM_TimeBaseInitStructure.TIM_Prescaler=psc;  //��ʱ����Ƶ
	TIM_TimeBaseInitStructure.TIM_CounterMode=TIM_CounterMode_Up; //���ϼ���ģʽ
	TIM_TimeBaseInitStructure.TIM_ClockDivision=TIM_CKD_DIV1; 
	
	TIM_TimeBaseInit(TIM3,&TIM_TimeBaseInitStructure);//��ʼ��TIM3
	
	TIM_ITConfig(TIM3,TIM_IT_Update,ENABLE); //����ʱ��3�����ж�
	TIM_Cmd(TIM3,ENABLE); //ʹ�ܶ�ʱ��3
	
	NVIC_InitStructure.NVIC_IRQChannel=TIM3_IRQn; //��ʱ��3�ж�
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=0x00; //��ռ���ȼ�1
	NVIC_InitStructure.NVIC_IRQChannelSubPriority=0x01; //�����ȼ�3
	NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE;
	NVIC_Init(&NVIC_InitStructure);
}
TimeBase timebase;
extern int g_Time_OutCnt;
extern int g_Time_OutCnt;
extern u8  InputDeviceCnt[35];
void  TimeBasefun(void)
{
	timebase.Time1msCnt++;
	if (timebase.Time1msCnt % 10 == 0)
	{
		timebase.Time10msFlg = SET;
		MotorStopRun();
	}
	if (timebase.Time1msCnt % 100 == 0)
	{
		timebase.Time100msFlg = SET;
		if (timebase.WdogFlg == SET)
		{
			timebase.WdogFlg = RESET;
			FeedDogFinish();
		}
	}
	if (timebase.Time1msCnt % 500 == 0)
	{
		timebase.Time500msFlg = SET;
	    timebase.WdogFlg = SET;
		memset(InputDeviceCnt, 0x00, sizeof(InputDeviceCnt));//

		 StartFeedDog();
	}
	if (timebase.Time1msCnt % 1000 == 0)
	{
		timebase.Time500msFlg = SET;

        g_Time_OutCnt--;
		timebase.Time1msCnt = 0;
	}


}
//��ʱ��3�жϷ�����
void TIM3_IRQHandler(void)
{
	if(TIM_GetITStatus(TIM3,TIM_IT_Update)==SET) //����ж�
	{
		TimeBasefun();

	}
	TIM_ClearITPendingBit(TIM3,TIM_IT_Update);  //����жϱ�־λ
}

extern int DsthMotorRunTime;
void MotorStopRun(void)
{
	if (DsthMotorRunTime != 0)
	{
		DsthMotorRunTime--;
	}

}

