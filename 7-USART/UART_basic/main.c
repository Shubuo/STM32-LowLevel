#include "stm32f10x.h"                  // Device header
#include <stdio.h>
#include "usart.h"

char str[50];


int main(void){	
	
	volatile unsigned long dly;
	int i = 5000000;
	
			RCC->APB2ENR |= RCC_APB2ENR_IOPCEN;
			GPIOC->CRL 		= 0x3000;							//GPIO_CRL_MODE3 | (GPIO_CRL_CNF3&0x0); pc3
			GPIOC->ODR	 |= GPIO_ODR_ODR3;
	
			usart_init();
		
			while(i>1){i --; }                  // Wait for USART module to stabilize
			i=5000000;
 
					
					sprintf(str, "Lorem Ipsum Dolor Sit Amet\t\r");
					usart_string(str); 	
			
			for(;;){
	//					 if (USART_CR1_RXNEIE)       // If data is received (Receiver Register bosalmistir demek)    
//						{    
//							ch = USART1_Rx();     // read the received data								
//								USART1_TxCh();       // and send data via UART
//				putch(getch());

					

				for(dly = 0; dly < 3000000; dly++);	//i=7500000;while(i)i --; 		 // Delay 1sn
				GPIOC->BRR	|= GPIO_BRR_BR3; 
			}
	
}

