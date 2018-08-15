#include "stm32f10x.h"                  // Device header


int main()
{
//  unsigned char ch = 0;
	while(1){
	RCC->APB2ENR |= RCC_APB2ENR_IOPCEN;
	GPIOC->CRL 	= 0x3300;							//GPIO_CRL_MODE3 | (GPIO_CRL_CNF3&0x0); pc3
  GPIOC->ODR	|= GPIO_ODR_ODR3;
	GPIOC->ODR |= GPIO_ODR_ODR2;
	 int i = 5000000;
	while(i>1){i --; }                  // delay ~yarım saniye falan
	i=5000000;
	GPIOC->BSRR	|= GPIO_BSRR_BR3;
	GPIOC->BSRR	|= GPIO_BSRR_BR2;

	while(i>1){i --; }  
	}
}
