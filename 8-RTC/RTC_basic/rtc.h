/*
 * 			rtc.h
 *
 *  		Created on: 13.08.2018
 *      Author: Burak Yoruk
 */

//#ifndef BSP_INC_RTC_H_
//#define BSP_INC_RTC_H_

#ifndef __STM32F10x_RTC_H
#define __STM32F10x_RTC_H

#include "stm32f10x.h"                  // Device header


/** @defgroup RTC_interrupts_define 
  * @{
  */

#define RTC_IT_OW            ((uint16_t)0x0004)  /*!< Overflow interrupt */
#define RTC_IT_ALR           ((uint16_t)0x0002)  /*!< Alarm interrupt */
#define RTC_IT_SEC           ((uint16_t)0x0001)  /*!< Second interrupt */
#define IS_RTC_IT(IT) ((((IT) & (uint16_t)0xFFF8) == 0x00) && ((IT) != 0x00))
#define IS_RTC_GET_IT(IT) (((IT) == RTC_IT_OW) || ((IT) == RTC_IT_ALR) || \
                           ((IT) == RTC_IT_SEC))
/**
  * @}
  */ 

/** @defgroup RTC_interrupts_flags 
  * @{
  */

#define RTC_FLAG_RTOFF       ((uint16_t)0x0020)  /*!< RTC Operation OFF flag */
#define RTC_FLAG_RSF         ((uint16_t)0x0008)  /*!< Registers Synchronized flag */
#define RTC_FLAG_OW          ((uint16_t)0x0004)  /*!< Overflow flag */
#define RTC_FLAG_ALR         ((uint16_t)0x0002)  /*!< Alarm flag */
#define RTC_FLAG_SEC         ((uint16_t)0x0001)  /*!< Second flag */
#define IS_RTC_CLEAR_FLAG(FLAG) ((((FLAG) & (uint16_t)0xFFF0) == 0x00) && ((FLAG) != 0x00))
#define IS_RTC_GET_FLAG(FLAG) (((FLAG) == RTC_FLAG_RTOFF) || ((FLAG) == RTC_FLAG_RSF) || \
                               ((FLAG) == RTC_FLAG_OW) || ((FLAG) == RTC_FLAG_ALR) || \
                               ((FLAG) == RTC_FLAG_SEC))
#define IS_RTC_PRESCALER(PRESCALER) ((PRESCALER) <= 0xFFFFF)

// Typedefs

typedef struct
{
	uint16_t hours;
	uint16_t minutes;
	uint16_t seconds;
} time_t;

// Static functions

static uint16_t byte2bcd	(uint16_t byte);
static uint16_t bcd2byte	(uint16_t bcd);

// Public functions
void RTC_ITConfig(uint16_t RTC_IT, FunctionalState NewState);

void RTC_Clock_Config	(void);
static void SystemClock_Config(void);


void RTC_SetTime	(time_t *ptime);
void RTC_GetTime	(time_t *ptime);

#endif /* BSP_INC_RTC_H_ */
