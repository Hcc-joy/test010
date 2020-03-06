#include "sys.h"
#include "usart.h"	
#include "stm32f4xx_dma.h"
#include "string.h"	
////////////////////////////////////////////////////////////////////////////////// 	 
//如果使用ucos,则包括下面的头文件即可.
#if SYSTEM_SUPPORT_OS
#include "includes.h"					//ucos 使用	  
#endif
//////////////////////////////////////////////////////////////////////////////////	 

//////////////////////////////////////////////////////////////////
//加入以下代码,支持printf函数,而不需要选择use MicroLIB	  
#if 1
#pragma import(__use_no_semihosting)             
//标准库需要的支持函数                 
struct __FILE 
{ 
	int handle; 
}; 

FILE __stdout;       
//定义_sys_exit()以避免使用半主机模式    
_sys_exit(int x) 
{ 
	x = x; 
} 
//重定义fputc函数 
int fputc(int ch, FILE *f)
{ 	
	while((USART1->SR&0X40)==0);//循环发送,直到发送完毕   
	USART1->DR = (u8) ch;      
	return ch;
}
#endif
 
#if EN_USART1_RX   //如果使能了接收
//串口1中断服务程序
//注意,读取USARTx->SR能避免莫名其妙的错误   	
u8 USART_RX_BUF[USART_REC_LEN];     //接收缓冲,最大USART_REC_LEN个字节.
//接收状态
//bit15，	接收完成标志
//bit14，	接收到0x0d
//bit13~0，	接收到的有效字节数目
u16 USART_RX_STA=0;       //接收状态标记	
u8 g_aUart1Buffer[Uart1RxLen];
u8 g_aUsart1RxData[Uart1RxLen];
u8 g_cUart1RxLen=0;


//初始化IO 串口1 
//bound:波特率
void USART1_init(u32 bound)
	{
   //GPIO端口设置
    GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA,ENABLE); //使能GPIOA时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1,ENABLE);//使能USART1时钟
 
	//串口1对应引脚复用映射
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource9,GPIO_AF_USART1); //GPIOA9复用为USART1
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource10,GPIO_AF_USART1); //GPIOA10复用为USART1
	
	//USART1端口配置
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9 | GPIO_Pin_10; //GPIOA9与GPIOA10
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;//复用功能
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	//速度50MHz
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP; //推挽复用输出
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP; //上拉
	GPIO_Init(GPIOA,&GPIO_InitStructure); //初始化PA9，PA10

   //USART1 初始化设置
	USART_InitStructure.USART_BaudRate = bound;//波特率设置
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//字长为8位数据格式
	USART_InitStructure.USART_StopBits = USART_StopBits_1;//一个停止位
	USART_InitStructure.USART_Parity = USART_Parity_No;//无奇偶校验位
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//无硬件数据流控制
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//收发模式
    USART_Init(USART1, &USART_InitStructure); //初始化串口1
    USART_Cmd(USART1, ENABLE);  //使能串口1 
	
	//USART_ClearFlag(USART1, USART_FLAG_TC);
	
#if EN_USART1_RX	
	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);//开启相关中断
	//Usart1 NVIC 配置
    NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;//串口1中断通道
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=3;//抢占优先级3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority =3;		//子优先级3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQ通道使能
	NVIC_Init(&NVIC_InitStructure);	//根据指定的参数初始化VIC寄存器、

#endif

USART_DMA_Config();
	USART_ITConfig(USART1, USART_IT_RXNE, DISABLE);   //¹Ø±Õ½ÓÊÕÖÐ¶Ï
	USART_ClearITPendingBit(USART1, USART_IT_IDLE|USART_IT_RXNE);    //Çå³ý¿ÕÏÐÖÐ¶Ï±êÖ¾ 
	USART_ITConfig(USART1, USART_IT_IDLE, ENABLE);   //¿ªÆô¿ÕÏÐÖÐ¶Ï
	USART_DMACmd(USART1,USART_DMAReq_Rx,ENABLE);   //Ê¹ÄÜ´®¿Ú2 DMA½ÓÊÕ

}

