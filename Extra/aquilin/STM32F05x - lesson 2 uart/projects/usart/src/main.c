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
#include "usart.h"

// ----------------------------------------------------------------------------
// Global variables
// ----------------------------------------------------------------------------
extern volatile char rx_buffer;

// ----------------------------------------------------------------------------
// Function prototypes
// ----------------------------------------------------------------------------
void delay(const int d);

// ----------------------------------------------------------------------------
// Main
// ----------------------------------------------------------------------------
int main(void)
{
  char c1=0, c2=1;
   
  // --------------------------------------------------------------------------
  // Setup PC8 (blue LED) and PC9 (green LED)
  // GPIOC Periph clock enable
  RCC->AHBENR |= RCC_AHBENR_GPIOCEN; 
  // PC8 and PC9 in output mode
  GPIOC->MODER |= (GPIO_MODER_MODER8_0 | GPIO_MODER_MODER9_0) ;
  // Push pull mode selected
  GPIOC->OTYPER &= ~(GPIO_OTYPER_OT_8 | GPIO_OTYPER_OT_9) ;
  // Maximum speed setting (even though it is unnecessary)
  GPIOC->OSPEEDR |= (GPIO_OSPEEDER_OSPEEDR8 | GPIO_OSPEEDER_OSPEEDR9);
  // Pull-up and pull-down resistors disabled
  GPIOC->PUPDR &= ~(GPIO_PUPDR_PUPDR8 | GPIO_PUPDR_PUPDR9);

  // --------------------------------------------------------------------------
  // Setup USART1 (PA9 & PA10)
  USART_init();
  USART_clearscreen();
  USART_putstr("This is STM32F0 microcontroller lesson 2.\n");
  USART_putstr("Press different characters.\n\n");
  USART_putstr("Why is the green LED not blinking continuously??\n\n");
  
  while(1)
  {
    // Blink the green LED
    // Set PC9
    GPIOC->BSRR = GPIO_BSRR_BS_9;
    // Delay ~ 0.1 sec.
    delay(SystemCoreClock/8/10);
    // Reset PC9
    GPIOC->BSRR = GPIO_BSRR_BR_9;
    // Delay ~ 0.1 sec.
    delay(SystemCoreClock/8/10);
    
    c1 = USART_getc();
  //c1 = rx_buffer;
    
    // Different data received?
    if(c1 != c2)
    {
      // Yes
      c2=c1;
      USART_putstr("You pressed: ");
      USART_putc(c1);
      USART_putstr("\n");
    }
  }
}

void delay(const int d)
{
  volatile int i;

  for(i=d; i>0; i--){ ; }

  return;
}
