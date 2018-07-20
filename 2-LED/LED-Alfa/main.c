/********************************************************************************
  *
	* @file    Karasimsek EXS03 
  * @author  Burak Yörük
  * @date    29-June-2017
  * @brief   This file contains all the functions prototypes for the RCC firmware 
  *          
  ******************************************************************************/

#include "stm32f10x.h"

#define LED_NUM 				 (1)

const unsigned long led_mask[] = {1ul << 5};
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
void Delay (uint32_t dlyTicks) {
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
	RCC->CR |= ((uint32_t)RCC_CR_HSION);                     // Enable HSI
  while ((RCC->CR & RCC_CR_HSIRDY) == 0);                  // Wait for HSI Ready

  RCC->CFGR = RCC_CFGR_SW_HSI;                             // HSI is system clock
  while ((RCC->CFGR & RCC_CFGR_SWS) != RCC_CFGR_SWS_HSI);  // Wait for HSI used as system clock

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

while(1){
	RCC->APB2ENR |= (1UL << 2);                /* Enable GPIOA clock 				 */
	RCC->APB2ENR |=  1 <<  4;                    /* Enable GPIOC clock          */
	//15.pin |PA1 - Led4 //   14.pin |PA0 - Led3   //  11.pin|C3 - Led2 //   10.pin|C2 - Led1 
	GPIOA->CRL   = 0xFFFFFF3F;  //1SET               /* Configure the GPIO for LEDs  | GPIOA = port A -> 1    */
	GPIOA->BSRR  = 0x00000002;	//1AÇ
	Delay(100);
	GPIOA->CRL   = 0xFFFFFFF3;  //2SET  /* Configure the GPIO for LEDs  | GPIOA = port A -> 0 */
	GPIOA->BSRR  = 0x00000001;	//2AÇ
	GPIOA->BSRR  = 0x00020000;	//1KAPAT
	Delay(100);
	GPIOC->CRL   = 0xFFFF3FFF;  //3SET              /* Configure the GPIO for LEDs  | GPIOC = port C -> 3    */
	GPIOC->BSRR  = 0x00000008;	//3AÇ 
	GPIOA->BSRR  = 0x00010000;	//2KAPAT 
	Delay(100);
	GPIOC->CRL   = 0xFFFFF3FF;  //4SET              /* Configure the GPIO for LEDs  | GPIOC = port C -> 2    */
	GPIOC->BSRR  = 0x00000004;	//4AÇ
	GPIOC->BSRR  = 0x00080000;	//3KAPAT
	Delay(100);
	GPIOC->CRL   = 0xFFFF3FFF;  //3SET              /* Configure the GPIO for LEDs  | GPIOC = port C -> 3    */
	GPIOC->BSRR  = 0x00000008;	//3AÇ 
	GPIOC->BSRR  = 0x00040000;	//4KAPAT
	Delay(100);
	GPIOA->CRL   = 0xFFFFFFF3;  //2SET  /* Configure the GPIO for LEDs  | GPIOA = port A -> 0 */
	GPIOA->BSRR  = 0x00000001;	//2AÇ
	GPIOC->BSRR  = 0x00080000;	//3KAPAT
	Delay(100);
	GPIOA->CRL   = 0xFFFFFF3F;  //1SET               /* Configure the GPIO for LEDs  | GPIOA = port A -> 1    */
	GPIOA->BSRR  = 0x00000002;	//1AÇ
	GPIOA->BSRR  = 0x00010000;	//2KAPAT 

}	
	
//  #define GPIO_Pin_0                 ((uint16_t)0x0001)  /*!< Pin 0 selected */
//	#define GPIO_Speed_50MHz
//	#define GPIO_Mode_Out_PP 0x10
		
//  GPIOC->CRH   &= 0xFFFFFF00;                  /* Configure the GPIO for LEDs */
//  GPIOC->CRH   |= 0x00000033;                  /* Configure the GPIO for LEDs */
//	//	-------------------
//	 RCC->APB2ENR |= (1UL << 2);                /* Enable GPIOA clock            */
//	//  /* Configure LED (PA.5) pins as output */
//	 GPIOA->CRL   &= ~((15ul << 4*5));
//	 GPIOA->CRL   |=  (( 1ul << 4*5));
		
		
//	RCC->APB2ENR |= (1ul << 4);                /* Enable GPIOC clock       ul(unsigned long)     */
//	GPIOC->CRH &= ~(15ul << 4*(13-8));
//  GPIOC->CRH |=  ( 4ul << 4*(13-8));
		
		
//		#ifdef __DBG_ITM
//  ITM_RxBuffer = ITM_RXBUFFER_EMPTY;       /*  CMSIS Debug Input              */
//#else
//  RCC->APB2ENR |=  (   1ul <<  2);         /* Enable GPIOA clock              */
//  RCC->APB1ENR |=  (   1ul << 17);         /* Enable USART#2 clock            */

//  /* Configure PA3 to USART2_RX, PA2 to USART2_TX */
//  RCC->APB2ENR |=  (   1ul <<  0);         /* enable clock Alternate Function */
//  AFIO->MAPR   &= ~(   1ul <<  3);         /* clear USART2 remap              */

//  GPIOA->CRL   &= ~(0xFFul <<  8);         /* clear PA2, PA3                  */
//  GPIOA->CRL   |=  (0x0Bul <<  8);         /* USART2 Tx (PA2) output push-pull*/
//  GPIOA->CRL   |=  (0x04ul << 12);         /* USART2 Rx (PA3) input floating  */

//  USARTx->BRR  = __USART_BRR(24000000ul, 115200ul);  /* 115200 baud @ 24MHz   */
//  USARTx->CR3   = 0x0000;                  /* no flow control                 */
//  USARTx->CR2   = 0x0000;                  /* 1 stop bit                      */
//  USARTx->CR1   = ((   1ul <<  2) |        /* enable RX                       */
//                   (   1ul <<  3) |        /* enable TX                       */
//                   (   0ul << 12) |        /* 1 start bit, 8 data bits        */
//                   (   1ul << 13) );       /* enable USART                    */
//#endif


//	GPIOC->BSRR |= (led_mask[num]);
//	GPIOC->BSRR |= (led_mask[num] << 16);
//	
	//GPIOA->BSRR |= (led_mask[num] << 16);
	
while(1);
}
	


