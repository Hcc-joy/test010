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
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4,ENABLE);  ///ʹ��TIMʱ��
	
  TIM_TimeBaseInitStructure.TIM_Period = arr; 	//�Զ���װ��ֵ
	TIM_TimeBaseInitStructure.TIM_Prescaler=psc;  //��ʱ����Ƶ
	TIM_TimeBaseInitStructure.TIM_CounterMode=TIM_CounterMode_Up; //���ϼ���ģʽ
	TIM_TimeBaseInitStructure.TIM_ClockDivision=TIM_CKD_DIV1; 
	
	TIM_TimeBaseInit(TIM4,&TIM_TimeBaseInitStructure);//��ʼ��TIM
	
	TIM_ITConfig(TIM4,TIM_IT_Update,ENABLE); //����ʱ��3�����ж�
	TIM_Cmd(TIM4,DISABLE); //
	
	NVIC_InitStructure.NVIC_IRQChannel=TIM4_IRQn; //��ʱ��3�ж�
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=0x00; //��ռ���ȼ�1
	NVIC_InitStructure.NVIC_IRQChannelSubPriority=0x00; //�����ȼ�3
	NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE;
	NVIC_Init(&NVIC_InitStructure);
}
long int g_iMotor1PulseNum;
void TIM4_IRQHandler(void)
{
	if(TIM_GetITStatus(TIM4,TIM_IT_Update)==SET) //����ж�
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
	TIM_ClearITPendingBit(TIM4,TIM_IT_Update);  //����жϱ�־λ
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
	
	GPIO_PinAFConfig(GPIOF,GPIO_PinSource9,GPIO_AF_TIM14); //GPIOF9����Ϊ��ʱ��14
	GPIO_PinAFConfig(GPIOF,GPIO_PinSource8,GPIO_AF_TIM13); //GPIOF8����Ϊ��ʱ��13
	GPIO_PinAFConfig(GPIOF,GPIO_PinSource7,GPIO_AF_TIM11); //GPIOF7����Ϊ��ʱ��11
	GPIO_PinAFConfig(GPIOF,GPIO_PinSource6,GPIO_AF_TIM10); //GPIOF6����Ϊ��ʱ��10
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9|GPIO_Pin_8|GPIO_Pin_7|GPIO_Pin_6;           //GPIOF 9 8 7 6
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;        //���ù���
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	//�ٶ�100MHz
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;      //���츴�����
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;        //����
	GPIO_Init(GPIOF,&GPIO_InitStructure);       
	
	

RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM11,ENABLE);
RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM10,ENABLE);
RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM14,ENABLE);  	//TIM14ʱ��ʹ��    
RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM13,ENABLE);  	//TIM13ʱ��ʹ��   	
	

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
	MotorrTIM_Init(10-1,9000-1);//���ڵ����������
   // TIM_PWM_Init(TIM14,1000 - 1, 90 - 1);	//90M/90=1Mhz�ļ���Ƶ��,��װ��ֵ500������PWMƵ��Ϊ 1M/500=2Khz.  
   // TIM_PWM_Init(TIM13, 1000 - 1, 90 - 1);
   // TIM_PWM_Init(TIM11, 1000 - 1, 90 - 1);
   //TIM_PWM_Init(TIM10, 1000 - 1, 90 - 1);
	
}

//TIM PWM���ֳ�ʼ�� 
//PWM�����ʼ��
//arr���Զ���װֵ
//psc��ʱ��Ԥ��Ƶ��
void TIM_PWM_Init(TIM_TypeDef * TIMx,u32 arr,u32 psc)//�ú�����ʹ��
{		 					 
	//�˲������ֶ��޸�IO������
	

	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	TIM_OCInitTypeDef  TIM_OCInitStructure;
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM14,ENABLE);  	//TIM14ʱ��ʹ��    
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM13,ENABLE);  	//TIM13ʱ��ʹ��   
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM11, ENABLE);
	RCC_AHB2PeriphClockCmd(RCC_APB2Periph_TIM10, ENABLE);
	
//	TIM_DeInit(TIMx);  
	TIM_TimeBaseStructure.TIM_Period=arr;   //�Զ���װ��ֵ
	TIM_TimeBaseStructure.TIM_Prescaler=psc;  //��ʱ����Ƶ
	TIM_TimeBaseStructure.TIM_ClockDivision=TIM_CKD_DIV1; 
	TIM_TimeBaseStructure.TIM_CounterMode=TIM_CounterMode_Up; //���ϼ���ģʽ
	TIM_TimeBaseInit(TIMx,&TIM_TimeBaseStructure);//��ʼ����ʱ��14
	TIM_ITConfig(TIMx,TIM_IT_Update,ENABLE );//�����ж�ʹ��

	//��ʼ��TIM14 Channel1 PWMģʽ	 
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1; //ѡ��ʱ��ģʽ:TIM�����ȵ���ģʽ2
 	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; //�Ƚ����ʹ��
	TIM_OCInitStructure.TIM_Pulse=0;
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_Low; //�������:TIM����Ƚϼ��Ե�
	TIM_OC1Init(TIMx, &TIM_OCInitStructure);  //����Tָ���Ĳ�����ʼ������TIM1 4OC1

	TIM_OC1PreloadConfig(TIMx, TIM_OCPreload_Enable);  //ʹ��TIM14��CCR1�ϵ�Ԥװ�ؼĴ���
 
   TIM_ARRPreloadConfig(TIMx,ENABLE);//ARPEʹ�� 
	TIM_CtrlPWMOutputs(TIMx, ENABLE);
	TIM_Cmd(TIMx, ENABLE);  //ʹ��TIM14

}  
u32 MotorStartRun(u8 MotorNum,TIM_TypeDef* TIMx,u8 Orientation,int arr, long int Rotate)
{
	u32 m_TempData_32=0;
	TIM_SetCompare1(TIMx, arr );
	GPIO_ResetBits(s_paMotorGPIO_Pro[MotorNum][Motor_EN],s_aMotorGPIO_Pin[MotorNum][Motor_EN]);//���ʹ��
	if(Orientation==MotorFwd)
	{
	GPIO_ResetBits(s_paMotorGPIO_Pro[MotorNum][Motor_DIR],s_aMotorGPIO_Pin[MotorNum][Motor_DIR]);
	}
	else
	{
	GPIO_SetBits(s_paMotorGPIO_Pro[MotorNum][Motor_DIR],s_aMotorGPIO_Pin[MotorNum][Motor_DIR]);
	}
	TIM_Cmd(TIMx, ENABLE);  //ʹ��TIM
	TIM_Cmd(TIM4, ENABLE);  //ʹ��TIM
	g_iMotor1PulseNum = Rotate;
       //�˴���ȡŤ��
	
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

