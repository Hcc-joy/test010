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
  * @brief   输入浮空        
  * @note    
  */
 
//输入浮空

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
  * @brief   输入下拉     
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
  * @brief   输入上拉     
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
  * @brief   模拟输入        
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
  * @brief   推挽输出      
  * @note    
  */
void GPIO_Out_Push_Pull(GPIO_TypeDef * GPIO_Port, int16_t GPIO_Pin,u8 Flg)
{
	
  GPIO_InitTypeDef  GPIO_InitStructure;
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;//普通输出模式
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//推挽输出
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100MHz
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//上拉
  GPIO_Init(GPIO_Port, &GPIO_InitStructure);//初始化
	if(Flg==1)//输出初始化
	{
	GPIO_SetBits(GPIO_Port,GPIO_Pin);
	}
	else
	{
	GPIO_ResetBits(GPIO_Port,GPIO_Pin);
	}
}

/**
  * @brief   复用推挽输出      
  * @note    
  */
void GPIO_Out_AF_Push(GPIO_TypeDef * GPIO_Port, int16_t GPIO_Pin,u8 Flg)
{
	GPIO_InitTypeDef  GPIO_InitStructure;
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;//普通输出模式
  GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;//推挽输出
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100MHz
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//内部上拉
  GPIO_Init(GPIO_Port, &GPIO_InitStructure);//初始化
	if(Flg==1)//输出初始化
	{
	GPIO_SetBits(GPIO_Port,GPIO_Pin);
	}
	else
	{
	GPIO_ResetBits(GPIO_Port,GPIO_Pin);
	}
}

/**
  * @brief   中断分组配置
  * @note    IRQn:中断通道    Preemption：抢占优先级  Sub：响应优先级
  */
void NVIC_Init_Config(uint8_t IRQn,uint8_t Preemption,uint8_t Sub)
{
	 NVIC_InitTypeDef NVIC_InitStructure;
   NVIC_InitStructure.NVIC_IRQChannel =IRQn;                        //IRQ通道
	 NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=Preemption ;//抢占优先级
	 NVIC_InitStructure.NVIC_IRQChannelSubPriority = Sub;		          //子优先级
	 NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			            
	 NVIC_Init(&NVIC_InitStructure);	
}

/**
  * @brief   串口初始化配置
  * @note    USARTx：串口  bound：波特率  mode：工作模式
  */
void 	USART_Init_Config(USART_TypeDef* USARTx,uint32_t bound,uint16_t mode)
{
       USART_InitTypeDef USART_InitStructure;
		USART_InitStructure.USART_BaudRate = bound;//串口波特率
		USART_InitStructure.USART_WordLength = USART_WordLength_8b;//字长为8位数据格式
		USART_InitStructure.USART_StopBits = USART_StopBits_1;//一个停止位
		USART_InitStructure.USART_Parity = USART_Parity_No;//无奇偶校验位
		USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//无硬件数据流控制
		USART_InitStructure.USART_Mode = mode;	  //串口模式
		USART_Init(USARTx, &USART_InitStructure); //初始化串口1	
}


 








