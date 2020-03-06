#include "main.h"	


void ReceiveInstruction(void);
 uint32_t aaa = 0;
 double TempuiVoltage;

int main(void)
{ 
	delay_init(180);  //��ʼ����ʱ����
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//����ϵͳ�ж����ȼ�����2
	GPIO_PeriphClock();
	TIM3_Int_Init(10-1,9000-1);
	Dsth_MotorInit();//ADC��ʼ��
	WatchDogInit();
	USART1_init(115200);//��ʼ�����ڲ�����Ϊ115200
	USART2_init(115200);//��ʼ�����ڲ�����Ϊ115200
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
	m_aAckSendData[4] = g_aUsart1RxData[4];  //��������
	switch (g_aUsart1RxData[4])
	{
	case ACKFrame://
		if (g_aUsart1RxData[5] == 0x01)
		{
			//PCӦ��
		}
		else if (g_aUsart1RxData[5] == 0x04)
		{
			//ack pc
			m_aAckSendData[5] = 0x02;
			Ack_SendData(m_aAckSendData, 2);
			//���ذ帴λ    
		}
		break;
	case DTSHMotorVoltage://��ȡ��������ѹ��OK��
		m_DataTempVuale_16=(Get_Adc_Average(DsthMotorVoltageADCx,DsthMotorVoltageADCChannel,GetVNum)*(3300.0/4096)*2);
		m_aAckSendData[5] = m_DataTempVuale_16 >> 8;
		m_aAckSendData[6] = m_DataTempVuale_16;
		Ack_SendData(m_aAckSendData, 3);
		break;
	case DTSHSensorVoltage://��ʹ�øù���

		break;
	case DTSHMotorCurrent://ok
		m_DataTempVuale_16=Dsth_Motor(MotorRev,0);
		m_aAckSendData[5] = m_DataTempVuale_16 >> 8;
		m_aAckSendData[6] = m_DataTempVuale_16;
		Dsth_Motor(MotorFwd,0);//����
		Ack_SendData(m_aAckSendData, 3);
		break;
	case DTSHSensorCurrent://���崫��������ok
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
	 // Dsth_Motor(MotorFwd,0);//��֤�ڿ���״̬�� 01 00 00 01
		m_DataTempVuale_32=DsthSensorDetection();//�ȶ���һ��
		m_aAckSendData[5] = m_DataTempVuale_32 >> 24;
		m_aAckSendData[6] = m_DataTempVuale_32 >> 16;
		m_aAckSendData[7] = m_DataTempVuale_32 >> 8;
		m_aAckSendData[8] = m_DataTempVuale_32;
		Dsth_Motor(MotorRev, 0);//������� ���Լ�������
		OutputDeviceSet(MV6,SET);//���벦�ֵ��
	  
		//�����뵽λ
		delay_ms(100);//����ʹ�ã���ʽ��ɾ�� 
		MotorStartRun(Motor1, TIM10, MotorFwd, 500, 3200);
		OutputDeviceSet(MV7, SET);//���²���
		m_DataTempVuale_32=DsthSensorDetection();//���һ�������෴����ȷ
	
		MotorStartRun(Motor1, TIM10, MotorRev, 500, 3200);
		OutputDeviceSet(MV6, RESET);//�γ����ֵ��
		OutputDeviceSet(MV7, RESET);//�ջز���

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
		 m_aAckSendData[5] = 0x00;//����
		if (BitState.InstallFinish == RESET)
		{
			m_aAckSendData[5] = 0x01;//�쳣
		}
		Ack_SendData(m_aAckSendData, 2);
		break;
	case DTSHBoltRetractionHeightDifference://�������غ�߶�
		 Dsth_Motor(MotorFwd, 0);//����
		 delay_ms(200);
		for(i=0;i<20;i++)
		{
	  	m_DataTempVuale_32 += TM7707_ReadADCValue(CH_1, TM7707_U26);//ֻ�ϴ�ADֵ
		}
		m_DataTempVuale_32=m_DataTempVuale_32/20;
		m_aAckSendData[5] = m_DataTempVuale_32 >> 24;
		m_aAckSendData[6] = m_DataTempVuale_32>>16;
		m_aAckSendData[7] = m_DataTempVuale_32>>16;
		Ack_SendData(m_aAckSendData, 4);
		
	  //	printf("Height:%1.4f",2*2.5 * m_DataTempVuale_32 /16777216/0.071-35); //����ʹ��
		
		break;
	case DTSHBoltProtrudeHeightDifference://���������߶�
		TempuiVoltage=0;
		Dsth_Motor(MotorRev, 0);//���
  	    delay_ms(500);
	    for(i=0;i<20;i++)
	    {
	  	m_DataTempVuale_32 += TM7707_ReadADCValue(CH_1, TM7707_U26);
	    }
	    m_DataTempVuale_32=m_DataTempVuale_32/20;
		m_aAckSendData[5] = m_DataTempVuale_32 >> 24;
		m_aAckSendData[6] = m_DataTempVuale_32>>16;
		m_aAckSendData[7] = m_DataTempVuale_32>>16;
		Dsth_Motor(MotorFwd, 0);//����
		Ack_SendData(m_aAckSendData, 4);	 
		 
			 
  //	printf("Height:%1.4f",2*2.5 * m_DataTempVuale_32 /16777216/0.071-35); //����ʹ��

		break;
	case DTSHBoltProtrudeTorsion:
		//OutputDeviceSet(MV6,SET);
		m_DataTempVuale_32 = Torque_Check(CH_2, TM7707_U27);//��Ҫ��ȥ0��
		delay_ms(100);//����ʹ�ã���ʽ��ɾ��
		m_DataTempVuale_32=MotorStartRun(Motor1, TIM10, MotorFwd, 500, 6400)- m_DataTempVuale_32;
		delay_ms(100);
		MotorStartRun(Motor1, TIM10, MotorRev, 500, 6400);
		m_aAckSendData[5] = m_DataTempVuale_32 >> 24;
		m_aAckSendData[6] = m_DataTempVuale_32 >> 16;
		m_aAckSendData[7] = m_DataTempVuale_32 >> 16;
		Ack_SendData(m_aAckSendData, 4);
		break;
	case DTSHBoltRetractionTorsion:
		m_DataTempVuale_32 = Torque_Check(CH_1, TM7707_U1);//��Ҫ��ȥ0��
		delay_ms(100);//����ʹ�ã���ʽ��ɾ��
		MotorStartRun(Motor1, TIM10, MotorFwd, 500, 6400);
		delay_ms(100);
		m_DataTempVuale_32 = MotorStartRun(Motor2, TIM10, MotorRev, 500, 6400) - m_DataTempVuale_32;
		m_aAckSendData[5] = m_DataTempVuale_32 >> 24;
		m_aAckSendData[6] = m_DataTempVuale_32 >> 16;
		m_aAckSendData[7] = m_DataTempVuale_32 >> 16;
		Ack_SendData(m_aAckSendData, 4);
	
		break;
	case DTSHIdleTorsion:
		m_DataTempVuale_32 = Torque_Check(CH_1, TM7707_U1);//��Ҫ��ȥ0��
		delay_ms(100);//����ʹ�ã���ʽ��ɾ��
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
