/*======================================================================*/
/*= TM7707 Driver library.												*/
/*======================================================================*/
#ifndef _INCLUDE_TM7707_H_
#define _INCLUDE_TM7707_H_
#include "sys.h"
#include "delay.h"
#define TM7707_GPIO		GPIO_STRUCT

#define TM7707_CS_ENABLE()			GPIO_ResetBits(GPIOB,GPIO_Pin_12);
#define TM7707_CS_DISABLE()			GPIO_SetBits(GPIOB,GPIO_Pin_12);

#define TS1 0
#define TS2 1
#define TS3 2
#define TS4 3
#define TS5 4
#define TS6 5
#define TS7 6
#define TS8 7
#define TS9 8
#define TS10 9
#define TS11 10
#define LDS1 11
#define LDS2 12
#define LDS3 13
#define LDS4 14


#define TM7707_U27  0
#define TM7707_U1   1
#define TM7707_U6   2
#define TM7707_U5   3
#define TM7707_U30  4
#define TM7707_U2   5
#define TM7707_U31  6 //位移1
#define TM7707_U26  7

#define SCLK  0
#define MOSI 1
#define MISO 2
#define Drdy 3



// Communication register
enum
{
	// Destination register
	REG_COMM		= 0x00,	// Communication register.
	REG_SETUP		= 0x10,	// Setting register.
	REG_CLOCK		= 0x20,	// Clock register.
	REG_DATA		= 0x30,	// Data register.
	REG_ZERO_CH1	= 0x60,	/* 偏移寄存器	*/
	REG_FULL_CH1	= 0x70,	/* 满量程寄存器	*/
	// Register 
	WRITE			= 0x00,	// Write register
	READ			= 0x08,	// Read register
	// Channel index
	CH_1			= 0x00,	/* AIN1+  AIN1-		*/
	CH_2			= 0x01,	/* AIN2+  AIN2-		*/
	CH_3			= 0x02,	/* AIN1-  AIN1-		*/
	CH_4			= 0x03,	/* AIN1-  AIN2-		*/
	CH_MAX			= 0x04,	// Cannot write to device register.
};

// Setting register
enum
{
	MD_NORMAL		= (0 << 6),	/* 正常模式		*/
	MD_CAL_SELF		= (1 << 6),	/* 自校准模式	*/
	MD_CAL_ZERO		= (2 << 6),	/* 校准0刻度模式	*/
	MD_CAL_FULL		= (3 << 6),	/* 校准满刻度模式*/

	GAIN_1			= (0 << 3),	/* 增益			*/
	GAIN_2			= (1 << 3),	/* 增益			*/
	GAIN_4			= (2 << 3),	/* 增益			*/
	GAIN_8			= (3 << 3),	/* 增益			*/
	GAIN_16			= (4 << 3),	/* 增益			*/
	GAIN_32			= (5 << 3),	/* 增益			*/
	GAIN_64			= (6 << 3),	/* 增益			*/
	GAIN_128		= (7 << 3),	/* 增益			*/

	/* 无论双极性还是单极性都不改变任何输入信号的状态，它只改变输出数据的代码和转换函数上的校准点 */
	BIPOLAR			= (0 << 2),	/* 双极性输入 */
	UNIPOLAR		= (1 << 2),	/* 单极性输入 */

	BUF_NO			= (0 << 1),	/* 输入无缓冲（内部缓冲器不启用) */
	BUF_EN			= (1 << 1),	/* 输入有缓冲 (启用内部缓冲器) */

	FSYNC_0			= 0,
	FSYNC_1			= 1			/* 不启用 */
};

// Clock register define.
enum
{
	CLKDIS_0	= 0x00,		/* 时钟输出使能 （当外接晶振时，必须使能才能振荡） */
	CLKDIS_1	= 0x10,		/* 时钟禁止 （当外部提供时钟时，设置该位可以禁止MCK_OUT引脚输出时钟以省电 */

	/*
		2.4576MHz（CLKDIV=0 ）或为 4.9152MHz （CLKDIV=1 ），CLK 应置 “0”。
		1MHz （CLKDIV=0 ）或 2MHz   （CLKDIV=1 ），CLK 该位应置  “1”
	*/
	CLK_4_9152M = 0x08,
	CLK_2_4576M = 0x00,
	CLK_1M 		= 0x04,
	CLK_2M 		= 0x0C,

	FS_50HZ		= 0x00,
	FS_60HZ		= 0x01,
	FS_250HZ	= 0x02,
	FS_500HZ	= 0x04,

	ZERO_0		= 0x00,
	ZERO_1		= 0x80
};




// Function declare
void TM7707_Init(void);
uint8_t TM7707_TransferByte(uint8_t uiTxData, u8 Num);
void	TM7707_SynchronizeDeviceClock(void);
uint32_t  TM7707_ReadADCValue(uint8_t uiChannelIndex,u8 Num);
void TM7707_SelfCalibration(uint8_t	uiChannelIndex, uint8_t uiGain, uint8_t POLAR, u8 Num);
#endif
