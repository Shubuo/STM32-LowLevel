//=============================================================================
// STM32F103R8 tutorial
/** 	@Lesson 5. Blinking with timer hardware  **/
// 		Copyright : Radoslaw Kwiecien
// 		http://en.radzio.dxp.pl
/**		@date : 11.07.2018	 **/
/**  	@edit : @BurakYoruk  **/
//=============================================================================
#include "stm32f10x.h"

//=============================================================================
// Defines
//=============================================================================
#define DELAY     35999
//=============================================================================
// main function
//=============================================================================
int main(void)
{
		RCC->APB2ENR = RCC_APB2ENR_AFIOEN | RCC_APB2ENR_IOPAEN | RCC_APB2ENR_IOPCEN ; //0x15; 
		/* Enable GPIOA/C/ AFIOEN clock 				 */
		RCC->APB1ENR = RCC_APB1ENR_TIM3EN ; //0x02 /* Enable TIM3 */		TIM6 kapali gözüküyor
		GPIOC->CRL = 0xAA00;							 	//AFIO Push-Pull 2Mhz for PC2/PC3

		AFIO->MAPR = AFIO_MAPR_TIM3_REMAP; // Full TIM3 remap

		TIM3->PSC  = DELAY;		// Set prescaler to 36 000 (PSC + 1)
		TIM3->ARR  = 1000;	  // Auto reload value 500
		TIM3->CCR3 = 250;	  // Compare 3 with 250
		TIM3->CCR4 = 1000;   // Compare 4 with 500

		TIM3->CCMR2 = TIM_CCMR2_OC4M_0 | TIM_CCMR2_OC4M_1 | // 0011 0000 0011 0000 (011 4 toggle, 011 3 toggle)
									TIM_CCMR2_OC3M_0 | TIM_CCMR2_OC3M_1; // Toggle output 3 & 4 on compare match 

		TIM3->CCER  = TIM_CCER_CC4E | TIM_CCER_CC3E; // Enable compare output 3 & 4
		TIM3->CR1   = TIM_CR1_CEN;	// Enable timer

while (1) {}
}
//=============================================================================
// End of file
//=============================================================================