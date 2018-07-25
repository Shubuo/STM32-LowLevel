#include "stm32f10x.h"                  // Device header
//#include <stdio.h>
 

unsigned temp;

void ADC1_2_IRQHandler(void)
{
   
	 if(ADC1->SR & 0x00000002)
	{
		ADC1->SR=0; // adc bayragini temizle
	        temp =ADC1->DR;
	}
        
	
}
int main(void)
{
	
	RCC->APB2ENR |=  1 <<  4;   		/* Enable GPIOC clock  */
	RCC->APB2ENR |= 0x00000004; // IO port A clock enable
        GPIOA->CRL &= ~0x000000C0;	// 14MHz PA.1 Output
	GPIOA->CRL   |= 0x33;  		//0/1SET 
	GPIOC->CRL   = 0x3304;   //2/3SET, 0 floating input
	
	RCC->CFGR |= 0x0000C000; //ADCPRE: ADC prescaler /11: PCLK2 divided by 8
	RCC->APB2ENR |= 0x00000200 ; //ADC1EN: ADC 1 interface clock enable     
	NVIC->ISER[0] |= 0x00040000; 
	ADC1->SMPR1 =0x07 ;//001: 7.5 cycles CH10
	ADC1->CR1 = 0x00000020;//Bit 5 EOCIE: Interrupt enable for EOC
	
	
	//111: SWSTART //1: Enable ADC and to start conversion//1: Conversion on external event enabled //1: Continuous conversion mode
	ADC1->CR2 = (1<<17) |(1<<18) |(1<<19) | (1<<0) | (1<<20) | (1<<1); 
	
	ADC1->SQR1 = 0x00000000;
	ADC1->SQR2 = 0x00000000;
	ADC1->SQR3 = 0x00000001; //1st conversion in regular sequence
	
 
	ADC1->CR2 |= 0x00400000;//1: Starts conversion of regular channels  //swstart
	
			if(ADC1->DR > 1862) //2^11, 1.6 volttan büyük ise
	{
	 GPIOA->BSRR  = 0x00000003;		// 0/1 		Ledleri yak 
	 GPIOC->BSRR  = 0x0000000C;		// 2/3 ON 
	}
 else 
		{
	 GPIOA->BSRR  = 0x00030000; // 0/1 Ledleri söndür 
	 GPIOC->BSRR  = 0x000C0000;	// 2/3	OFF
		}	
		
	while(1) 	{	}
}


//#include "stm32f10x.h"                  // Device header
////#include <stdio.h>
// 

//unsigned temp;

//void ADC1_2_IRQHandler(void)
//{
//   
//	 if(ADC1->SR & 0x00000002)
//	{
//		ADC1->SR=0; // adc bayragini temizle
//	        temp =ADC1->DR;
//	}
//        
//	
//}
//int main(void)
//{
//	
//	
//	+ RCC->APB2ENR |= 0x00000004; // IO port A clock enable   
//  +      GPIOA->CRL &= ~0x000000C0;	// 14MHz
//	+ RCC->CFGR |= 0x0000C000; //ADCPRE: ADC prescaler /11: PCLK2 divided by 8
//	+ RCC->APB2ENR |= 0x00000200 ; //ADC1EN: ADC 1 interface clock enable     
//	- NVIC->ISER[0] |= 0x00040000; 
//	+ ADC1->SMPR2 =0x00000008;//001: 7.5 cycles
//	+ ADC1->CR1 = 0x00000020;//Bit 5 EOCIE: Interrupt enable for EOC
//	
//	
//	//111: SWSTART //1: Enable ADC and to start conversion//1: Conversion on external event enabled //1: Continuous conversion mode
//	ADC1->CR2 = (1<<17) |(1<<18) |(1<<19) | (1<<0) | (1<<20) | (1<<1); 
//	
//	ADC1->SQR1 = 0x00000000;
//	ADC1->SQR2 = 0x00000000;
//	ADC1->SQR3 = 0x00000001; //1st conversion in regular sequence
//	
// 
//	ADC1->CR2 |= 0x00400000;//1: Starts conversion of regular channels
//	while(1)
//	{
//		
//		    
//	}
//}
