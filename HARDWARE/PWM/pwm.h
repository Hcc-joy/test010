#ifndef _TIMER_H
#define _TIMER_H
#include "sys.h"
#include "led.h"
#include "usart.h"
#include "GPIOlib.h"
#include "TM7707.h"
#include "Torque.h"

#define Motor1 0
#define Motor2 1
#define Motor3 2
#define Motor4 3

#define Motor_PUL 0
#define Motor_DIR 1
#define Motor_EN  2

void Motor_GPIOInit(void);
void StepMotor_Init(void);
u32 MotorStartRun(u8 MotoNum,TIM_TypeDef* TIMx,u8 Orientation,int arr, long int Rotate);
void TIM_PWM_Init(TIM_TypeDef* TIMx, u32 arr,u32 psc);
#endif
