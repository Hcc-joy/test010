#include "pwm.h"
#include "delay.h"
#include "Motor.h"

static  GPIO_TypeDef* s_paMotorGPIO_Pro[4][3] =
{
	{GPIOF,GPIOE,GPIOI}, //Motor1
	{GPIOF,GPIOI,GPIOI}, //Motor2
	{GPIOF,GPIOE,GPIOE}, //Motor3
	{GPIOF,GPIOE,GPIOE}  //Motor4

};

static  u16 s_aMotorGPIO_Pin[4][3] =
{
	{GPIO_Pin_6,GPIO_Pin_2,GPIO_Pin_7},
	{GPIO_Pin_7,GPIO_Pin_6,GPIO_Pin_5},
	{GPIO_Pin_8,GPIO_Pin_6,GPIO_Pin_5},
	{GPIO_Pin_9,GPIO_Pin_4,GPIO_Pin_3}
};

void MotorrTIM_Init(u16 arr,u16 psc)
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4,ENABLE);  ///使能TIM时钟
	
  TIM_TimeBaseInitStructure.TIM_Period = arr; 	//自动重装载值
	TIM_TimeBaseInitStructure.TIM_Prescaler=psc;  //定时器分频
	TIM_TimeBaseInitStructure.TIM_CounterMode=TIM_CounterMode_Up; //向上计数模式
	TIM_TimeBaseInitStructure.TIM_ClockDivision=TIM_CKD_DIV1; 
	
	TIM_TimeBaseInit(TIM4,&TIM_TimeBaseInitStructure);//初始化TIM
	
	TIM_ITConfig(TIM4,TIM_IT_Update,ENABLE); //允许定时器3更新中断
	TIM_Cmd(TIM4,DISABLE); //
	
	NVIC_InitStructure.NVIC_IRQChannel=TIM4_IRQn; //定时器3中断
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=0x00; //抢占优先级1
	NVIC_InitStructure.NVIC_IRQChannelSubPriority=0x00; //子优先级3
	NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE;
	NVIC_Init(&NVIC_InitStructure);
}
long int g_iMotor1PulseNum;
void TIM4_IRQHandler(void)
{
	if(TIM_GetITStatus(TIM4,TIM_IT_Update)==SET) //溢出中断
	{
			if (g_iMotor1PulseNum != 0)
			{
			g_iMotor1PulseNum--;
			}
			else
			{
				TIM_Cmd(TIM10, DISABLE);  // DIS 
				TIM_Cmd(TIM11, DISABLE);  // DIS 
				TIM_Cmd(TIM13, DISABLE);  // DIS 
				TIM_Cmd(TIM14, DISABLE);  // DIS 
			}
	}
	TIM_ClearITPendingBit(TIM4,TIM_IT_Update);  //清除中断标志位
}

void Motor_GPIOInit(void)
{
	int s_i, s_j;
  TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
  GPIO_InitTypeDef GPIO_InitStructure;
  TIM_OCInitTypeDef TIM_OCInitStructure;
	for (s_i = 0; s_i < 4; s_i++)
	{
		for (s_j = 1; s_j < 3; s_j++)
		{
			GPIO_Out_Push_Pull(s_paMotorGPIO_Pro[s_i][s_j], s_aMotorGPIO_Pin[s_i][s_j], 1);
		}
	}
	
	GPIO_PinAFConfig(GPIOF,GPIO_PinSource9,GPIO_AF_TIM14); //GPIOF9复用为定时器14
	GPIO_PinAFConfig(GPIOF,GPIO_PinSource8,GPIO_AF_TIM13); //GPIOF8复用为定时器13
	GPIO_PinAFConfig(GPIOF,GPIO_PinSource7,GPIO_AF_TIM11); //GPIOF7复用为定时器11
	GPIO_PinAFConfig(GPIOF,GPIO_PinSource6,GPIO_AF_TIM10); //GPIOF6复用为定时器10
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9|GPIO_Pin_8|GPIO_Pin_7|GPIO_Pin_6;           //GPIOF 9 8 7 6
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;        //复用功能
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	//速度100MHz
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;      //推挽复用输出
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;        //上拉
	GPIO_Init(GPIOF,&GPIO_InitStructure);       
	
	

RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM11,ENABLE);
RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM10,ENABLE);
RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM14,ENABLE);  	//TIM14时钟使能    
RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM13,ENABLE);  	//TIM13时钟使能   	
	

TIM_TimeBaseInitStructure.TIM_Period = 1000 - 1;
TIM_TimeBaseInitStructure.TIM_Prescaler = 90 - 1;
TIM_TimeBaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;
TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up;
TIM_TimeBaseInit(TIM13,&TIM_TimeBaseInitStructure);
TIM_TimeBaseInit(TIM14,&TIM_TimeBaseInitStructure);

TIM_TimeBaseInitStructure.TIM_Period = 2000 - 1;
TIM_TimeBaseInitStructure.TIM_Prescaler = 90 - 1;
TIM_TimeBaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;
TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up;
TIM_TimeBaseInit(TIM11,&TIM_TimeBaseInitStructure);
TIM_TimeBaseInit(TIM10,&TIM_TimeBaseInitStructure);

TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_Low;
TIM_OC1Init(TIM11,&TIM_OCInitStructure);
TIM_OC1Init(TIM10,&TIM_OCInitStructure);
TIM_OC1Init(TIM13,&TIM_OCInitStructure);
TIM_OC1Init(TIM14,&TIM_OCInitStructure);

