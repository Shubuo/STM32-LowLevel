/********************************************************************************
  *
	* @file    Button Kontrol with Timer EXS03 
  * @author  Burak Yörük
  * @date    12-02-2018 <- 05-08-2017
	* @brief   This file for understanding timer functions
  *          
  ******************************************************************************/

#include "stm32f10x.h"

#include <stdbool.h>


volatile uint32_t msTicks;               // counts 1ms timeTicks
/*----------------------------------------------------------------------------
 * SysTick_Handler:
 *----------------------------------------------------------------------------*/
void SysTick_Handler(void) {
  msTicks++; 
}
/*----------------------------------------------------------------------------
 * Delay: delays a number of Systicks
 *----------------------------------------------------------------------------*/
void Delay (uint32_t dlyTicks) 
	{
  uint32_t curTicks;

  curTicks = msTicks;
  while ((msTicks - curTicks) < dlyTicks) { __NOP(); }
	}

void SystemCoreClockConfigure(void) 
	/*----------------------------------------------------------------------------
 * main: blink LED and check button state
 *----------------------------------------------------------------------------*/
	{
	//STM32F103 Blinky(Nucleo) Kodu SystemCoreClockConfigure
	RCC->CR |= ((uint32_t)0x01);                   				   // Enable HSI 				 						/*!< Internal High Speed clock enable */ RCC_CR_HSION
  while ((RCC->CR & (uint32_t)0x02) == 0);                 // Wait for HSI Ready 						/*!<0x02(RCC_CR_HSIRDY ) Internal High Speed clock ready flag */

  RCC->CFGR = (uint32_t)0x00;                              // HSI is system clock
  while ((RCC->CFGR & (uint32_t)0x0C) != (uint32_t)0x00);  // Wait for HSI used as system clock 		/*!< SWS[1:0] bits (System Clock Switch Status) */0x0c(RCC_CFGR_SWS)
																														//0x00 (RCC_CFGR_SWS_HSI)								/*!< HSI oscillator used as system clock */		
  FLASH->ACR  = FLASH_ACR_PRFTBE;                          // Enable Prefetch Buffer
  FLASH->ACR |= FLASH_ACR_LATENCY;                         // Flash 1 wait state

  RCC->CFGR |= RCC_CFGR_HPRE_DIV1;                         // HCLK = SYSCLK
  RCC->CFGR |= RCC_CFGR_PPRE1_DIV2;                        // APB1 = HCLK/2
  RCC->CFGR |= RCC_CFGR_PPRE2_DIV1;                        // APB2 = HCLK

  RCC->CR &= ~RCC_CR_PLLON;                                // Disable PLL

 // PLL configuration:  = HSI/2 * 12 = 48 MHz
  RCC->CFGR &= ~(RCC_CFGR_PLLSRC | RCC_CFGR_PLLXTPRE | RCC_CFGR_PLLMULL);
  RCC->CFGR |=  (RCC_CFGR_PLLSRC_HSI_Div2 | RCC_CFGR_PLLMULL12);

  RCC->CR |= RCC_CR_PLLON;                                 // Enable PLL
  while((RCC->CR & RCC_CR_PLLRDY) == 0) __NOP();           // Wait till PLL is ready

  RCC->CFGR &= ~RCC_CFGR_SW;                               // Select PLL as system clock source
  RCC->CFGR |=  RCC_CFGR_SW_PLL;
  while ((RCC->CFGR & RCC_CFGR_SWS) != RCC_CFGR_SWS_PLL);  // Wait till PLL is system clock src
}
	
	
int main()
	{

	SystemCoreClockUpdate();
	SysTick_Config(SystemCoreClock / 1000);                  // SysTick 1 msec interrupts

	RCC->APB2ENR |= (1UL << 2);                /* Enable GPIOA clock 				 */
	RCC->APB2ENR |= (1UL << 4);                /* Enable GPIOC clock          */
	RCC->APB1ENR |= (1UL << 1);                /* Enable TIM3 clock          */

	//15.pin |PA1 - SW2 //   14.pin |PA0 - SW3 //  11.pin|C3 - SW4 //   10.pin|C2 - SW5 
	GPIOA->CRL   = 0x00000044;  	// Buton 1-2 SET FLoationg input
	GPIOC->CRH   = 0xFFFFFFF3;   	// BUZZER SET 
	GPIOC->CRL 	 = 0xFFFF33FF;		// C2/C3 4./3.led
	//TIM6->CNT		 = 0x00001000;	//Counter Period 1000 
	TIM3-> PSC		 = 35999;	//Counter Clock Freq(CK_CNT) = Fck_psc(8MHZ) / PSC+1(8KHZ) = 1000 -> Period = 1ms
	TIM3-> ARR		 = 1000;	// period
//	TIM3->EGR |= 1;         // Up Counter
	TIM3->CR1 = 0x1;         // Enable TIM3
	
	while(1){
		
	if(GPIOA->IDR & 0x02){		//PA1
			GPIOC->BSRR  = 0x00000100;	//BUZZER AÇ
			GPIOC->BSRR = 0x08;					//LED 3 AÇ
		if(TIM3->SR & TIM_SR_UIF){
			TIM3->SR &= ~TIM_SR_UIF ; // clear UIF flag
			GPIOC->BSRR  = 0x01000000;
		}
	}	
	else if(GPIOA->IDR & 0x01){	//PA0
		GPIOC->BSRR  = 0x00000100;	//BUZZER AÇ
		GPIOC->BSRR = 0x04;						//LED 4 AÇ
		if(TIM3->SR & TIM_SR_UIF){
			TIM3->SR &= ~TIM_SR_UIF ; // clear UIF flag
			GPIOC->BSRR  = 0x01000000;
			}
		}	
	else{		
		GPIOC->BSRR  = 0x01000000;		//BUZZER KAPAT	
		if(GPIOC->ODR == 0x0C)
			GPIOC->BSRR = 0x000C0000;	//LED'leri kapat
			}
		}
}	
	
