#include "stm32f10x.h"                  // Device header

int main()
	
{
	int j=0;
	RCC->APB1ENR |= (1<<29); // activer DAC
	
	RCC->APB2ENR |= (1<<2);  // activer GPIO A
	
	GPIOA->CRL = 0xFFF00FFF;	//(3<<2*4)+(3<<2*5); // GPIO A PIn 4 et 5 analog ?
	
	DAC->CR |= (1<<0)+ (1<<16);  // DAC Channel 1|2 active 
	
	while(1)
	{
		
  DAC->DHR12R1 = j&0xfff;  						//channel 1
	DAC->DHR12R2 = 0xfff - (j&0xfff);		//channel 2
	j=(j+1)&0xfff;
	}
	
	
}
