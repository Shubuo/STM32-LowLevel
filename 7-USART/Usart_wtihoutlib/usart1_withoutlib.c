/*********************************
*
*		USART Led Yakma uygulamasi
*
**********************************/

#include "stm32f10x.h"                  // Device header
#include <stdio.h>



void usartSetup (void) {
  // make sure the relevant pins are appropriately set up.
  RCC->APB2ENR |= RCC_APB2ENR_IOPAEN;              // enable clock for GPIOA
  GPIOA->CRH   = (0x0BUL  << 4);                  // Tx (PA9) alt. out push-pull
  GPIOA->CRH   |= (0x04UL  << 8);                  // Rx (PA10) in floating
  RCC->APB2ENR |= RCC_APB2ENR_USART1EN;            // enable clock for USART1
  USART1->BRR  = 0x1D4C; //72000000L/9600L;                // set baudrate
  USART1->CR1 |= (USART_CR1_RE | USART_CR1_TE);  	// RX, TX enable
  USART1->CR1 |= USART_CR1_UE;                    // USART enable
  }

int SendData (char ch)  {
  while (!(USART1->SR & USART_SR_TXE));
  USART1->DR = (ch);
  return (ch);
}
void Usart_puts(volatile char *s){
	while(*s){
		while(!(USART1->SR	& USART_SR_TC	));
		SendData(*s++);
	}	
}

char GetChar (void)  {
  while (!(USART1->SR & USART_SR_RXNE));
  return ((USART1->DR));
}



int main () {
	
	usartSetup();
	
	GPIOA->CRL = GPIO_CRL_MODE0 | (GPIO_CRL_CNF0&0x0) ; 
 
	char str[50], key;
	uint32_t i;
	
	while(1){
		
//	  while (!(USART1->CR1 & USART_CR1_RXNEIE));
		key = GetChar();
			if( key == 'l')
			{
				SendData(key);
				sprintf(str, "\rLed'i actiniz\r");
				Usart_puts(str);
				GPIOA->ODR	|= GPIO_ODR_ODR0; 
			}
			else
			{
				SendData(key);
				i = 5000000;
				while(i) i--;
				sprintf(str, "\rLed'i kapadiniz\r");
				Usart_puts(str);
				GPIOA->BRR	|= GPIO_BRR_BR0; 

			}
		 i = 5000000;
		 while(i) i--;
		
	}	 
	
}

