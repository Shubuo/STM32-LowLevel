#include "stm32f10x.h"                  // Device header
#include "stm32f10x_usart.h"            // Keil::Device:StdPeriph Drivers:USART
#include "stm32f10x_rcc.h"              // Keil::Device:StdPeriph Drivers:RCC
#include <stdio.h>


//void setup(void);
//unsigned char USART1_receive(void);
//void USART1_send(unsigned char value);


int main()
{
  unsigned char ch = 0;
	RCC->APB2ENR |= RCC_APB2ENR_IOPCEN;
	GPIOC->CRL |= 0x3000;//GPIO_CRL_MODE3 | (GPIO_CRL_CNF3&0x0);
  GPIOC->ODR	|= GPIO_ODR_ODR3;
	 int i = 50000;
	while(i>1){i --; }                  // Wait for UART module to stabilize
	i=5000000;
//	GPIOC->BSRR	|= GPIO_BSRR_BR3;
	
//	setup();
////  UART1_Init_Advanced(9600, _UART_8_BIT_DATA, _UART_NOPARITY, _UART_ONE_STOPBIT, &_GPIO_MODULE_USART1_PA9_10);
// 	
//  while (1) 
//  {                     
//            if (USART_CR1_RXNEIE)       // If data is received     
//            {     
//                    ch = USART1_receive();     // read the received data
//                    USART1_send(ch);       // and send data via UART
//										printf("Mesajini aldim");
//										GPIOC->BRR	|= GPIO_BRR_BR3;
//            }    
//  }
}

void setup()

{
		RCC->APB2ENR |= RCC_APB2ENR_IOPAEN;     	  //Enable GPIOA
		RCC->APB2ENR |= RCC_APB2ENR_USART1EN;       //Enable USART1

    GPIOA->CRH = 0x4A0;                                //Set PA9 and PA10 as AFIO pins
    USART1->BRR = 0x341;                               //Set value for required baud rate
		USART1->CR1 |= USART_CR1_UE | USART_CR1_RE | USART_CR1_TE;
//Set USART1  | Set USART1 RX to receive incoming data |Set USART1 TX to send data
}



unsigned char USART1_receive()                        

{
	USART1->DR |= USART_DR_DR;
     while(USART_SR_RXNE == 0);                 //wait until data is available for reading
     return (0xFF & USART_DR_DR);               //return read data

}

 

void USART1_send(unsigned char value)

{
     USART1->DR = value;                         //transfer data to DR register
		 USART1 -> SR |= USART_SR_TXE;

     while(USART_SR_TXE == 0);             //wait until data transmission is complete

}    
