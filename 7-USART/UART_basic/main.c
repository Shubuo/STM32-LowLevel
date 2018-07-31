#include "stm32f10x.h"                  // Device header

int main(){
	RCC->APB1ENR |= RCC_APB1ENR_USART2EN;
	RCC->APB2ENR |= RCC_APB2ENR_IOPAEN ;
	
/*******************USART1 Konfigurasyonlari*********************/

// Configure USART Tx as alternate function  /
	GPIOA->CRL |= GPIO_CRL_MODE2 ; // OUTPUT 50Mhz
	GPIOA->CRL |= GPIO_CRL_CNF2_1 ; //AF out PP
	
// Configure USART Rx as alternate function  /
	GPIOA->CRL |= GPIO_CRL_CNF3_1 ; //AF out PP
	GPIOA->CRL |= GPIO_CRL_MODE3 ; // OUTPUT 50Mhz

// Baud rate formula:
// - IntegerDivider = ((PCLKx) / (16 * (USART_InitStruct->USART_BaudRate)))
// - FractionalDivider = ((IntegerDivider - ((u32) IntegerDivider)) * 16) + 0.5 */
	
//  USART BaudRate Calculation for 9600
	USART2->BRR = 0xEA << 4;		//USARTDIV = 234,375 ~ 234,38	
	USART2->BRR |= 0x6 ;			//DIV_Mantissa = 234, DIV_Fraction = 16*0,38 = 6 (nearest real number)
	// BRR = 0xEA6 = 12bit Mantissa + 4bit Fraction
	


	USART2->CR1 = (0 << 12);  	//Wordlength 8 bit, 12.bit
	USART2->CR2 = (0 << 12) | (0 << 13);		// 1-stop bit
	
	USART2->CR1 |= (0 << 10) | (0 << 9) ;    // ParityControlEnable =0 ParitySelection=0 > No parity
	
	USART2->CR3 = (0 << 9) | (0 << 8)	; 		// RTS&CTS Disable
	
	USART2->CR1 |= USART_CR1_TE | USART_CR1_RE ;		//Usart Tx/Rx Enable
		
	USART2->CR1 |= USART_CR1_UE ; // USART2 Enable
}


