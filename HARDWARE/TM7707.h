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
#define TM7707_U31  6 //λ��1
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
	REG_ZERO_CH1	= 0x60,	/* ƫ�ƼĴ���	*/
	REG_FULL_CH1	= 0x70,	/* �����̼Ĵ���	*/
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
	MD_NORMAL		= (0 << 6),	/* ����ģʽ		*/
	MD_CAL_SELF		= (1 << 6),	/* ��У׼ģʽ	*/
	MD_CAL_ZERO		= (2 << 6),	/* У׼0�̶�ģʽ	*/
	MD_CAL_FULL		= (3 << 6),	/* У׼���̶�ģʽ*/

	GAIN_1			= (0 << 3),	/* ����			*/
	GAIN_2			= (1 << 3),	/* ����			*/
	GAIN_4			= (2 << 3),	/* ����			*/
	GAIN_8			= (3 << 3),	/* ����			*/
	GAIN_16			= (4 << 3),	/* ����			*/
	GAIN_32			= (5 << 3),	/* ����			*/
	GAIN_64			= (6 << 3),	/* ����			*/
	GAIN_128		= (7 << 3),	/* ����			*/

	/* ����˫���Ի��ǵ����Զ����ı��κ������źŵ�״̬����ֻ�ı�������ݵĴ����ת�������ϵ�У׼�� */
	BIPOLAR			= (0 << 2),	/* ˫�������� */
	UNIPOLAR		= (1 << 2),	/* ���������� */

	BUF_NO			= (0 << 1),	/* �����޻��壨�ڲ�������������) */
	BUF_EN			= (1 << 1),	/* �����л��� (�����ڲ�������) */

	FSYNC_0			= 0,
	FSYNC_1			= 1			/* ������ */
};

// Clock register define.
enum
{
	CLKDIS_0	= 0x00,		/* ʱ�����ʹ�� ������Ӿ���ʱ������ʹ�ܲ����񵴣� */
	CLKDIS_1	= 0x10,		/* ʱ�ӽ�ֹ �����ⲿ�ṩʱ��ʱ�����ø�λ���Խ�ֹMCK_OUT�������ʱ����ʡ�� */

	/*
		2.4576MHz��CLKDIV=0 ����Ϊ 4.9152MHz ��CLKDIV=1 ����CLK Ӧ�� ��0����
		1MHz ��CLKDIV=0 ���� 2MHz   ��CLKDIV=1 ����CLK ��λӦ��  ��1��
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
