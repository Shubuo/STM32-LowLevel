#include "stm32f407xx.h"

void clock()
	   {
    	    RCC->CFGR      =   0;
    	    RCC->CR        =   0;
    	    RCC->CFGR     |=   (RCC_CFGR_PPRE1_0|RCC_CFGR_PPRE1_DIV2|RCC_CFGR_PPRE2_DIV2);
    	   RCC->CR       |=   RCC_CR_HSEON;  
    	    while (!(RCC->CR & 0x00020000));
    	    RCC->PLLCFGR   =   (RCC_PLLCFGR_PLLQ_2|RCC_PLLCFGR_PLLQ_1|RCC_PLLCFGR_PLLQ_0 |RCC_PLLCFGR_PLLSRC_HSE);
    	    RCC->PLLCFGR  |=   (RCC_PLLCFGR_PLLN_8 |RCC_PLLCFGR_PLLN_6 |RCC_PLLCFGR_PLLN_4);
          RCC->CR       |=    RCC_CR_PLLON;   
          while(!(RCC->CR & 0x02000000));      
          FLASH->ACR     =    0x00000605;      
          RCC->CFGR     |=    0x00000002;      
          while ((RCC->CFGR & 0x0000000F) != 0x0000000A); 
}
		 
int main()
{
	
	 clock(); //config Clock
	
	
	// config GPIO A pin 0 ,1,2 and 3 for PWM output
		RCC->AHB1ENR |=(1<<0); 
	  GPIOA->MODER |= (2<<2*0)+(2<<2*1)+(2<<2*2)+(2<<2*3); // PA0 -> PA3 en mode AF "10"
	  GPIOA->OSPEEDR |= (3<<2*0)+(3<<2*1)+(3<<2*2)+(3<<2*3); // output speed max=100 MHz 
	  
	  // config AF
	GPIOA->AFR[0] |=(1<<4*0)+(1<<4*1)+(1<<4*2)+(1<<4*3); // AF1 (Tim2) as AF for PA0...PA3
	
	// config timer 2:
	  RCC->APB1ENR |=(1<<0); // Clock foor timer 2 
	  TIM2->PSC =100;  //    Prescaler = 100
	  TIM2->ARR = 1000; //Autoreload= 1000;  ftimer= 84MHz/(100+1)*(1000+1); 
	  TIM2->EGR |=1; // activation compteur
	  TIM2->CCMR1 |= 0x6060; //  Set PWM channel 1 and PWM Channel 2
	  TIM2->CCMR2 |= 0x6060; // Set PWM channel 3 and PWM Channel 4
	
	  // Duty cycle with ARR = 1000
	TIM2->CCR1 |= 250;  // channel 1: 25 % 
	TIM2->CCR2 |= 500;  //channel 2 : 50 %
	TIM2->CCR3 |= 750;  // channel 3: 75 %
	TIM2->CCR4 |= 1000; // channel 4: 100%
	 
    TIM2->CCER |= 0x1111; // // Enable PWM1,PWM2,PWM3,and PWM4
	  TIM2->CR1 |= 1;//Enable TIM2

	
	while(1)
	{
		//
	}


}
