//=============================================================================
// STM32VLDISCOVERY tutorial
/** 	@Lesson 6. Pusle Width Modulation (PWM)	**/
// Copyright : Radoslaw Kwiecien
// http://en.radzio.dxp.pl
// e-mail : radek(at)dxp.pl
/**		@date : 12.07.2018	 **/
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
		GPIOC->CRL = 0xAA00;								//AFIO Push-Pull 2Mhz for PC2/PC3

		AFIO->MAPR = AFIO_MAPR_TIM3_REMAP; // Full TIM3 remap

		TIM3->PSC = DELAY;	// Set prescaler to 36 000 (PSC + 1)
		TIM3->ARR = 500;	// Auto reload value 500 (0.5 sn)

		TIM3->CCR3 = 0;	  // Start PWM duty for channel 3
		TIM3->CCR4 = 500; // Start PWM duty for channel 4

		TIM3->CCMR2 = TIM_CCMR2_OC4M_2 | TIM_CCMR2_OC4M_1 | 
									TIM_CCMR2_OC3M_2 | TIM_CCMR2_OC3M_1; // PWM mode on channel 3 & 4

		TIM3->CCER = TIM_CCER_CC4E | TIM_CCER_CC3E; // Enable compare on channel 3 & 4
		TIM3->CR1  = TIM_CR1_CEN;	// Enable timer

		TIM3->DIER = TIM_DIER_UIE; // Enable update interrupt (timer level)
		NVIC_EnableIRQ(TIM3_IRQn); // Enable interrupt from TIM3 (NVIC level)

while (1) {}
}
//=============================================================================
// TIM3 Interrupt Handler
//=============================================================================
void TIM3_IRQHandler(void)
{
if(TIM3->SR & TIM_SR_UIF) // if UIF flag is set
  {
  TIM3->SR &= ~TIM_SR_UIF;  // clear UIF flag
  TIM3->CCR3 += 5;	         // increase ch3 duty
  if(TIM3->CCR3 == 500)	    // if maximum
    TIM3->CCR3 = 0;	        // set to zero 
  TIM3->CCR4 -= 5;	         // decrease ch4 duty
  if(TIM3->CCR4 == 0)	      // if minimum
    TIM3->CCR4 = 500;	      // set to maximum
	}
}

//=============================================================================
// End of file
//=============================================================================
