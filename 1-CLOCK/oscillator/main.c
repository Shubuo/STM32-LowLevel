#include "stm32f10x.h"                  // Device header


//void RCC_HSICmd(FunctionalState NewState)

int main()
{
	//1-Register Seç 
	//2-System Clock Source Seç
	//3-HSION enable et
	
	void RCC_DeInit(void);
			/* Set HSION bit */
			//RCC->CR |= (uint32_t)0x00000001;
	RCC_SYSCLKConfig(RCC_SYSCLKSource_HSI);
			//RCC_AdjustHSICalibrationValue(RCC_CR_HSION);
	RCC_HSICmd(ENABLE);
}

/*
#define  RCC_CFGR_SWS_HSI  - HSI oscillator used as system clock

#define PERIPH_BB_BASE        ((uint32_t)0x42000000)   !< Peripheral base address in the bit-band region 

#define CR_OFFSET                 (RCC_OFFSET + 0x00)
#define RCC_OFFSET                (RCC_BASE - PERIPH_BASE)
#define RCC_BASE              (AHBPERIPH_BASE + 0x1000)
#define AHBPERIPH_BASE        (PERIPH_BASE + 0x20000)
#define PERIPH_BASE           ((uint32_t)0x40000000)   !< Peripheral base address in the alias region 

*/
/*
	GPIO_InitTypeDef GPIO_InitStruct;
	    // GPIOA-GPIOC Periph clock enable 
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD, ENABLE);
	
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IN_FLOATING ;
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_12 | GPIO_Pin_13| GPIO_Pin_14 | GPIO_Pin_15 ;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_2MHz;
		GPIO_Init(GPIOD, &GPIO_InitStruct);
	
	GPIO_SetBits(GPIOD, GPIO_Pin_12);
	*/ 
	