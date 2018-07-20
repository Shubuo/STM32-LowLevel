/********************************************************************************
  *
	* @file    Timer Led Kontrol EXS03 
  * @author  Burak Yörük
  * @date    05-08-2017 -> 04.07.2018
	* @brief   This file for understanding timer functions
  *          
  ******************************************************************************/
//=============================================================================
#include "stm32f10x.h"
//=============================================================================
// Defines
//=============================================================================
#define DELAY     35999
#define DELAY2    1000000


volatile	uint32_t msTicks;
	
//	void Delay(uint32_t dlyTicks){
//		uint32_t curTicks;
//		curTicks = msTicks;
//		while((curTicks - msTicks) < dlyTicks);
//	}
//	void SysTick_Handler(void){
//		msTicks++;
//	}

//=============================================================================
// main function
//============================================================================
int main()
	{	
//		SystemCoreClockUpdate();
//		SysTick_Config(SystemCoreClock / 1000);
		
//		RCC->APB2ENR |= (1UL << 2);          /* Enable GPIOA clock 				 */
		RCC->APB2ENR = 0x14;                /* Enable GPIOA/C clock 				 */
		RCC->APB1ENR = 0x12;								/* Enable TIM3/TIM6 */		
		GPIOA->CRL = 0x44 ;  								//Input floating for PA0/PA1
		GPIOC->CRL = 0x3300;								//Output 50Mhz 	 for PC2/PC3
		
		TIM3->PSC = DELAY;
//		TIM3->SR  = 0	;
		TIM3->ARR = 1000;			
		TIM3->CR1 |= TIM_CR1_CEN;
		TIM3->DIER = TIM_DIER_UIE; // Enable update interrupt (timer level)
		
		NVIC_EnableIRQ(TIM3_IRQn); // Enable interrupt from TIM3 (NVIC level)
		
		while (1){
			GPIOC->ODR = 0x4;	//LED 4 ON
			for(msTicks = 0; msTicks < DELAY2; msTicks++);
			GPIOC->BRR = 0x4; //LED 4 OFF
			for(msTicks = 0; msTicks < DELAY2; msTicks++);


//		GPIOC->ODR = 0;
////		TIM3->SR  = 0	;
////		TIM3->PSC = DELAY;
////		TIM3->ARR = 1000;			
////		TIM3->CR1 |= TIM_CR1_CEN;
//	while (!(TIM3->SR & TIM_SR_UIF));
		};
}		

//=============================================================================
// TIM3 Interrupt Handler
//=============================================================================
void TIM3_IRQHandler(void)
{
if(TIM3->SR & TIM_SR_UIF) // if UIF flag is set
  {
  TIM3->SR &= ~TIM_SR_UIF; // clear UIF flag
		if(GPIOA->IDR & 0x2)
  GPIOC->ODR = 0x8; // toggle LED 3 state
		
  }
}
//=============================================================================
// Timer Delay Function
//=============================================================================
//void delay_ms(register unsigned short n) 
//{
//   if (n > 1) n--;
//   TIM3->PSC = 35999;   // Set prescaler to 24,000 (PSC + 1)
//   TIM3->ARR = n;       // n = 1 gives 2msec delay rest are ok
////   TIM3->CNT = 0;
////   TIM3->EGR = TIM_EGR_UG;  // copy values into actual registers!
////                            // Enable timer in one pulse mode
//   TIM3->CR1 |= (TIM_CR1_OPM | TIM_CR1_CEN);
//   while (TIM3->CR1 & TIM_CR1_CEN); // wait for it to switch off
//}

//=============================================================================
// End of Program
//=============================================================================



//	TIM3-> PSC	= 35999;			  
//// Counter Clock Freq(CK_CNT) also means "update event" 
//// = Fck_psc(36MHz) / PSC+1(36KHz) = 1000Hz -> Period = 1ms
//	TIM3-> ARR	= 1000;			    // 1ms*1000 = 1sn
//  TIM3->CR1 = TIM_CR1_CEN;    /* enable */		

//		while (1){
//			if(TIM3->SR & TIM_SR_UIF) // if UIF flag is set
//		{
//			TIM3->SR &= ~TIM_SR_UIF ; // clear UIF flag
//			GPIOA->ODR = ~GPIOA->ODR;	//GPIOA- 8 AÇ  /* The required time delay has been elapsed */
//			};
//		};		


//=============================================================================
// /* 	Simple Delay	*/
//=============================================================================
//	uint32_t dly_cnt = 0;
//	void SysTick_Handler(){
//		if(dly_cnt>0)
//			dly_cnt--;
//	}
//	void delay(__IO uint32_t time){
//	dly_cnt = time;
//	while(dly_cnt){};
//}


//int counter = 0;
//while (1) {
////	counter ++;			
////		if(counter == 0x16E360) {
////			counter = 0;
//	};

