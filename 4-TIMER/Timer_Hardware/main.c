//=============================================================================
// STM32F103R8 tutorial
/** 	@Lesson 5. Blinking with timer hardware  **/
// 	Copyright : Radoslaw Kwiecien
// 	http://en.radzio.dxp.pl
/**	@date : 11.07.2018	 **/
/**  	@edit : @BurakYoruk  **/
//=============================================================================
#include "stm32f10x.h"
#include "stm_gpio.h"
//=============================================================================
// Defines
//=============================================================================
#define DELAY     35999
//=============================================================================
// main function
//=============================================================================
int main(void)
{
		RCC->APB2ENR =  RCC_APB2ENR_IOPAEN | RCC_APB2ENR_IOPCEN ; //0x15; 
		/* Enable GPIOA/C clock 				 */
		RCC->APB1ENR = RCC_APB1ENR_TIM2EN ; 	 /* Enable TIM2 */
//		GPIOA->CRL = 0xAA;							//AFIO Push-Pull 2Mhz for PA0/PA1
		GPIOA->CRL = (GPIOA->CRL & CONFMASKL(0)) | 
					GPIOPINCONFL(0, GPIOCONF(GPIO_MODE_OUTPUT2MHz, GPIO_CNF_AFIO_PUSHPULL)); //PA0 = 0x4444444A
		GPIOA->CRL = (GPIOA->CRL & CONFMASKL(1)) | 
					GPIOPINCONFL(1, GPIOCONF(GPIO_MODE_OUTPUT2MHz, GPIO_CNF_AFIO_PUSHPULL));

	//bende yok onlara bagli pin
//		AFIO->MAPR = AFIO_MAPR_TIM3_REMAP; // Full TIM3 remap
	
		TIM2->PSC  = DELAY;		// Set prescaler to 36 000 (PSC + 1)
		TIM2->ARR  = 1000;	  // Auto reload value 500
		TIM2->CCR1 = 250;	   // Compare 3 with 250
		TIM2->CCR2 = 1000;   // Compare 4 with 500

		TIM2->CCMR1 = TIM_CCMR1_OC2M_0 | TIM_CCMR1_OC2M_1 | // 0011 0000 0011 0000 (011 2 toggle, 011 1 toggle)
						  TIM_CCMR1_OC1M_0 | TIM_CCMR1_OC1M_1;  // Toggle output 1 & 2 on compare match 

		TIM2->CCER  = TIM_CCER_CC2E | TIM_CCER_CC1E; // Enable compare output 1 & 2
		TIM2->CR1   = TIM_CR1_CEN;	// Enable timer

while (1) {}
}
//=============================================================================
// End of file
//=============================================================================

//TIMER1->regs.gen->CCMR1 |= 0x0001; // Ch. 1 as TI1
//TIMER1->regs.gen->SMCR |= 0x0007; // Ext. clk mode 1
//TIMER1->regs.gen->SMCR |= 0x0050; // TI1FP1 is the trigger
//TIMER1->regs.gen->CR1 |= 0x0001; // enable counting 