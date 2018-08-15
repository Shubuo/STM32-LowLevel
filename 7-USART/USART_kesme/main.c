#include "stm32f10x.h"                  // Device header
#include "lcd.h"
#include "usart.h"
#include <stdio.h>

unsigned char data[50],flag=0;
extern uint8_t	rx_irq;

//	void Usart_IRQHandler(void) __irq 	// IRQ Handler
//	{ 
////	 unsigned char kesme_bilgisi; 
//	 
////		kesme_bilgisi=USART_CR1_RXNEIE; 
//		
//		// Test for RXNE pending interrupt
//	if ((USART1->SR & USART_SR_RXNE) == USART_SR_RXNE)
//		{
//		
//		data[rx_irq]	=	USART1->DR; 
//		rx_irq++;		//flag=1; // Bilgi alindi bayragi 

//		}
//	}
	

int main() {
	
//unsigned char data,flag=0; 
uint8_t *prx_buffer;

	 
 lcd_init(); 

 USART1->CR1 |= USART_CR1_TXEIE;
	
// VICVectAddr0 = (unsigned int) &Usart_ISR; 
	Usart_NVIC();
	
	usart_string("LCD Yazmaya Hazir\n\r\n\r"); 
 
	
	 for(;;) 
	 { 
//		 if(flag) // Bilgi geldiyse bunu LCD'ye bas 
//		 { 
//		 veri_yolla(data); 
//		 flag=0; 
//		 } 
		 while (rx_irq > 0)
		{
			printf("%c", *prx_buffer);
			prx_buffer++;
			rx_irq --;
		}
		 
	 } 
}

