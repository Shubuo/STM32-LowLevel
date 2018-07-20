/********************************************************************************
  *
	* @file    Button Kontrol with Timer EXS03 
  * @author  Burak Yörük
  * @date    12-02-2018 <- 05-08-2017
	* @brief   Button control with timer using
  *          
  ******************************************************************************/
	//15.pin |PA1 - SW2 //   14.pin |PA0 - SW3 //  11.pin|C3 - SW4 //   10.pin|C2 - SW5 
#include "stm32f10x.h"
#include <stdio.h>

volatile uint32_t msTicks;               // counts 1ms timeTicks
///*----------------------------------------------------------------------------
// * SysTick_Handler:
// *----------------------------------------------------------------------------*/
//void SysTick_Handler(void) {
//  msTicks++; 
//}
///*----------------------------------------------------------------------------
// * Delay: delays a number of Systicks
// *----------------------------------------------------------------------------*/
//void Delay (uint32_t dlyTicks) 
//	{
//  uint32_t curTicks;

//  curTicks = msTicks;
//  while ((msTicks - curTicks) < dlyTicks) { __NOP(); }
//	}

int main()
	{
//	int N;
//		printf("Main starts");

	RCC->APB2ENR |= (1UL << 2);                /* Enable GPIOA clock 				 */
	RCC->APB2ENR |= (1UL << 4);                /* Enable GPIOC clock          */
	RCC->APB1ENR |= (1UL << 1);                /* Enable TIM3 clock          */

	GPIOA->CRL   = 0x44;  	// Buton 0-1 SET Floationg input
	GPIOC->CRH   = 0x03;   	// BUZZER SET 
	GPIOC->CRL 	 = 0x3300;		// C2/C3 4./3.led
		
	//TIM6->CNT		 = 0x00001000;	//Counter Period 1000 
	TIM3-> PSC		 = 35999;	//Counter Clock Freq(CK_CNT) = Fck_psc(8MHZ) / PSC+1(8KHZ) = 1000 -> Period = 1ms
	TIM3-> ARR		 = 1000;	// period
//	TIM3->EGR |= 1;         // Up Counter
	TIM3->CR1 = 0x1;         // Enable TIM3
	
	while(1){	
//		scanf("%d", &N);
//		printf("%i", N);
	if(GPIOA->IDR & 0x02){		//PA1
			GPIOC->BSRR  = 0x00000100;	//BUZZER AÇ
			GPIOC->BRR = 0x08;					//LED 3 OFF
		while(!(TIM3->SR & TIM_SR_UIF)){
			TIM3->SR &= ~TIM_SR_UIF ; // clear UIF flag
			GPIOC->BSRR  = 0x01000000;
		}
	}
	else if(GPIOA->IDR & 0x01){	//PA0
		GPIOC->BSRR  = 0x00000100;	//BUZZER AÇ
		GPIOC->BRR = 0x4; //LED 4 OFF
		if(TIM3->SR & TIM_SR_UIF){
			TIM3->SR &= ~TIM_SR_UIF ; // clear UIF flag
			GPIOC->BSRR  = 0x01000000;
				}
			}	
	else {
		GPIOC->ODR = 0xC;	//LED 4/3 ON
		GPIOC->BSRR  = 0x01000000;		//BUZZER KAPAT
			}
		};
}	



		


//=============================================================================
// Some Delay Functions
//=============================================================================

//volatile uint32_t msTicks;               // counts 1ms timeTicks
///*----------------------------------------------------------------------------
// * SysTick_Handler:
// *----------------------------------------------------------------------------*/
//void SysTick_Handler(void) {
//  msTicks++; 
//}
/*----------------------------------------------------------------------------
 * Delay: delays a number of Systicks
 *----------------------------------------------------------------------------*/
//void Delay (uint32_t dlyTicks) {
//  uint32_t curTicks;

//  curTicks = msTicks;
//  while ((msTicks - curTicks) < dlyTicks) { __NOP(); }
//}
/*void delay (int a)
{
    volatile int i,j;
    for (i=0 ; i = a ; i++)
    { j++;  }
    return;
}*/
