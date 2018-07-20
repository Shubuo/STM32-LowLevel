/******************** (C) COPYRIGHT 2008 STMicroelectronics ********************
* File Name          : stm32f10x_systick.c
* Author             : MCD Application Team
* Version            : V2.0.3
* Date               : 09/22/2008
* Description        : This file provides all the SysTick firmware functions.
********************************************************************************
* THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
* WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE TIME.
* AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY DIRECT,
* INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING FROM THE
* CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE CODING
* INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
*******************************************************************************/

/****************************************Copyright (c)**************************
**                                      
**                                 http://www.powermcu.com
**
**--------------File Info-------------------------------------------------------
** File name:               systick.c
** Descriptions:            Ê¹ÓÃSysTickµÄÆÕÍ¨¼ÆÊýÄ£Ê½¶ÔÑÓ³Ù½øÐÐ¹ÜÀí
**
**------------------------------------------------------------------------------
** Created by:              AVRman
** Created date:            2010-10-30
** Version:                 v1.0
** Descriptions:            The original version
**
**------------------------------------------------------------------------------
** Modified by: 			Burak BAYRAK            
** Modified date:           10.03.2012
** Version:                 v1.1
** Descriptions:            Adapted & Merged for STM32F205RB, Expkits EXS-03 DEVKIT
**
*******************************************************************************/

/*******************************************************************************
 Not:	Doðru geçikme saðlayabilmek için "system_stmf322xx.c" 
 		"STM32F2xx Clock Configuration Utility" kullanýlarak
 	   	yeniden oluþturulmalýdýr.	
*******************************************************************************/

#include "delay.h"

#if defined  (HSE_VALUE)
/* HSE deðerini yeniden tanýmlýyoruz. Bizim (EXS-03 Kartýmýzdaki kristal 8Mhz. 
 Ancak ST' nin kütüphanesinde bu deðer öntanýmlý olarak 25Mhz olarak gelmektedir.*/
 #undef HSE_VALUE
 #define HSE_VALUE    ((uint32_t)8000000)	/* Kristalimiz ne ise burada o tanýmlanmalý */
#endif /* HSE_VALUE */

#define CTRL_TICKINT_Set      ((u32)0x00000002)
#define CTRL_TICKINT_Reset    ((u32)0xFFFFFFFD)

/* Private variables ---------------------------------------------------------*/	 
static uint8_t  delay_fac_us = 0;  
static uint16_t delay_fac_ms = 0;  
static FlagStatus  Status;


/************************** SysTick Fonksiyonlarý******************************/

void SysTick_SetReload(u32 Reload)
{
  /* Check the parameters */
  assert_param(IS_SYSTICK_RELOAD(Reload));

  SysTick->LOAD = Reload;
}


void SysTick_CounterCmd(u32 SysTick_Counter)
{
  /* Check the parameters */
  assert_param(IS_SYSTICK_COUNTER(SysTick_Counter));

  if (SysTick_Counter == SysTick_Counter_Enable)
  {
    SysTick->CTRL |= SysTick_Counter_Enable;
  }
  else if (SysTick_Counter == SysTick_Counter_Disable) 
  {
    SysTick->CTRL &= SysTick_Counter_Disable;
  }
  else /* SysTick_Counter == SysTick_Counter_Clear */
  {
    SysTick->VAL = SysTick_Counter_Clear;
  }    
}

void SysTick_ITConfig(FunctionalState NewState)
{
  /* Check the parameters */
  assert_param(IS_FUNCTIONAL_STATE(NewState));

  if (NewState != DISABLE)
  {
    SysTick->CTRL |= CTRL_TICKINT_Set;
  }
  else
  {
    SysTick->CTRL &= CTRL_TICKINT_Reset;
  }
}

u32 SysTick_GetCounter(void)
{
  return(SysTick->VAL);
}

FlagStatus SysTick_GetFlagStatus(u8 SysTick_FLAG)
{
  u32 statusreg = 0, tmp = 0 ;
  FlagStatus bitstatus = RESET;

  /* Check the parameters */
  assert_param(IS_SYSTICK_FLAG(SysTick_FLAG));

  /* Get the SysTick register index */
  tmp = SysTick_FLAG >> 3;

  if (tmp == 2) /* The flag to check is in CTRL register */
  {
    statusreg = SysTick->CTRL;
  }
  else          /* The flag to check is in CALIB register */
  {
    statusreg = SysTick->CALIB;
  }

  if ((statusreg & ((u32)1 << SysTick_FLAG)) != (u32)RESET)
  {
    bitstatus = SET;
  }
  else
  {
    bitstatus = RESET;
  }
  return bitstatus;
}

/***********************Gecikme Rutinleri***************************************/

void delay_init(void)
 {
  RCC_ClocksTypeDef RCC_ClocksStatus;
  RCC_GetClocksFreq(&RCC_ClocksStatus);
  SysTick_CLKSourceConfig(SysTick_CLKSource_HCLK_Div8);  /* HCLK/8 */  
  SysTick_ITConfig(DISABLE);
  delay_fac_us = RCC_ClocksStatus.HCLK_Frequency / 8000000;	// 8Mhz Kristal için
  delay_fac_ms = RCC_ClocksStatus.HCLK_Frequency / 8000;    // 8Mhz Kristal için  
 }

void delay_ms(uint16_t nms)
{    
    uint32_t temp = delay_fac_ms * nms;

    if (temp > 0x00ffffff)
    {
        temp = 0x00ffffff;
    }
    SysTick_SetReload(temp);		             
    SysTick_CounterCmd(SysTick_Counter_Clear);	 
    SysTick_CounterCmd(SysTick_Counter_Enable);	 
    do
    {
        Status = SysTick_GetFlagStatus(SysTick_FLAG_COUNT);
    }while (Status != SET);				         
    SysTick_CounterCmd(SysTick_Counter_Disable); 
	SysTick_CounterCmd(SysTick_Counter_Clear);	 
}

void delay_us(uint32_t nus)
{ 
    SysTick_SetReload(delay_fac_us * nus);       
    SysTick_CounterCmd(SysTick_Counter_Clear);	
    SysTick_CounterCmd(SysTick_Counter_Enable);	
    do
    {
        Status = SysTick_GetFlagStatus(SysTick_FLAG_COUNT);
    }while (Status != SET);						
    SysTick_CounterCmd(SysTick_Counter_Disable);
	SysTick_CounterCmd(SysTick_Counter_Clear);	
}


