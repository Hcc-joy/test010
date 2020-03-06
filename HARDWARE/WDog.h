/*
 * WDog.h
 *
 *  Created on: 2020Äê1ÔÂ10ÈÕ
 *      Author: Hcc
 */

#ifndef HARDWARE_WDOG_H_
#define HARDWARE_WDOG_H_

#include "sys.h"
#include "gpiolib.h"


#define WDogPro  GPIOA
#define WDogPin  GPIO_Pin_1 
#define StartFeedDog()  GPIO_ResetBits(WDogPro,WDogPin)
#define FeedDogFinish() GPIO_SetBits(WDogPro,WDogPin)

void WatchDogInit(void);
void WatchFeedDog(void);
#endif /* HARDWARE_WDOG_H_ */
