/*
 * TM7707.c
 *
 *  Created on: 2019年7月25日
 *      Author: Hcc
 */

#include "sys.h"
#include "spi.h"
#include "TM7707.h"
static  GPIO_TypeDef* s_paGPIO_Pro[8][4] =
{
	{GPIOH,GPIOH,GPIOH,GPIOH}, //TS1     u27
	{GPIOA,GPIOA,GPIOA,GPIOA}, //TS2&TS3 u1
	{GPIOC,GPIOC,GPIOB,GPIOB}, //TS4&TS5 u6
	{GPIOB,GPIOF,GPIOF,GPIOF}, //TS6&TS7 u5
	{GPIOF,GPIOF,GPIOG,GPIOG}, //TS8&TS9 u30
	{GPIOE,GPIOE,GPIOE,GPIOE}, //TS10&TS11 u2
	{GPIOE,GPIOE,GPIOE,GPIOE}, //LDS4&LDS3 u31
	{GPIOH,GPIOE,GPIOB,GPIOB}  //LDS2&LDS1 u26
};

static  u16 s_aGPIO_Pin[8][4] =
{
	{GPIO_Pin_2,GPIO_Pin_3,GPIO_Pin_4,GPIO_Pin_5},
	{GPIO_Pin_4,GPIO_Pin_5,GPIO_Pin_6,GPIO_Pin_7},
	{GPIO_Pin_4,GPIO_Pin_5,GPIO_Pin_0,GPIO_Pin_1},
	{GPIO_Pin_2,GPIO_Pin_11,GPIO_Pin_12,GPIO_Pin_13},
	{GPIO_Pin_14,GPIO_Pin_15,GPIO_Pin_0,GPIO_Pin_1},
	{GPIO_Pin_7,GPIO_Pin_8,GPIO_Pin_9,GPIO_Pin_10},
	{GPIO_Pin_14,GPIO_Pin_11,GPIO_Pin_12,GPIO_Pin_13 },
	{GPIO_Pin_6,GPIO_Pin_15,GPIO_Pin_10,GPIO_Pin_11 }
};

void TM7707_GPIOInit(void)
{
	u8 s_i;
	for (s_i = 0; s_i < TM7707_U26 + 1; s_i++)
	{
		SpiGpioInit(s_i);
	}
}

/*TM7707需要检测等待可读取信号*/
int g_Time_OutCnt;
void TM7707_WaitForDRDY(u8 Num)
{
	g_Time_OutCnt = 5;
	while (GPIO_ReadInputDataBit(s_paGPIO_Pro[Num][Drdy], s_aGPIO_Pin[Num][Drdy]) != 0)
	{
		if (g_Time_OutCnt == 0)
		{
			break;
		}
	}
}
/* TM7707初始化
 *
 *
 * */
void TM7707_Init(void)
{
	 u8 s_i;
     TM7707_GPIOInit();
	 delay_ms(100);
	 TM7707_SynchronizeDeviceClock();
	 for (s_i = 0; s_i < TM7707_U2 + 1; s_i++)
	 {
		 TM7707_TransferByte(REG_CLOCK | WRITE | CH_1, s_i); // Write communication register to set clook register.
		 TM7707_TransferByte(CLKDIS_0 | CLK_2_4576M | FS_500HZ, s_i); // External clock, frequency is 4.9152 MHz and the sampling frequency is 500Hz.
		 TM7707_TransferByte(REG_CLOCK | WRITE | CH_2, s_i); // Write communication register to set clook register.
		 TM7707_TransferByte(CLKDIS_0 | CLK_2_4576M | FS_500HZ, s_i); // External clock, frequency is 4.9152 MHz and the sampling frequency is 500Hz.
		 TM7707_SelfCalibration(CH_1, GAIN_1,UNIPOLAR,s_i);//
	 }
	 for (s_i= TM7707_U31; s_i < TM7707_U26 + 1; s_i++)
	 {
		 TM7707_TransferByte(REG_CLOCK | WRITE | CH_1, s_i); // Write communication register to set clook register.
		 TM7707_TransferByte(CLKDIS_0 | CLK_2_4576M | FS_500HZ, s_i); // External clock, frequency is 4.9152 MHz and the sampling frequency is 500Hz.
		 TM7707_TransferByte(REG_CLOCK | WRITE | CH_2, s_i); // Write communication register to set clook register.
		 TM7707_TransferByte(CLKDIS_0 | CLK_2_4576M | FS_500HZ, s_i); // External clock, frequency is 4.9152 MHz and the sampling frequency is 500Hz.
		 TM7707_SelfCalibration(CH_1, GAIN_1, UNIPOLAR, s_i);//
	 }
}
/////////////////////////////////

