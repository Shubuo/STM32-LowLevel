/******************************************************************************/
/* BLINKY.C: LED Flasher                                                      */
/******************************************************************************/
/* This file is part of the uVision/ARM development tools.                    */
/* Copyright (c) 2010 Keil Software. All rights reserved.                     */
/* This software may only be used under the terms of a valid, current,        */
/* end user licence from KEIL for a compatible version of KEIL software       */
/* development tools. Nothing else gives you the right to use this software.  */
/******************************************************************************/

#include "stm32f10x.h"


#define LED_BLINK_RATE    20             /* rate = LED_BLINK_RATE * systick   */
#define LED_NUM            2             /* Number of user LEDs               */
const unsigned long led_mask[] = { 1UL<<8, 1UL<<9 };//256,512

#define USER1              0x0001        /* PA0 : USER1                       */
#define UNBOUNCE_CNT       10            /* unbounce the Push Button          */


uint32_t ledVal   = 1;
uint32_t ledOn    = 0;
uint32_t ledBlink = 0;

/*----------------------------------------------------------------------------
  configer LED pins
 *----------------------------------------------------------------------------*/
__INLINE static void LED_Config(void) {

  RCC->APB2ENR |=  1 <<  4;                    /* Enable GPIOC clock          */
  GPIOC->CRH   &= 0xFFFFFF00;                  /* Configure the GPIO for LEDs */
  GPIOC->CRH   |= 0x00000033;                  /* Configure the GPIO for LEDs */
}

/*----------------------------------------------------------------------------
  Switch on LEDs
 *----------------------------------------------------------------------------*/
__INLINE static void LED_On (uint32_t num) {

  GPIOC->BSRR = led_mask[num];                 /* Turn On  LED                */
}


/*----------------------------------------------------------------------------
  Switch off LEDs
 *----------------------------------------------------------------------------*/
__INLINE static void LED_Off (uint32_t num) {

  GPIOC->BRR  = led_mask[num];                 /* Turn Off LED                */
}

/*----------------------------------------------------------------------------
  Function that outputs value to LEDs
 *----------------------------------------------------------------------------*/
void LED_Out(uint32_t value) {
  int i;

  for (i = 0; i < LED_NUM; i++) {
    if (value & (1<<i)) {
      LED_On (i);
    } else {
      LED_Off(i);
    }
  }
}

/*----------------------------------------------------------------------------
  configer Button pins
 *----------------------------------------------------------------------------*/
__INLINE static void BTN_Config(void) {

  RCC->APB2ENR |=  1 <<  2;                    /* Enable GPIOA clock          */
  GPIOA->CRL   &= 0xFFFFFFF0;                  /* Configure the GPIO for BTNs */
  GPIOA->CRL   |= 0x00000008;                  /* Configure the GPIO for BTNs */
}

/*----------------------------------------------------------------------------
  Read Button pins
 *----------------------------------------------------------------------------*/
__INLINE static uint32_t BTN_Get(void) {

 return (GPIOA->IDR & 0x0001);

}


/*----------------------------------------------------------------------------
  USER1Pressed
  check if USER1 is pressed (unbounced).
 *----------------------------------------------------------------------------*/
uint32_t BTN_Pressed (void) {
  static uint32_t USER1KeyCount = 0, USER1KeyPressed = 0;

  if (USER1KeyPressed) {
    if ((BTN_Get() == 0 )) {                   /* Check if USER1 not pressed  */
      if (USER1KeyCount < UNBOUNCE_CNT) USER1KeyCount++;
      else {
        USER1KeyPressed = 0;
        USER1KeyCount = 0;    
      }
    }
  }
  else {
    if (!(BTN_Get() == 0 ))  {                 /* Check if USER1 pressed      */
      if (USER1KeyCount < UNBOUNCE_CNT) USER1KeyCount++;
      else {
        USER1KeyPressed = 1;
        USER1KeyCount = 0;
		return (1);
      }
    }
  }
  return (0);
}
/*----------------------------------------------------------------------------
  SysTick_Handler
 *----------------------------------------------------------------------------*/
void SysTick_Handler (void) {
  static uint32_t ticks;

  if (ticks++ >= LED_BLINK_RATE) {
    ticks    = 0;
    ledBlink = 1;
  }

}


/*----------------------------------------------------------------------------
  MAIN function
 *----------------------------------------------------------------------------*/
int main (void) {

  LED_Config();
  BTN_Config();                             
 
  SysTick_Config(SystemCoreClock / 100);       /* Setup SysTick Timer  (10ms) */
  
  while(1) {
    if (BTN_Pressed()) {
      ledVal += 1;
      if (ledVal > 3) ledVal = 1;
    }

    if (ledBlink == 1) {
      ledBlink  = 0;
	  ledOn    ^= 1;
	  if (ledOn == 1)
        LED_Out (ledVal);                      /* switch the LEDs on          */
      else 
        LED_Out (0);                           /* switch the LEDs off         */    
	}
  }
  
}
