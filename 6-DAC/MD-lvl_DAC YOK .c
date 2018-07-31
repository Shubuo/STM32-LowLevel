#include "stm32f10x.h"                  // Device header

int main(){
	RCC->CR |= RCC_CR_HSION;
	RCC->CFGR |= RCC_CFGR_SWS_HSI | RCC_CFGR_SW_HSI ;
	RCC-> APB1ENR |= RCC_APB1ENR_TIM3EN ;


}
/****************************************

		DAC YOK !

********************************************/