TIM_OC1PreloadConfig(TIM11,TIM_OCPreload_Enable);
TIM_OC1PreloadConfig(TIM10,TIM_OCPreload_Enable);
TIM_OC1PreloadConfig(TIM13,TIM_OCPreload_Enable);
TIM_OC1PreloadConfig(TIM14,TIM_OCPreload_Enable);

TIM_ARRPreloadConfig(TIM11,ENABLE);
TIM_ARRPreloadConfig(TIM10,ENABLE);
TIM_ARRPreloadConfig(TIM13,ENABLE);
TIM_ARRPreloadConfig(TIM14,ENABLE);

TIM_Cmd(TIM11,DISABLE);
TIM_Cmd(TIM10,DISABLE);	
TIM_Cmd(TIM13,DISABLE);
TIM_Cmd(TIM14,DISABLE);
}
void StepMotor_Init(void)
{
    Motor_GPIOInit();
	MotorrTIM_Init(10-1,9000-1);//用于电机步数计数
   // TIM_PWM_Init(TIM14,1000 - 1, 90 - 1);	//90M/90=1Mhz的计数频率,重装载值500，所以PWM频率为 1M/500=2Khz.  
   // TIM_PWM_Init(TIM13, 1000 - 1, 90 - 1);
   // TIM_PWM_Init(TIM11, 1000 - 1, 90 - 1);
   //TIM_PWM_Init(TIM10, 1000 - 1, 90 - 1);
	
}

//TIM PWM部分初始化 
//PWM输出初始化
//arr：自动重装值
//psc：时钟预分频数
void TIM_PWM_Init(TIM_TypeDef * TIMx,u32 arr,u32 psc)//该函数不使用
{		 					 
	//此部分需手动修改IO口设置
	

	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	TIM_OCInitTypeDef  TIM_OCInitStructure;
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM14,ENABLE);  	//TIM14时钟使能    
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM13,ENABLE);  	//TIM13时钟使能   
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM11, ENABLE);
	RCC_AHB2PeriphClockCmd(RCC_APB2Periph_TIM10, ENABLE);
	
//	TIM_DeInit(TIMx);  
	TIM_TimeBaseStructure.TIM_Period=arr;   //自动重装载值
	TIM_TimeBaseStructure.TIM_Prescaler=psc;  //定时器分频
	TIM_TimeBaseStructure.TIM_ClockDivision=TIM_CKD_DIV1; 
	TIM_TimeBaseStructure.TIM_CounterMode=TIM_CounterMode_Up; //向上计数模式
	TIM_TimeBaseInit(TIMx,&TIM_TimeBaseStructure);//初始化定时器14
	TIM_ITConfig(TIMx,TIM_IT_Update,ENABLE );//开启中断使能

	//初始化TIM14 Channel1 PWM模式	 
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1; //选择定时器模式:TIM脉冲宽度调制模式2
 	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; //比较输出使能
	TIM_OCInitStructure.TIM_Pulse=0;
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_Low; //输出极性:TIM输出比较极性低
	TIM_OC1Init(TIMx, &TIM_OCInitStructure);  //根据T指定的参数初始化外设TIM1 4OC1

	TIM_OC1PreloadConfig(TIMx, TIM_OCPreload_Enable);  //使能TIM14在CCR1上的预装载寄存器
 
   TIM_ARRPreloadConfig(TIMx,ENABLE);//ARPE使能 
	TIM_CtrlPWMOutputs(TIMx, ENABLE);
	TIM_Cmd(TIMx, ENABLE);  //使能TIM14

}  
u32 MotorStartRun(u8 MotorNum,TIM_TypeDef* TIMx,u8 Orientation,int arr, long int Rotate)
{
	u32 m_TempData_32=0;
	TIM_SetCompare1(TIMx, arr );
	GPIO_ResetBits(s_paMotorGPIO_Pro[MotorNum][Motor_EN],s_aMotorGPIO_Pin[MotorNum][Motor_EN]);//电机使能
	if(Orientation==MotorFwd)
	{
	GPIO_ResetBits(s_paMotorGPIO_Pro[MotorNum][Motor_DIR],s_aMotorGPIO_Pin[MotorNum][Motor_DIR]);
	}
	else
	{
	GPIO_SetBits(s_paMotorGPIO_Pro[MotorNum][Motor_DIR],s_aMotorGPIO_Pin[MotorNum][Motor_DIR]);
	}
	TIM_Cmd(TIMx, ENABLE);  //使能TIM
	TIM_Cmd(TIM4, ENABLE);  //使能TIM
	g_iMotor1PulseNum = Rotate;
       //此处读取扭力
	
		switch (MotorNum)
		{
		case Motor1:
			m_TempData_32 = Torque_Check(CH_2, TM7707_U27);
			break;
		case Motor2:
			m_TempData_32 = Torque_Check(CH_1, TM7707_U1);
			break;
		case Motor3:
			m_TempData_32 = Torque_Check(CH_2, TM7707_U1);
			break;
		case Motor4:
			m_TempData_32 = Torque_Check(CH_1, TM7707_U6);
			break;
		default:
			break;
		}
    while(g_iMotor1PulseNum);
	TIM_Cmd(TIM4, DISABLE);  // DIS 
	GPIO_SetBits(s_paMotorGPIO_Pro[MotorNum][Motor_EN],s_aMotorGPIO_Pin[MotorNum][Motor_EN]);
	return m_TempData_32;
}

