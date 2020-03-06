#include "GPIOlib.h"


void GPIO_PeriphClock(void)
{

	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE); 
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE); 
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE); 
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE); 
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE, ENABLE); 
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOF, ENABLE); 
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOG, ENABLE); 
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOH, ENABLE); 
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOI, ENABLE); 
}



/**
  * @brief   ���븡��        
  * @note    
  */
 
//���븡��

void GPIO_In_Float(GPIO_TypeDef * GPIO_Port, int16_t GPIO_Pin)
{
    GPIO_InitTypeDef  GPIO_InitStructure;	
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin;
	  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;//
	  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;//
    GPIO_Init(GPIO_Port, &GPIO_InitStructure);
}

/**
  * @brief   ��������     
  * @note    
  */
void GPIO_In_Down(GPIO_TypeDef * GPIO_Port, int16_t GPIO_Pin)
{
    GPIO_InitTypeDef  GPIO_InitStructure;	
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin;
	  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;//
	  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN;//
    GPIO_Init(GPIO_Port, &GPIO_InitStructure);
}

/**
  * @brief   ��������     
  * @note    
  */
void GPIO_In_Up(GPIO_TypeDef * GPIO_Port, int16_t GPIO_Pin)
{
    GPIO_InitTypeDef  GPIO_InitStructure;	
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin;
	  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;//
	  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//
    GPIO_Init(GPIO_Port, &GPIO_InitStructure);
}

/**
  * @brief   ģ������        
  * @note    
  */
void GPIO_In_Mode(GPIO_TypeDef * GPIO_Port, int16_t GPIO_Pin)
{
    GPIO_InitTypeDef  GPIO_InitStructure;	
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin;
	  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AN;//
	  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;//
    GPIO_Init(GPIO_Port, &GPIO_InitStructure);
}

/**
  * @brief   �������      
  * @note    
  */
void GPIO_Out_Push_Pull(GPIO_TypeDef * GPIO_Port, int16_t GPIO_Pin,u8 Flg)
{
	
  GPIO_InitTypeDef  GPIO_InitStructure;
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;//��ͨ���ģʽ
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//�������
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100MHz
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//����
  GPIO_Init(GPIO_Port, &GPIO_InitStructure);//��ʼ��
	if(Flg==1)//�����ʼ��
	{
	GPIO_SetBits(GPIO_Port,GPIO_Pin);
	}
	else
	{
	GPIO_ResetBits(GPIO_Port,GPIO_Pin);
	}
}

/**
  * @brief   �����������      
  * @note    
  */
void GPIO_Out_AF_Push(GPIO_TypeDef * GPIO_Port, int16_t GPIO_Pin,u8 Flg)
{
	GPIO_InitTypeDef  GPIO_InitStructure;
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;//��ͨ���ģʽ
  GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;//�������
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100MHz
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//�ڲ�����
  GPIO_Init(GPIO_Port, &GPIO_InitStructure);//��ʼ��
	if(Flg==1)//�����ʼ��
	{
	GPIO_SetBits(GPIO_Port,GPIO_Pin);
	}
	else
	{
	GPIO_ResetBits(GPIO_Port,GPIO_Pin);
	}
}

/**
  * @brief   �жϷ�������
  * @note    IRQn:�ж�ͨ��    Preemption����ռ���ȼ�  Sub����Ӧ���ȼ�
  */
void NVIC_Init_Config(uint8_t IRQn,uint8_t Preemption,uint8_t Sub)
{
	 NVIC_InitTypeDef NVIC_InitStructure;
   NVIC_InitStructure.NVIC_IRQChannel =IRQn;                        //IRQͨ��
	 NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=Preemption ;//��ռ���ȼ�
	 NVIC_InitStructure.NVIC_IRQChannelSubPriority = Sub;		          //�����ȼ�
	 NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			            
	 NVIC_Init(&NVIC_InitStructure);	
}

/**
  * @brief   ���ڳ�ʼ������
  * @note    USARTx������  bound��������  mode������ģʽ
  */
void 	USART_Init_Config(USART_TypeDef* USARTx,uint32_t bound,uint16_t mode)
{
       USART_InitTypeDef USART_InitStructure;
		USART_InitStructure.USART_BaudRate = bound;//���ڲ�����
		USART_InitStructure.USART_WordLength = USART_WordLength_8b;//�ֳ�Ϊ8λ���ݸ�ʽ
		USART_InitStructure.USART_StopBits = USART_StopBits_1;//һ��ֹͣλ
		USART_InitStructure.USART_Parity = USART_Parity_No;//����żУ��λ
		USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//��Ӳ������������
		USART_InitStructure.USART_Mode = mode;	  //����ģʽ
		USART_Init(USARTx, &USART_InitStructure); //��ʼ������1	
}


 