void USART2_init(u32 bound)
{
	//GPIO端口设置
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE); //
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);//

	//串口1对应引脚复用映射
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource2, GPIO_AF_USART2); //
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource3, GPIO_AF_USART2); //

	//USART1端口配置
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2 | GPIO_Pin_3; 
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;//复用功能
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	//速度50MHz
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP; //推挽复用输出
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP; //上拉
	GPIO_Init(GPIOA, &GPIO_InitStructure); 

   //USART2 初始化设置
	USART_InitStructure.USART_BaudRate = bound;//波特率设置
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//字长为8位数据格式
	USART_InitStructure.USART_StopBits = USART_StopBits_1;//一个停止位
	USART_InitStructure.USART_Parity = USART_Parity_No;//无奇偶校验位
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//无硬件数据流控制
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//收发模式
	USART_Init(USART2, &USART_InitStructure); //初始化串口2
	USART_Cmd(USART2, ENABLE);  //使能串口2
	

}
/************************************************************************/
/* Function Name:	USART_DMA_Config.						            */
/* Purpose:			初始化USART DMA设置									*/
/*																		*/
/* Resources:		USART1 Rx DMA			 							*/
/* Params:			None.												*/
/* Return:			None.												*/
/* Limitation:		None.												*/
/************************************************************************/
void USART_DMA_Config(void)
{
  DMA_InitTypeDef DMA_InitStructure;
	/*开启DMA时钟*/
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA2, ENABLE);  
  DMA_DeInit(DMA2_Stream5);
  DMA_InitStructure.DMA_Channel = DMA_Channel_4;
  DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)(&USART1->DR);         //外设地址      
  DMA_InitStructure.DMA_Memory0BaseAddr = (uint32_t)g_aUart1Buffer;                 //内存地址      
  DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralToMemory;                     //dma传输方向单向      
  DMA_InitStructure.DMA_BufferSize = Uart1RxLen;                            //设置DMA在传输时缓冲区的长度     
  DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;            //设置DMA的外设递增模式，一个外设        
  DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;                     //设置DMA的内存递增模式      
  DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;     //外设数据字长         
  DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;             //内存数据字长      
  DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;                               //设置DMA的传输模式      
  DMA_InitStructure.DMA_Priority = DMA_Priority_VeryHigh;                     //设置DMA的优先级别  

  DMA_InitStructure.DMA_FIFOMode = DMA_FIFOMode_Disable;
  DMA_InitStructure.DMA_FIFOThreshold = DMA_FIFOThreshold_HalfFull;
  DMA_InitStructure.DMA_MemoryBurst = DMA_MemoryBurst_Single;
  DMA_InitStructure.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;

  //    DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;                                //设置DMA的2个memory中的变量互相访问
  DMA_Init(DMA2_Stream5, &DMA_InitStructure);
  DMA_Cmd(DMA2_Stream5, ENABLE);                                               //使能数据流1 通道5

}
/************************************************************************/
/* Function Name:	UsartSendStartCom.						            */
/* Purpose:			USART2 发送开始启动命令								*/
/*																		*/
/* Resources:		USART2 Tx				 							*/
/* Params:			None.												*/
/* Return:			None.												*/
/* Limitation:		None.												*/
/************************************************************************/
void UsartSendStartCom(void)                	//串口2中断服务程序
{
	int length = 6, i = 0;
	u8  m_aStartData[6] = {0xF5,0x21,0x40,0x02,0x01,0x03};
	while (length--)
	{
		while (USART_GetFlagStatus(USART2, USART_FLAG_TC) == RESET);
		USART_SendData(USART2, m_aStartData[i]);
		i++;
	}
} 
/************************************************************************/
/* Function Name:	USART1_IRQHandler.						            */
/* Purpose:			USART1 Receivce	Interrupt							*/
/*																		*/
/* Resources:		USART1 Rx				 							*/
/* Params:			None.												*/
/* Return:			None.												*/
/* Limitation:		None.												*/
/************************************************************************/
void USART1_IRQHandler(void)                	//串口1中断服务程序
{
	u16 len = 0;
	if (USART_GetITStatus(USART1, USART_IT_IDLE) != RESET)
	{
        
		USART_ClearITPendingBit(USART1, USART_IT_IDLE);         //清除中断标志
		USART_ReceiveData(USART1);//读取数据 注意：这句必须要，否则不能够清除中断标志位。
		DMA_Cmd(DMA2_Stream5, DISABLE);
		len=DMA_GetCurrDataCounter(DMA2_Stream5);
		g_cUart1RxLen= Uart1RxLen - len;
		DMA_SetCurrDataCounter(DMA2_Stream5, Uart1RxLen);
		DMA_ClearFlag(DMA2_Stream5,DMA_FLAG_TCIF5 | DMA_FLAG_FEIF5 | DMA_FLAG_DMEIF5 | DMA_FLAG_TEIF5 | DMA_FLAG_HTIF5);//清除DMA2_Steam5传输完成标志  
		DMA_Cmd(DMA2_Stream5, ENABLE);
		if (Usart1DataCheck(g_aUart1Buffer, g_cUart1RxLen) == SET)
		{
			memcpy(g_aUsart1RxData, g_aUart1Buffer, g_cUart1RxLen);
		}
		memset(g_aUart1Buffer, 0x00, sizeof(g_aUart1Buffer));//
	}
} 
#endif	
/************************************************************************/
/* Function Name:	usMBCRC16								            */
/* Purpose:			CRC16 formula mode									*/
/*																		*/
/* Resources:		Modbus CRC16			 							*/
/* Params:			None.												*/
/* Return:			None.												*/
/* Limitation:		None.												*/
/************************************************************************/
// CRC16 Modbus
// CRC16 Modbus


