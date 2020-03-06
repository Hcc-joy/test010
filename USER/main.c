#include "main.h"	


void ReceiveInstruction(void);
 uint32_t aaa = 0;
 double TempuiVoltage;

int main(void)
{ 
	delay_init(180);  //初始化延时函数
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//设置系统中断优先级分组2
	GPIO_PeriphClock();
	TIM3_Int_Init(10-1,9000-1);
	Dsth_MotorInit();//ADC初始化
	WatchDogInit();
	USART1_init(115200);//初始化串口波特率为115200
	USART2_init(115200);//初始化串口波特率为115200
	TM7707_Init();
	StepMotor_Init();
    OutputDeviceGpioInit();
	InputDeviceGpioInit();
   while(1) 
	{
	   SignalDetection();
	   ReceiveInstruction();
	}
}

extern u8 g_aUsart1RxData[Uart1RxLen];
extern u32 DsthSensorState;
State BitState;
void ReceiveInstruction(void)
{
	u8 m_aAckSendData[100] = { TxFrameHeader};
	int i,j;
	u8 m_DataTempVuale_8 = 0;
	u16 m_DataTempVuale_16=0;
	u32 m_DataTempVuale_32=0;
	double uATempuiVoltage;
	if (g_aUsart1RxData[0] != RxFrameHeader)
		return;
	m_aAckSendData[4] = g_aUsart1RxData[4];  //赋功能码
	switch (g_aUsart1RxData[4])
	{
	case ACKFrame://
		if (g_aUsart1RxData[5] == 0x01)
		{
			//PC应答
		}
		else if (g_aUsart1RxData[5] == 0x04)
		{
			//ack pc
			m_aAckSendData[5] = 0x02;
			Ack_SendData(m_aAckSendData, 2);
			//主控板复位    
		}
		break;
	case DTSHMotorVoltage://获取电机供电电压（OK）
		m_DataTempVuale_16=(Get_Adc_Average(DsthMotorVoltageADCx,DsthMotorVoltageADCChannel,GetVNum)*(3300.0/4096)*2);
		m_aAckSendData[5] = m_DataTempVuale_16 >> 8;
		m_aAckSendData[6] = m_DataTempVuale_16;
		Ack_SendData(m_aAckSendData, 3);
		break;
	case DTSHSensorVoltage://不使用该功能

		break;
	case DTSHMotorCurrent://ok
		m_DataTempVuale_16=Dsth_Motor(MotorRev,0);
		m_aAckSendData[5] = m_DataTempVuale_16 >> 8;
		m_aAckSendData[6] = m_DataTempVuale_16;
		Dsth_Motor(MotorFwd,0);//缩回
		Ack_SendData(m_aAckSendData, 3);
		break;
	case DTSHSensorCurrent://锁体传感器电流ok
		m_DataTempVuale_32 = TM7707_ReadADCValue(CH_1,TM7707_U27);
		m_DataTempVuale_32=m_DataTempVuale_32*2.5/16777216/10*1000*1000;
		m_aAckSendData[5] = m_DataTempVuale_32 >> 24;
	 	m_aAckSendData[6] = m_DataTempVuale_32 >> 16;
		m_aAckSendData[7] = m_DataTempVuale_32 >> 8;
		m_aAckSendData[8] = m_DataTempVuale_32 ;
		Ack_SendData(m_aAckSendData, 5);
		break;
	case DTSHOptocouplerState:
		DsthSensorState=0;
	 // Dsth_Motor(MotorFwd,0);//保证在开门状态下 01 00 00 01
		m_DataTempVuale_32=DsthSensorDetection();//先读第一次
		m_aAckSendData[5] = m_DataTempVuale_32 >> 24;
		m_aAckSendData[6] = m_DataTempVuale_32 >> 16;
		m_aAckSendData[7] = m_DataTempVuale_32 >> 8;
		m_aAckSendData[8] = m_DataTempVuale_32;
		Dsth_Motor(MotorRev, 0);//动作打回 可以检测出两个
		OutputDeviceSet(MV6,SET);//插入拨轮电机
	  
		//检测插入到位
		delay_ms(100);//测试使用，正式版删除 
		MotorStartRun(Motor1, TIM10, MotorFwd, 500, 3200);
		OutputDeviceSet(MV7, SET);//按下叉舌
		m_DataTempVuale_32=DsthSensorDetection();//与第一次数据相反则正确
	
		MotorStartRun(Motor1, TIM10, MotorRev, 500, 3200);
		OutputDeviceSet(MV6, RESET);//拔出拨轮电机
		OutputDeviceSet(MV7, RESET);//收回叉舌

		for (i = 0,j=8; i < 4; i++,j--)
		{
			m_DataTempVuale_8 = m_DataTempVuale_32 >> i*8;
			if (m_DataTempVuale_8 == m_aAckSendData[j])
			{
				m_aAckSendData[j] = 0x01;
			}
			else
			{
				m_aAckSendData[j] = 0x00;
			}
		}
		Ack_SendData(m_aAckSendData, 5);
		
		break;
	case DTSHIntallState:
		 m_aAckSendData[5] = 0x00;//正常
		if (BitState.InstallFinish == RESET)
		{
			m_aAckSendData[5] = 0x01;//异常
		}
		Ack_SendData(m_aAckSendData, 2);
		break;
	case DTSHBoltRetractionHeightDifference://锁舌缩回后高度
		 Dsth_Motor(MotorFwd, 0);//缩回
		 delay_ms(200);
		for(i=0;i<20;i++)
		{
	  	m_DataTempVuale_32 += TM7707_ReadADCValue(CH_1, TM7707_U26);//只上传AD值
		}
		m_DataTempVuale_32=m_DataTempVuale_32/20;
		m_aAckSendData[5] = m_DataTempVuale_32 >> 24;
		m_aAckSendData[6] = m_DataTempVuale_32>>16;
		m_aAckSendData[7] = m_DataTempVuale_32>>16;
		Ack_SendData(m_aAckSendData, 4);
		
	  //	printf("Height:%1.4f",2*2.5 * m_DataTempVuale_32 /16777216/0.071-35); //调试使用
		
		break;
	case DTSHBoltProtrudeHeightDifference://锁舌伸出后高度
		TempuiVoltage=0;
		Dsth_Motor(MotorRev, 0);//伸出
  	    delay_ms(500);
	    for(i=0;i<20;i++)
	    {
	  	m_DataTempVuale_32 += TM7707_ReadADCValue(CH_1, TM7707_U26);
	    }
	    m_DataTempVuale_32=m_DataTempVuale_32/20;
		m_aAckSendData[5] = m_DataTempVuale_32 >> 24;
		m_aAckSendData[6] = m_DataTempVuale_32>>16;
		m_aAckSendData[7] = m_DataTempVuale_32>>16;
		Dsth_Motor(MotorFwd, 0);//缩回
		Ack_SendData(m_aAckSendData, 4);	 
		 
			 
  //	printf("Height:%1.4f",2*2.5 * m_DataTempVuale_32 /16777216/0.071-35); //调试使用

		break;
	case DTSHBoltProtrudeTorsion:
		//OutputDeviceSet(MV6,SET);
		m_DataTempVuale_32 = Torque_Check(CH_2, TM7707_U27);//需要减去0点
		delay_ms(100);//测试使用，正式版删除
		m_DataTempVuale_32=MotorStartRun(Motor1, TIM10, MotorFwd, 500, 6400)- m_DataTempVuale_32;
		delay_ms(100);
		MotorStartRun(Motor1, TIM10, MotorRev, 500, 6400);
		m_aAckSendData[5] = m_DataTempVuale_32 >> 24;
		m_aAckSendData[6] = m_DataTempVuale_32 >> 16;
		m_aAckSendData[7] = m_DataTempVuale_32 >> 16;
		Ack_SendData(m_aAckSendData, 4);
		break;
	case DTSHBoltRetractionTorsion:
		m_DataTempVuale_32 = Torque_Check(CH_1, TM7707_U1);//需要减去0点
		delay_ms(100);//测试使用，正式版删除
		MotorStartRun(Motor1, TIM10, MotorFwd, 500, 6400);
		delay_ms(100);
		m_DataTempVuale_32 = MotorStartRun(Motor2, TIM10, MotorRev, 500, 6400) - m_DataTempVuale_32;
		m_aAckSendData[5] = m_DataTempVuale_32 >> 24;
		m_aAckSendData[6] = m_DataTempVuale_32 >> 16;
		m_aAckSendData[7] = m_DataTempVuale_32 >> 16;
		Ack_SendData(m_aAckSendData, 4);
	
		break;
	case DTSHIdleTorsion:
		m_DataTempVuale_32 = Torque_Check(CH_1, TM7707_U1);//需要减去0点
		delay_ms(100);//测试使用，正式版删除
		m_DataTempVuale_32 = MotorStartRun(Motor2, TIM10, MotorRev, 500, 6400) - m_DataTempVuale_32;
		delay_ms(100);
		m_aAckSendData[5] = m_DataTempVuale_32 >> 24;
		m_aAckSendData[6] = m_DataTempVuale_32 >> 16;
		m_aAckSendData[7] = m_DataTempVuale_32 >> 16;
		Ack_SendData(m_aAckSendData, 4);
		break;
	case DTSHOpenDoorState:
		
		m_aAckSendData[5] = 0;
		Ack_SendData(m_aAckSendData, 2);

		break;
	case DTSHCloseDoorState:
		m_aAckSendData[5] = 0;
		Ack_SendData(m_aAckSendData, 2);
		break;
	case DTSHBoltViolencePress:
		m_aAckSendData[5] = 0;
		Ack_SendData(m_aAckSendData, 2);
     break;
	case SolenoidValveState:

		break;
	default:
		break;
	}
	memset(g_aUsart1RxData, 0x00, sizeof(g_aUsart1RxData));//
}