/************************************************************************/
/* Function Name:	TM7707_SelfCalibration.								*/
/* Purpose:			Internal channel self-calibration.					*/
/* Resources:		SPI controler and GPIOs.							*/
/* Params:																*/
/*	@uiChannelIndex:	Channel index.									*/
/* Return:			None.												*/
/* Limitation:		None.												*/
/************************************************************************/
void TM7707_SelfCalibration(uint8_t	uiChannelIndex, uint8_t uiGain,uint8_t POLAR, u8 Num)
{
	 uiChannelIndex = uiChannelIndex % CH_MAX;
	//Write communication register to set setup register.
	 TM7707_TransferByte(REG_SETUP | WRITE | uiChannelIndex, Num);
	//Write configuration register, start self-calibration.
	 TM7707_TransferByte(MD_CAL_SELF | uiGain | POLAR | BUF_NO | FSYNC_0, Num);
	// Wait for operation to complete(ablut 180ms).
	 TM7707_WaitForDRDY(Num);
} 


/************************************************************************/
/* Function Name:	TM7707_SynchronizeDeviceClock.						*/
/* Purpose:			Synchronize TM7707 device clock, keep SCLK pin high	*/
/*					at least 32 cycles.									*/
/* Resources:		SPI controler and GPIOs.							*/
/* Params:			None.												*/
/* Return:			None.												*/
/* Limitation:		None.												*/
/************************************************************************/
void TM7707_SynchronizeDeviceClock(void)
{
	uint8_t i,j;
	// Keep SCLK pin high at 40 cycles.
	for(i=0;i<TM7707_U26+1;i++)
	{
	for(j=0; j<5; j++)
	{
		TM7707_TransferByte(0xFF,i);
	}
 }
}
/*读取TM7707 24位AD值 */
uint32_t TM7707_ReadADCValue(uint8_t uiChannelIndex,u8 Num)
{
  uint32_t rtnADValue = 0;
  rtnADValue = 0;
  TM7707_TransferByte(REG_DATA | READ | uiChannelIndex,Num);
  TM7707_WaitForDRDY(Num);
	rtnADValue |= SPI_ReadByte(Num);
	rtnADValue <<= 8;
  rtnADValue |= SPI_ReadByte(Num);
	rtnADValue <<= 8;
	rtnADValue |= SPI_ReadByte(Num);
	return rtnADValue;

}

uint8_t TM7707_TransferByte(uint8_t uiTxData,u8 Num)
{
	//u8 s_RxData;
	SPI_WriteByte(uiTxData, Num);
	//delay_ms(10);
	//s_RxData=SPI_ReadByte(Num);
	return 0;
}
/*********************************************************************************
  *Function:  //函数名称 位移传感器测试函数
  * Description：  //函数功能，性能等的描述  使用 TM7707测试
  *Calls:  //被本函数调用的函数清单
  *Called By:  //调用本函数的清单
  *Input:  //输入参数的说明，包括每个参数的作用、取值说明及参数间关系   TM7707序号、通道号
  *Output:  //输出参数的说明
  *Return:  //函数返回值的说明  位移传感器测试的高度
  *Others:  //其他说明
**********************************************************************************/
double DtshBoltHeight(u8 TM7707_Num, u8 TM7707_Channel)
{
	u32 m_ADC_Vaule=0;
	double m_BoltHeight;
	m_ADC_Vaule =TM7707_ReadADCValue(TM7707_Channel, TM7707_Num);
	m_BoltHeight =2*2.5* m_ADC_Vaule /16777216/0.071;
	return m_BoltHeight;
}

/*********************************************************************************
  *Function:  //函数名称 扭力传感器测试函数
  * Description：  //函数功能，性能等的描述  使用 TM7707测试
  *Calls:  //被本函数调用的函数清单
  *Called By:  //调用本函数的清单
  *Input:  //输入参数的说明，包括每个参数的作用、取值说明及参数间关系   TM7707序号、通道号
  *Output:  //输出参数的说明
  *Return:  //函数返回值的说明  扭力传感器测试高度
  *Others:  //该函数算法还没有验证
**********************************************************************************/
double DtshGripiTorsion(u8 TM7707_Num, u8 Channel)
{
	u32 m_ADC_Vaule;
	double m_GripTorsion;
	m_ADC_Vaule = TM7707_ReadADCValue(Channel, TM7707_Num);
	m_GripTorsion = 2 * 2.5 * m_ADC_Vaule / 16777216 / 0.071;
	return m_GripTorsion;
}