u16 usMBCRC16(u8* pucFrame, u8 usLen)
{

	u8 ucCRCHi = 0xFF;
	u8 ucCRCLo = 0xFF;
	int iIndex = 0;
	pucFrame = pucFrame + 4;
	while (usLen--)
	{
		iIndex = ucCRCLo ^ *(pucFrame++);
		ucCRCLo = (char)(ucCRCHi ^ aucCRCHi[iIndex]);
		ucCRCHi = aucCRCLo[iIndex];
	}
	return (int)(ucCRCHi << 8 | ucCRCLo);
}

/************************************************************************/
/* Function Name:	Usart1DataCheck							            */
/* Purpose:			USART1 Receivce Data Validation						*/
/*																		*/
/* Resources:		Modbus CRC16			 							*/
/* Params:			None.												*/
/* Return:			0 or 1												*/
/* Limitation:		None.												*/
/************************************************************************/
u8  Usart1DataCheck(u8* pData, u8 lenght)
{
	//u16 CRC16 = 0xFFFF;
	if (*pData != 0xF5)
		return 0;
	if (lenght < 5)
		return 0;
	if (lenght != (*(pData + 3) + 4))
		return 0;
	if ((*(pData + 1)<< 8|*(pData + 2)) == usMBCRC16(pData, *(pData + 3)))
		return 1;
	return 0;
}

/************************************************************************/
/* Function Name:	UsartSendData							            */
/* Purpose:			USART1 Tx					*/
/*																		*/
/* Resources:		None		 							*/
/* Params:			None.												*/
/* Return:			None											*/
/* Limitation:		None.												*/
/************************************************************************/
void Usart_SendData(u8* nDATA, u8 length)
{
	while (length--)
	{
		while (USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET);	
		USART_SendData(USART1, *nDATA);
		nDATA++;
	}
}
/************************************************************************/
/* Function Name:	Ack_SendData							            */
/* Purpose:			发送有效数据到上位机					*/
/*																		*/
/* Resources:		None		 							*/
/* Params:			None.												*/
/* Return:			None											*/
/* Limitation:		None.												*/
/************************************************************************/
void Ack_SendData(u8* pData, u8 length)
{
	u16 m_CRC16Vuale;
	*pData = TxFrameHeader;
	*(pData + 3)=length;
	m_CRC16Vuale = usMBCRC16(pData, *(pData + 3));
	*(pData + 1) = m_CRC16Vuale >>8;
	*(pData + 2) = m_CRC16Vuale;
	Usart_SendData(pData, length+4);
}

