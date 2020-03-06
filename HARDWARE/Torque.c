/*
 * Torque.c
 *
 *  Created on: 2020年2月27日
 *      Author: Hcc
 */

#include "Torque.h"
#include "main.h"
//求一个无序数组的中位数
u32  GetMidNumNoSort1(u32 *pData, int len)
{
	u32 *pTemp = pData;
	u32 *pTemp1 = pTemp + 1;
	u32  iTemp=0;
	for (; pTemp < (pData + len - 1); pTemp++)
	{
		for (pTemp1 = pTemp + 1; pTemp1 < (pData + len); pTemp1++)
		{
			if (*pTemp > * pTemp1)
			{
				iTemp = *pTemp;
				*pTemp = *pTemp1;
				*pTemp1 = iTemp;
			}
		}
	}
	if (len % 2 == 1)
	{
		pTemp = pData;
		return *(pTemp + ((len - 1) / 2));
	}
	if (len % 2 == 0)
	{
		pTemp = pData;
		return (*(pTemp +(len/2)-1) + *(pTemp+(len/2)))/2;
	}

}

u32 Torque_Check(u8 Channel,u8 TM707_Num)
{
	u32 ADC_Temp[100] ;
	u32 m_TempData=0;
	double TempuiVoltage1;
	int i;
	for (i = 0; i < 11; i++)
	{
		ADC_Temp[i]=TM7707_ReadADCValue(Channel, TM707_Num);
	}
	 m_TempData= GetMidNumNoSort1(ADC_Temp, 11);
	 TempuiVoltage1 = 2 * 2.5 * m_TempData /16777216;
	 return m_TempData;
	 
}