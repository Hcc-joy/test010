/*
 * WDog.c
 *
 *  Created on: 2020��1��10��
 *      Author: Hcc
 */

#include "WDog.h"

void WatchDogInit(void)
{
	GPIO_Out_Push_Pull(WDogPro,WDogPin,0);
}
/************************************************************************/
/* Function Name:	WatchFeedDog                .						*/
/* Purpose:			                                                    */
/*									                        			*/
/* Resources:						                        			*/
/* Params:			None.												*/
/* Return:			None.												*/
/* Limitation:		None.												*/
/************************************************************************/
void WatchFeedDog(void)
{
    FeedDogFinish();
}