/*FlagStatus Readpin(GPIO_TypeDef GPIOx, uint16_t GPIO_Pin)
	{
		FlagStatus flag;
		if ((GPIOx -> IDR & GPIO_Pin) != (uint32_t)RESET)
  {
    flag = SET;
  }
  else
  {
    flag = RESET;
  }
  return flag;
	}*/

/*void delay (int a)
{
    volatile int i,j;
    for (i=0 ; i = a ; i++)
    { j++;  }
    return;
}*/

/*uint8_t GPIO_ReadInputDataBit(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin)
		{
			uint8_t bitstatus = 0x00;
			
			Check the parameters 
			assert_param(IS_GPIO_ALL_PERIPH(GPIOx));
			assert_param(IS_GET_GPIO_PIN(GPIO_Pin)); 
			
			if ((GPIOx->IDR & GPIO_Pin) != (uint32_t)Bit_RESET)
			{
				bitstatus = (uint8_t)Bit_SET;
			}
			else
			{
				bitstatus = (uint8_t)Bit_RESET;
			}
			return bitstatus;
		}		*/
		
		
		/*
RCC->APB1ENR |= (1<<1); // Open clock Timer3
TIM3->PSC = 100;        // Prescale
TIM3->ARR = 1000;       // Autoreload : Counter Resolution  
TIM3->EGR |= 1;         // Up Counter
//  Timer 3 Gen PWM  config
TIM3->CCMR1 |= 0x6060;  // Set PWM channel 1 and PWM Channel 2
TIM3->CCMR2 |= 0x6060;  // Set PWM channel 3 and PWM Channel 4
// Duty Cycle :
TIM3->CCR1 = 750;       
TIM3->CCR2 = 250;       
TIM3->CCR3 = 750;       
TIM3->CCR4 = 100;       
TIM3->CCER |= 0x1111;   // Enable PWM1,PWM2,PWM3,andPWM4
TIM6->CR1 |= 1;         // Enable TIM3

Timer 3 Frequency: F = 84MHz/(Psc+1)(ARR+1)=830,85 Hz, Period= 1,2 ms
Channel 1: Duty Cycle =  75% = 902 us
Channel 2: Duty Cycle = 25% = 300 us
		*/



//volatile uint32_t msTicks;               // counts 1ms timeTicks
///*----------------------------------------------------------------------------
// * SysTick_Handler:
// *----------------------------------------------------------------------------*/
//void SysTick_Handler(void) {
//  msTicks++; 
//}
///*----------------------------------------------------------------------------
// * Delay: delays a number of Systicks
// *----------------------------------------------------------------------------*/
//void Delay (uint32_t dlyTicks) {
//  uint32_t curTicks;

//  curTicks = msTicks;
//  while ((msTicks - curTicks) < dlyTicks) { __NOP(); }
//}
