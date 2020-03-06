/*
 * spi.c
 *
 *  Created on: 2019年7月25日
 *      Author: Hcc
 */
#include "spi.h"
#include "GPIOlib.h"
#include "TM7707.h"
//该底层驱动使用软件SPI
#define SCLK_SET(Pro,Pin)  GPIO_SetBits(Pro,Pin)
#define SCLK_CLR(Pro,Pin)  GPIO_ResetBits(Pro,Pin)
#define MOSI_SET(Pro, Pin)  GPIO_SetBits(Pro, Pin)
#define MOSI_CLR(Pro,Pin)  GPIO_ResetBits(Pro,Pin)
#define MISO_Read(Pro,Pin) GPIO_ReadInputDataBit(Pro,Pin)

static  GPIO_TypeDef* s_paGPIO_Pro[8][4] =
{
	{GPIOH,GPIOH,GPIOH,GPIOH}, //TS1 u27
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

void SpiGpioInit(u8 Num)
{
	 
	   GPIO_Out_Push_Pull(s_paGPIO_Pro[Num][MOSI], s_aGPIO_Pin[Num][MOSI], 1);  //mosi
     GPIO_In_Up(s_paGPIO_Pro[Num][MISO], s_aGPIO_Pin[Num][MISO]);  //miso
  	 GPIO_Out_Push_Pull(s_paGPIO_Pro[Num][SCLK], s_aGPIO_Pin[Num][SCLK], 1); //sclk
  	 GPIO_In_Up(s_paGPIO_Pro[Num][Drdy], s_aGPIO_Pin[Num][Drdy]);  //Drdy
	
}
void SPI_delay(void)
{
	uint16_t	i;
	for(i=0; i<10; i++)
	{
		__ASM("nop");
	}
}





//TxData:要写入的字节
//SPI 写一个字节
void SPI_WriteByte(u8 s_SendByte ,u8 Num)
{	

	u8 s_i = 0x80, s_j = 0x08;
	 SCLK_SET(s_paGPIO_Pro[Num][SCLK], s_aGPIO_Pin[Num][SCLK]);
	for (; s_j > 0; s_j--) 
	{
		SCLK_CLR(s_paGPIO_Pro[Num][SCLK], s_aGPIO_Pin[Num][SCLK]);
		SPI_delay();
		if (s_SendByte & s_i)
			MOSI_SET(s_paGPIO_Pro[Num][MOSI], s_aGPIO_Pin[Num][MOSI]);
		else
			MOSI_CLR(s_paGPIO_Pro[Num][MOSI], s_aGPIO_Pin[Num][MOSI]);
		SPI_delay();
		SCLK_SET(s_paGPIO_Pro[Num][SCLK], s_aGPIO_Pin[Num][SCLK]);
		s_SendByte <<= 1;
		SPI_delay();
	}
	SCLK_SET(s_paGPIO_Pro[Num][SCLK], s_aGPIO_Pin[Num][SCLK]);
}

u8 SPI_ReadByte(u8 Num)
{
	
	u8 s_Byte = 0x00, s_j = 0x08;
	SCLK_SET(s_paGPIO_Pro[Num][SCLK], s_aGPIO_Pin[Num][SCLK]);
	for (; s_j > 0; s_j--)
	{
		SCLK_CLR(s_paGPIO_Pro[Num][SCLK], s_aGPIO_Pin[Num][SCLK]);
		s_Byte = s_Byte << 1;
		SPI_delay();
		if (MISO_Read(s_paGPIO_Pro[Num][MISO], s_aGPIO_Pin[Num][MISO])==RESET)
			s_Byte = s_Byte | 0x00;
		else
			s_Byte = s_Byte | 0x01;
		SCLK_SET(s_paGPIO_Pro[Num][SCLK], s_aGPIO_Pin[Num][SCLK]);
		SPI_delay();
	}
 SCLK_SET(s_paGPIO_Pro[Num][SCLK], s_aGPIO_Pin[Num][SCLK]);
	return s_Byte;
}