/*

 for (;;) {
	RCC->APB2ENR |= (1UL << 2);                /* Enable GPIOA clock 				 
	//GPIOA->ODR = 0x00000000;
	RCC->APB2ENR |=  1 <<  4;                    /* Enable GPIOC clock          
	//15.pin |PA1 - Led1   //   14.pin|PA0 - Led2   //   11.pin|C3 - Led3   //   10.pin|C2 - Led4
	//GPIOA = port A -> 0  //Ledin oldugu 14.pini pa0 pini olarak tanimladik
//Led1
	GPIOA->CRL   = 0xFFFFFF3F;                  /* Configure the GPIO for LEDs  | GPIOA = port A -> 1    
	GPIOA->BSRR  = 0x00000002;
	Delay(50);
	GPIOA->BSRR  = 0x00020000;
	Delay(50);
//Led2 
	GPIOA->CRL   = 0xFFFFFFF3;                  /* Configure the GPIO for LEDs  |     
	GPIOA->BSRR  = 0x00000001;
	Delay(50);
	GPIOA->BSRR  = 0x00010000;
	Delay(50);
// Led3 
	GPIOC->CRL   = 0xFFFF3FFF;                  /* Configure the GPIO for LEDs  | GPIOA = port C -> 3    
	GPIOC->BSRR  = 0x00000008;
	Delay(50);
	GPIOC->BSRR  = 0x00080000;
	Delay(50);
//Led4 
	GPIOC->CRL   = 0xFFFFF3FF;                  /* Configure the GPIO for LEDs  | GPIOA = port C -> 2    
	GPIOC->BSRR  = 0x00000004;
	Delay(50);
	GPIOC->BSRR  = 0x00040000;
	Delay(50);	
//Led3 
	GPIOC->CRL   = 0xFFFF3FFF;                  /* Configure the GPIO for LEDs  | GPIOA = port C -> 3    
	GPIOC->BSRR  = 0x00000008;
	Delay(50);
	GPIOC->BSRR  = 0x00080000;
	Delay(50);
//Led2 
	GPIOA->CRL   = 0xFFFFFFF3;                  /* Configure the GPIO for LEDs  | GPIOA = port A -> 0     
	GPIOA->BSRR  = 0x00000001;
	Delay(50);
	GPIOA->BSRR  = 0x00010000;
	Delay(50);
}
*/