/******************************************************************************
 * Project        : HAN ESE PRJ2, PRJ1V & PRJ1D
 * File           : Main program
 * Copyright      : 2013 HAN Embedded Systems Engineering
 ******************************************************************************
  Change History:

    Version 1.0 - April 2013
    > Initial revision

******************************************************************************/
#include "stm32f0xx.h"
 
void delay(const int d);

int main(void)
{
  // GPIOC Periph clock enable
  RCC->AHBENR |= RCC_AHBENR_GPIOCEN; 

  // PC8 and PC9 in output mode
  GPIOC->MODER |= (GPIO_MODER_MODER8_0 | GPIO_MODER_MODER9_0);

  // Push pull mode selected
  GPIOC->OTYPER &= ~(GPIO_OTYPER_OT_8 | GPIO_OTYPER_OT_9);

  // Maximum speed setting
  GPIOC->OSPEEDR |= (GPIO_OSPEEDER_OSPEEDR8 | GPIO_OSPEEDER_OSPEEDR9);

  // Pull-up and pull-down resistors disabled
  GPIOC->PUPDR &= ~(GPIO_PUPDR_PUPDR8 | GPIO_PUPDR_PUPDR9);

  while(1)
  {
    // Set PC9
    GPIOC->BSRR = GPIO_BSRR_BS_9;
    // Reset PC8
    GPIOC->BSRR = GPIO_BSRR_BR_8;
    // Delay ~ 1 sec.
    delay(SystemCoreClock/8);    
    
    // Reset PC9
    GPIOC->BSRR = GPIO_BSRR_BR_9;
    // Set PC8
    GPIOC->BSRR = GPIO_BSRR_BS_8;
    // Delay ~ 1 sec.
    delay(SystemCoreClock/8);
  }
}

void delay(const int d)
{
  volatile int i;

  for(i=d; i>0; i--){ ; }

  return;
}

// When compiled with maximum optimization level (-O3), this delay function
// produces the following assembly instructions (start the debugger to see).
// When we know the number of cycles for each instruction, the timing of one
// delay loop is easy to calculate.
//
// 8 cycles per loop => 8 * (1/SystemCoreClock) sec.
//
// Ref: http://infocenter.arm.com/help/index.jsp?topic=/com.arm.doc.ddi0432c/CHDCICDF.html
//
//                                              Cycles required
// 0x080000E2 9000      STR      r0,[sp,#0x00]  2
// 0x080000E4 2800      CMP      r0,#0x00       1
// 0x080000E6 DD01      BLE      0x080000EC     1
// 0x080000E8 1E40      SUBS     r0,r0,#1       1
// 0x080000EA E7FA      B        0x080000E2     3
