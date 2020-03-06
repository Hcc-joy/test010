#ifndef __GPIOLIB_H
#define __GPIOLIB_H	 
#include "sys.h"
void GPIO_PeriphClock(void);
void GPIO_In_Float(GPIO_TypeDef * GPIO_Port, int16_t GPIO_Pin);
void GPIO_In_Down(GPIO_TypeDef * GPIO_Port, int16_t GPIO_Pin);
void GPIO_In_Up(GPIO_TypeDef * GPIO_Port, int16_t GPIO_Pin);
void GPIO_Out_Push_Pull(GPIO_TypeDef * GPIO_Port, int16_t GPIO_Pin,u8 Flg);
void NVIC_Init_Config(uint8_t IRQn,uint8_t Preemption,uint8_t Sub);
void USART_Init_Config(USART_TypeDef* USARTx,uint32_t bound,uint16_t mode);
void GPIO_Out_AF_Push(GPIO_TypeDef * GPIO_Port, int16_t GPIO_Pin,u8 Flg);
void GPIO_In_Mode(GPIO_TypeDef * GPIO_Port, int16_t GPIO_Pin);
		 				    
#endif











