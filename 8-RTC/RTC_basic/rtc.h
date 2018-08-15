/*
 * 			rtc.h
 *
 *  		Created on: 13.08.2018
 *      Author: Burak Yoruk
 */

#ifndef BSP_INC_RTC_H_
#define BSP_INC_RTC_H_

#include "stm32f10x.h"                  // Device header

// Typedefs

typedef struct
{
	uint8_t hours;
	uint8_t minutes;
	uint8_t seconds;
} time_t;

// Static functions

static uint8_t byte2bcd	(uint8_t byte);
static uint8_t bcd2byte	(uint8_t bcd);

// Public functions

void BSP_RTC_Clock_Config	(void);
static void SystemClock_Config(void);


#endif /* BSP_INC_RTC_H_ */