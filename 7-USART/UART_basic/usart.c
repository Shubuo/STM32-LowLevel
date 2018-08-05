#include "stm32f10x.h"                  // Device header
#include "usart.h"

void usart_init(void)
{
//	float div;
	
	RCC->APB1ENR |= RCC_APB1ENR_USART2EN;
	RCC->APB2ENR |= RCC_APB2ENR_IOPAEN ;
	
	USART2->CR1 |= (USART2_M << 12) ; 	//Wordlength 8 bit, 12.bit
	// ParityControlEnable =0 ParitySelection=0 > No parity	
	USART2->CR1 |= (USART2_PCE << 10)| (USART2_PS << 9)  ;								
	USART2->CR2 |= (USART2_STOP << 12) | (USART2_STOP << 13);		// 1-stop bit
	
	//div = USART2_PCLK1 / (16*USART2_BAUD_RATE) ;	//USARTDIV = 234,375 ~ 234,38
	USART2->BRR |= 0xEA6 ;
	
// Configure USART Tx as alternate function  / 1011
	GPIOA->CRL |= GPIO_CRL_MODE2 ; // OUTPUT 50Mhz 
	GPIOA->CRL |= GPIO_CRL_CNF2_1 ; //AF out PP	
	
// Configure USART Rx as alternate function  / 0100
	GPIOA->CRL |= GPIO_CRL_CNF3_0 ; //Floating Input Reset


	USART2->CR3 |= (0 << 9) | (0 << 8)	; 		// RTS&CTS Disable
	USART2->CR1 |= USART_CR1_TE | USART_CR1_RE ;		//Usart Tx/Rx Enable	
	USART2->CR1 |= USART_CR1_UE ; // USART2 Enable

}

void putch(unsigned char c) //TX-send
{ 
	while(USART_SR_TXE == 0); // TX Complete olana kadar bekle 
	USART2->DR = c; 						// Datayi DR reg e gönder
} 
 
char getch(void) //Rx-receive
{ 
	while(USART_SR_RXNE == 0); 			// Verilerin okunabilmesi için bekle //rx not empty 0 olunca
	return (0xFF & USART_DR_DR); 
} 
 
void usart2_string(const char *st) 
{ 
 while(*st) 
 putch(*st++); 
} 