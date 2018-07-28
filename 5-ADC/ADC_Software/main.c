#include "stm32f10x.h"                  // Device header
#include "stm32f10x_adc.h"              // Keil::Device:StdPeriph Drivers:ADC

int adc_1, adc_2;

unsigned int adc_read(unsigned char channel) 
{ 
 unsigned int i; 
 // ADC blogu açiliyor 
	RCC->APB2ENR	|= (1 << 9);  	//ADC1 Clock Enable 
	RCC->CFGR	 |= RCC_CFGR_ADCPRE_DIV6;  //0x8000 RCC_CFGR_ADCPRE_DIV6 72/6 = 12Mhz max

	ADC1->CR1 |= (0 << 8); // scan mode disable
	//ADC1->SQR1 = 0x100000; // L:0001, 2 conversions oldupu için hiç dokunulmadi
	// POT pini PC0 = ADC12_IN10 (ADC 1 veya 2 CH 10)
	ADC1->SQR3 = 0x0A; //1st Conversion Ch10
	ADC1->SMPR1 = 0x0 | ( 0x07 << channel*3); 	//SMP10 239.5 cycles 
	//12Mhz=83ns, 83*239.5~ 20us rcc bölümlerinden sonra 22 ms.
	
	// Start ADC1 ON, EXTI_Trig ON for Swstart ON,	Continuous mode, 11.bit 0 right allign
 ADC1 -> CR2  =  0x5E0003	 ; 
 ADC1 -> CR1 |= (1UL << 5 );  //EOCIE
 
 if(ADC1->SR & 0x02 )  //while(!(ADC1->DR & 0x8000)) 
 i = ADC1->DR; // Çevrim bilgisi okunuyor 
 
 return (i); // 11.bite kadar 12 bitlik ADC data 
} 

int main(void) 
{ 
	RCC->APB2ENR |= (1UL << 2);   	/* Enable GPIOA clock  */
	RCC->APB2ENR |=  1 <<  4;   		/* Enable GPIOC clock  */
 	RCC->APB1ENR |=  1 <<  29;   		/* Enable DAC clock  */
 	RCC->APB1ENR |=  1 <<  4;   		/* Enable TIM6 clock  */

 GPIOA->CRL   = 0x33;  		//0/1SET 
 GPIOC->CRL   = 0x3304;   //2/3SET, 0 floating input
	unsigned long int sayac = 50000; //(153 de yaniyor)
	
 for(;;) 
 { 
	while (sayac--){}; 
	 sayac = 50000;
 //adc_1=adc_read(1);  //if(adc_1>2048 && adc_2>2048)
 adc_1=adc_read(0); 
 if(adc_1>1862) //2^11, 1.6 volttan büyük ise
	{
	 GPIOA->BSRR  = 0x00000003;		// 0/1 		Ledleri yak 
	 GPIOC->BSRR  = 0x0000000C;		// 2/3 ON 
	}
 else 
		{
	 GPIOA->BSRR  = 0x00030000; // 0/1 Ledleri söndür 
	 GPIOC->BSRR  = 0x000C0000;	// 2/3	OFF
		}	 
	} 
 
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
