/*
 * InputDevice.h
 *
 *  Created on: 2020年2月22日
 *      Author: Hcc
 */

#ifndef HARDWARE_INPUTDEVICE_H_
#define HARDWARE_INPUTDEVICE_H_
#include "sys.h"
#include "gpiolib.h"
#include "Tim.h"
#include "usart.h"

#define MP1_GPIO_Prt  GPIOI
#define MP1_GPIO_Pin  GPIO_Pin_0
#define MP2_GPIO_Prt  GPIOI
#define MP2_GPIO_Pin  GPIO_Pin_1
#define MP3_GPIO_Prt  GPIOI
#define MP3_GPIO_Pin  GPIO_Pin_2
#define MP4_GPIO_Prt  GPIOI
#define MP4_GPIO_Pin  GPIO_Pin_3

#define MP5_GPIO_Prt  GPIOA
#define MP5_GPIO_Pin  GPIO_Pin_15
#define MP6_GPIO_Prt  GPIOC
#define MP6_GPIO_Pin  GPIO_Pin_10
#define MP7_GPIO_Prt  GPIOC
#define MP7_GPIO_Pin  GPIO_Pin_11
#define MP8_GPIO_Prt  GPIOC
#define MP8_GPIO_Pin  GPIO_Pin_9

#define MP9_GPIO_Prt  GPIOA
#define MP9_GPIO_Pin  GPIO_Pin_8
#define MP10_GPIO_Prt  GPIOA
#define MP10_GPIO_Pin  GPIO_Pin_11
#define MP11_GPIO_Prt  GPIOA
#define MP11_GPIO_Pin  GPIO_Pin_12
#define MP12_GPIO_Prt  GPIOH
#define MP12_GPIO_Pin  GPIO_Pin_13

#define MP13_GPIO_Prt  GPIOH
#define MP13_GPIO_Pin  GPIO_Pin_14
#define MP14_GPIO_Prt  GPIOH
#define MP14_GPIO_Pin  GPIO_Pin_15
#define MP15_GPIO_Prt  GPIOG
#define MP15_GPIO_Pin  GPIO_Pin_5
#define MP16_GPIO_Prt  GPIOG
#define MP16_GPIO_Pin  GPIO_Pin_6

#define MP17_GPIO_Prt  GPIOG
#define MP17_GPIO_Pin  GPIO_Pin_7
#define MP18_GPIO_Prt  GPIOG
#define MP18_GPIO_Pin  GPIO_Pin_8
#define MP19_GPIO_Prt  GPIOC
#define MP19_GPIO_Pin  GPIO_Pin_6
#define MP20_GPIO_Prt  GPIOC
#define MP20_GPIO_Pin  GPIO_Pin_7

#define MP21_GPIO_Prt  GPIOC
#define MP21_GPIO_Pin  GPIO_Pin_8
#define MP22_GPIO_Prt  GPIOD
#define MP22_GPIO_Pin  GPIO_Pin_12
#define MP23_GPIO_Prt  GPIOD
#define MP23_GPIO_Pin  GPIO_Pin_13
#define MP24_GPIO_Prt  GPIOD
#define MP24_GPIO_Pin  GPIO_Pin_14

#define MP25_GPIO_Prt  GPIOD
#define MP25_GPIO_Pin  GPIO_Pin_15
#define MP26_GPIO_Prt  GPIOG
#define MP26_GPIO_Pin  GPIO_Pin_2
#define Key1_GPIO_Prt  GPIOG
#define Key1_GPIO_Pin  GPIO_Pin_3
#define Key2_GPIO_Prt  GPIOG
#define Key2_GPIO_Pin  GPIO_Pin_4
#define Key3_GPIO_Prt  GPIOI
#define Key3_GPIO_Pin  GPIO_Pin_8

#define MP1 0 //
#define MP2 1
#define MP3 2
#define MP4 3
#define MP5 4
#define MP6 5
#define MP7 6
#define MP8 7
#define MP9 8
#define MP10 9
#define MP11 10
#define MP12 11
#define MP13 12
#define MP14 13
#define MP15 14
#define MP16 15
#define MP17 16
#define MP18 17
#define MP19 18
#define MP20 19
#define MP21 20
#define MP22 21
#define MP23 22
#define MP24 23
#define MP25 24
#define MP26 25

#define Key1 26 //开始按键1
#define Key2 27 //开始按键2
#define Key3 28 

#define LEDS1_M 29
#define LEDS2_M 30
#define LEDS3_M 31
#define LEDS4_M 32
#define SLC 33
#define OL1 34  //产品在位1
#define OL2 35	//产品在位2

typedef struct State
{
	u8 StartTestFlg : 1;
	u8 SLCFlg : 1;
	u8 InstallFinish : 1;
	u8 SlottedTongueFlg : 1;

	u8 OL1Flg : 1;
	u8 OL2Flg : 1;
	u8 EmergencySwFlg : 1;
	u8 MP1Flg : 1;

	u8 MP2Flg : 1;
	u8 MP3Flg : 1;
	u8 MP4Flg : 1;
	u8 MP5Flg : 1;

	u8 MP6Flg : 1;
	u8 MP7Flg : 1;
	u8 MP8Flg : 1;
	u8 MP9Flg : 1;

	u8 MP10Flg : 1;
	u8 MP11Flg : 1;
	u8 MP12Flg : 1;
	u8 MP13Flg : 1;

	u8 MP14Flg : 1;
	u8 MP15Flg : 1;
	u8 MP16Flg : 1;
	u8 MP17Flg : 1;

	u8 MP18Flg : 1;
	u8 MP19Flg : 1;
	u8 MP20Flg : 1;
	u8 MP21Flg : 1;

	u8 MP22Flg : 1;
	u8 MP23Flg : 1;
	u8 MP24Flg : 1;
	u8 MP25Flg : 1;
}State;
void InputDeviceGpioInit(void);
void SignalDetection(void);
u8 ReadInputDeviceSignal(u8 location);
#endif /* HARDWARE_INPUTDEVICE_H_ */
