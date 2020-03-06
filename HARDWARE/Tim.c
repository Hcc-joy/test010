/*
 * Tim.c
 *
 *  Created on: 2020年1月10日
 *      Author: Hcc
 */
#include "Tim.h"
#include "Motor.h"
#include "string.h"	
//通用定时器3中断初始化
//arr：自动重装值。
//psc：时钟预分频数
//定时器溢出时间计算方法:Tout=((arr+1)*(psc+1))/Ft us.
//Ft=定时器工作频率,单位:Mhz
//这里使用的是定时器3!
void TIM3_Int_Init(u16 arr,u16 psc)
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3,ENABLE);  ///使能TIM3时钟
	
  TIM_TimeBaseInitStructure.TIM_Period = arr; 	//自动重装载值
	TIM_TimeBaseInitStructure.TIM_Prescaler=psc;  //定时器分频
	TIM_TimeBaseInitStructure.TIM_CounterMode=TIM_CounterMode_Up; //向上计数模式
	TIM_TimeBaseInitStructure.TIM_ClockDivision=TIM_CKD_DIV1; 
	
	TIM_TimeBaseInit(TIM3,&TIM_TimeBaseInitStructure);//初始化TIM3
	
	TIM_ITConfig(TIM3,TIM_IT_Update,ENABLE); //允许定时器3更新中断
	TIM_Cmd(TIM3,ENABLE); //使能定时器3
	
	NVIC_InitStructure.NVIC_IRQChannel=TIM3_IRQn; //定时器3中断
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=0x00; //抢占优先级1
	NVIC_InitStructure.NVIC_IRQChannelSubPriority=0x01; //子优先级3
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
//定时器3中断服务函数
void TIM3_IRQHandler(void)
{
	if(TIM_GetITStatus(TIM3,TIM_IT_Update)==SET) //溢出中断
	{
		TimeBasefun();

	}
	TIM_ClearITPendingBit(TIM3,TIM_IT_Update);  //清除中断标志位
}

extern int DsthMotorRunTime;
void MotorStopRun(void)
{
	if (DsthMotorRunTime != 0)
	{
		DsthMotorRunTime--;
	}

}

