#include "stm32f10x.h"                  // Device header
#include <stdio.h>

char str[50];
uint32_t i;

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
  USART1->DR = (ch & 0xFF);
  return (ch);
}
void Usart_puts(volatile char *s){
	while(*s){
		while(!(USART1->SR	& USART_SR_TC	));
		SendData(*s++);
	}	
}

int GetChar (void)  {
  while (!(USART1->SR & USART_SR_RXNE));
  return ((int)(USART1->DR & 0xFF));
}

int main () {

 usartSetup();
 
	while(1){
	sprintf(str, "Lorem Ipsum Dolor Sit Amet\r");
	Usart_puts(str);
	 i = 5000000;
	 while(i) i--;
	}	 
	

}
