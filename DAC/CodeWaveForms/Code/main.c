#include "stm32f10x.h"                  // Device header


void Timer2_init(void) {
  RCC->APB1ENR |= 0x0001;       // Enable clock for Timer 2
  
  TIM2->ARR = 84;         // Auto Reload value: 840 => 100 KHz  
  TIM2->CR2    |= 0x0020;       // select TRGO to be update event (UE)
  TIM2->CR1    |= 0x0001;       // Enable Counting
}

int main()
{
 
	Timer2_init();
		
	
	//DAC Clock Enable

	RCC->APB1ENR|=1<<29;


	//GPIOA Clock Enable
	RCC->APB2ENR |=  4;

	//GPIOA Pin 4 et 5 Analog Config
	GPIOA->CRL = 0xFFF00FFF; //GPIOA->CRL	|=(0x3)<<16	GPIOA->CRL	|=(0x3)<<20;

	 //Trigger Selection :Software Trigger
	DAC->CR|=((0x4)<<3); 
	DAC->CR|=((0x4)<<19);
	
	//Trigger ENable 1 et Trigger Enable 2
	DAC->CR|=(1<<2)+(1<<18);

	//WAVE 1 et WAVE 2 Config : Triangle Wave
	DAC->CR|=((0x1)<<6)+((0x1)<<22);

	//Config Amplitude

	DAC->CR|=((0xA)<<8);  // digital Amplitude 2045; Analog amplitude 1.6483 V
	DAC->CR|=((0xD)<<24); // digital Amplitude 4095; Analog amplitude 3.3 V

	//DAC ENABLE
	  DAC->CR|=(1<<0)+(1<<16);
	
	
	
	while(1)
	{	//DAC Software Trig
		DAC->SWTRIGR|=0x3;
	}
	

}
