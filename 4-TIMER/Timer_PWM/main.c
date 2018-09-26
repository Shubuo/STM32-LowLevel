//=============================================================================
// STM32VLDISCOVERY tutorial
/** 	@Lesson 6. Pusle Width Modulation (PWM)	**/
// 	Copyright : Radoslaw Kwiecien
// 	http://en.radzio.dxp.pl
// 	e-mail : radek(at)dxp.pl
/**	@date : 12.07.2018	 **/
/**  	@edit : @BurakYoruk  **/
//=============================================================================
#include "stm32f10x.h"
#include "stm_gpio.h"
//=============================================================================
// Defines
//=============================================================================
#define DELAY     35999
#define DELAY2    1000000
volatile	uint32_t msTicks;

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

		TIM2->PSC = DELAY;	// Set prescaler to 36 000 (PSC + 1)
 		TIM2->ARR = 2000;	// Auto reload value 500 (0.5 sn)

		TIM2->CCR1 = 0;	  // Start PWM duty for channel 3
		TIM2->CCR2 = 1000; // Start PWM duty for channel 4

		TIM2->CCMR1 = TIM_CCMR1_OC2M_2 | TIM_CCMR1_OC2M_1 | 	// 110 2 toggle, 110 1 toggle
						  TIM_CCMR1_OC1M_2 | TIM_CCMR1_OC1M_1; // PWM mode on channel 3 & 4

		TIM2->CCER = TIM_CCER_CC2E | TIM_CCER_CC1E; // Enable compare on channel 3 & 4
		TIM2->CR1  = TIM_CR1_CEN;	// Enable timer

		TIM2->DIER = TIM_DIER_UIE; // Enable update interrupt (timer level)
		NVIC_EnableIRQ(TIM2_IRQn); // Enable interrupt from TIM3 (NVIC level)

while (1) {}
}
//=============================================================================
// TIM2 Interrupt Handler
//=============================================================================
void TIM2_IRQHandler(void)
{
if(TIM2->SR & TIM_SR_UIF) // if UIF flag is set
  {
  TIM2->SR &= ~TIM_SR_UIF;  // clear UIF flag
  TIM2->CCR1 += 100;	        // increase ch1 duty
	  
  if(TIM2->CCR1 == 1000)	    // if maximum
    TIM2->CCR1 = 0;	        // set to zero 
  TIM2->CCR2 -= 100;	        // decrease ch2 duty
  
  if(TIM2->CCR2 == 0)	      // if minimum
    TIM2->CCR2 = 1000;	      // set to maximum
	}
}

//=============================================================================
// End of file
//=============================================================================


/*
		RCC->APB1ENR |= (1<<1); // Open clock Timer3
		TIM3->PSC = 35999;        // Prescale to 1Khz(1ms)
		TIM3->ARR = 1000;       // Autoreload : Counter Resolution  (1sn)
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
		TIM3->CR1 |= 1;         // Enable TIM3

		Timer 3 Frequency: F = 84MHz/(Psc+1)(ARR+1)=830,85 Hz, Period= 1,2 ms
		Channel 1: Duty Cycle =  75% = 902 us
		Channel 2: Duty Cycle = 25% = 300 us
*/

