/*----------------------------------------------------------------------------
 * Name:    Timer.c
 * Purpose: Timer usage for STM32
 * Version: V1.00
 *----------------------------------------------------------------------------
 * This file is part of the uVision/ARM development tools.
 * This software may only be used under the terms of a valid, current,
 * end user licence from KEIL for a compatible version of KEIL software
 * development tools. Nothing else gives you the right to use this software.
 *
 * Copyright (c) 2005-2007 Keil Software. All rights reserved.
 *----------------------------------------------------------------------------*/
#include "stm32f10x.h"                  // Device header
#include <stm32f10x_lib.h>                        // STM32F10x Library Definitions
#include "STM32_Init.h"                           // STM32 Initialization


int ledPos = 0;                                   // current led position from 0..7
int ledDir = 1;

/*------------------------------------------------------------------------------
  Timer1 Update Interrupt Handler
 *------------------------------------------------------------------------------*/
void TIM1_UP_IRQHandler (void) {

  if ((TIM1->SR & 0x0001) != 0) {                 // check interrupt source

    if (ledPos == 0)                              // lower limit reached ?
	  ledDir = 1;                                 // move up

    if (ledPos == 7)                              // upper limit reached ?
	  ledDir = -1;                                // move down

    GPIOB->ODR &= ~(1 << (ledPos+8));             // switch off old LED position
    ledPos += ledDir;
    GPIOB->ODR |=  (1 << (ledPos+8));             // switch on  new LED position

    TIM1->SR &= ~(1<<0);                          // clear UIF flag
 }
} // end TIM1_UP_IRQHandler


/*----------------------------------------------------------------------------
  MAIN function
 *----------------------------------------------------------------------------*/
int main (void) {

  stm32_Init ();                                  // STM32 setup

  GPIOB->ODR |=  (1 << (ledPos+8));               // switch on initial LED position

  while (1) {                                     // Loop forever
    ;
  } // end while
} // end main
