#include "stm32f10x.h"                  // Device header
#include "usart.h"
#include <stdio.h>

unsigned char data[50],flag=0;
uint8_t	rx_irq;

#define NUM 10

int n,j; 
char name[NUM+1] = {'\0'}; 

	void gpio_init(void){
	
	RCC->APB2ENR |= RCC_APB2ENR_IOPAEN | RCC_APB2ENR_IOPCEN | RCC_APB2ENR_IOPBEN;
	GPIOB->CRH	= 0x33333300 ; 	// pin 10~15 50MHZ Output pushpull
	GPIOA->CRL	= 0x33;
	GPIOC->CRL	= 0x3300;
		
	}
	
	void adc_init(){
	
	RCC->APB2ENR |= RCC_APB2ENR_ADC1EN | RCC_APB2ENR_IOPCEN ;
	GPIOC->CRL |= 0xFFFFFF00;	//Pin0|1 AnalogIN			//LDR + POT
	ADC1->CR1 |= ADC_CR1_SCAN;
	ADC1->CR2 |= ADC_CR2_CONT | (0UL << 11) | ADC_CR2_EXTTRIG | ADC_CR2_EXTSEL;
	ADC1->SMPR1 |= ADC_SMPR1_SMP11_0 | ADC_SMPR1_SMP11_2;
	ADC1->CR2 |= ADC_CR2_ADON;
	ADC1->CR2 |= ADC_CR2_RSTCAL;
	while(ADC_GetResetCalibrationStatus(ADC1));
	ADC1->CR2 |= ADC_CR2_CAL;
	while(ADC_GetCalibrationStatus(ADC1));
		
	}


	void Usart_NVIC(void){		//NVIC Configure
	
		// NVIC_BASE->ISER[1] |= NVIC_ISER_SETENA_5;//0x00000040; (1 << 5) ;
//  NVIC->ISER[(USART1_IRQn >> 5UL)] = (uint32_t)(1UL << (USART1_IRQn & 0x1FUL));
	NVIC_EnableIRQ(USART1_IRQn);		// USART1 kesmesi açiliyor

//	(((NVIC->IABR[(USART1_IRQn >> 5UL)] & (1UL << (USART1_IRQn & 0x1FUL))) != 0UL) ? 1UL : 0UL);
//	NVIC_GetActive(USART1_IRQn);
	
	NVIC_SetPriority(USART1_IRQn,1);
	
	}


	void usart_init(void)
	{
		
		RCC->APB2ENR |= RCC_APB2ENR_IOPAEN;     	  //Enable GPIOA
		RCC->APB2ENR |= RCC_APB2ENR_USART1EN;       //Enable USART1

	//Set PA9 and PA10 as AFIO pins --0x4A0
	// Configure USART Tx as alternate function  / 1010
		GPIOA->CRH = GPIO_CRH_MODE9_1 | ((GPIO_CRH_CNF9_0 & 0x00) | GPIO_CRH_CNF9_1) ;  //AF out PP	 OUTPUT 2Mhz
	// Configure USART Rx as alternate function  / 0100
		GPIOA->CRH |= GPIO_CRH_CNF10_0 ; //Floating Input Reset
		//div = USART2_PCLK1 / (16*USART2_BAUD_RATE) ;	//USARTDIV = 468,75
    USART1->BRR = (USART1_PCLK2/USART1_BAUD_RATE);				//0x1D4C ;   //Set value for required baud rate
//		USART1->CR1 |= (USART1_M << 12) ; 	//Wordlength 8 bit, 12.bit
//		// ParityControlEnable =0 ParitySelection=0 > No parity	
//		USART1->CR1 |= (USART1_PCE << 10)| (USART1_PS << 9)  ;								
//		USART1->CR2 |= (USART1_STOP << 12) | (USART1_STOP << 13);		// 1-stop bit
//		USART1->CR3 |= (0 << 9) | (0 << 8)	; 		// RTS&CTS Disable

		USART1->CR1 |= USART_CR1_UE | USART_CR1_RE | USART_CR1_TE;
//Set USART1  | Set USART1 RX to receive incoming data |Set USART1 TX to send data

	}

	
/////////     TRANSMITTER		/////////
//Send Character
void putch(unsigned char value)		
{
		while(!(USART1->SR & USART_SR_TXE));
		USART1->DR 	 = value;                    //transfer data to DR register
		//  USART1->DR = (ch & 0xFF);
		//		 USART1 -> SR |= USART_SR_TXE;	
//     while(USART_SR_TXE == 0);             			//wait until data transmission is complete
}  

//Send String
void putch_string(const char *st) // volatile char da kullanilabilir
	{ 
	 while(*st != 0) {
		while(!(USART1->SR	& USART_SR_TC	));
		putch(*st++);
	 }
	} 

//int USART1_Tx (int ch)  {
//  while (!(USART1->SR & USART_SR_TXE));
//  USART1->DR = (ch & 0xFF);
//  return (ch);
//	
//}
	
/////////     RECEIVER		///////// 
char getch(void)  //USART1_IRQ HAndler
	{ 
		while (!(USART1->SR & USART_SR_RXNE));
//		while(USART_SR_RXNE == 0); 			// Verilerin okunabilmesi için bekle //rx not empty 0 olunca
//		return (0xFF & USART_DR_DR); 
		return ((USART1->DR )); 
	} 

//void getch_string(const char *st) // volatile char da kullanilabilir
//	{ 
//		while(!(USART1->SR	& USART_SR_RXNE	));
//		getch(*st++);
//	} 	


	void Usart_IRQHandler(void) __irq 	// IRQ Handler
	{ 
//	 unsigned char kesme_bilgisi; 
	 
//		kesme_bilgisi=USART_CR1_RXNEIE; 
		
		// Test for RXNE pending interrupt
	if ((USART1->SR & USART_SR_RXNE) != RESET)
		{
			n = getch();
			if(j == NUM) {
				data[j] = n;
				j = 0;
			}
			else
			{
				data[j++] =n;
			}
			data[j] = '\0';
//		data[rx_irq]	=	USART1->DR; 
//		rx_irq++;		//flag=1; // Bilgi alindi bayragi 

		}
	} 
/*
	//Toggling the onboard led on PC13 after the occurred interrupt.
//The interrupt occurred when you connect PB12 to the ground.

volatile boolean led ;
volatile unsigned long x ;

void setup() 
{
  Serial.begin(250000);  
  Serial.println("Start"); 

  RCC_BASE->APB2ENR |= (1 << RCC_APB2ENR_IOPBEN);       // Enable PB. p145
  RCC_BASE->APB2ENR |= (1 << RCC_APB2ENR_IOPCEN);       // Enable PC. p145
  
  GPIOC->regs->CRH = GPIOC->regs->CRH | 0x00300000;     // Set output mode on PC13 @ max freq 50MHz (MODE13[1:0] = 11) p158, p171  
  GPIOC->regs->CRH = GPIOC->regs->CRH & 0xFF3FFFFF;     // Set output mode on PC13 as general push-pull output (cnf8[1:0] = 00)
  GPIOB->regs->CRH = 0x44484444;                        // Set input mode on PB12 P/U or P/D p158, p171 
  GPIOB->regs->ODR |= 0b0001000000000000;               // Pull up on PB12 is enabled
  
  RCC_BASE->APB2ENR  |= (1 << RCC_APB2ENR_AFIOEN);      // Enable AFIO. p145
  RCC_BASE->APB2RSTR |= (1 << RCC_APB2RSTR_AFIORST);    // Reset the AFIO registers. p105
  
  asm volatile("cpsie i");                              // Enables interrupts and configurable fault handlers. p99 C
  
  EXTI_BASE->FTSR    |= (1 << 12);                      // Falling on 12. p211
  EXTI_BASE->IMR     |= (1 << 12);                      // Interrupt mask register. p210
  AFIO_BASE->EXTICR4 |= (1 << 0);                       // On PB12. p191
  
  NVIC_BASE->IP[40]   = 0 << 4;                         // Priority to set, 0 being highest priority and 15 being lowest. From the table NVIC_EXTI_15_10 = 40. Move it to the left 4 positions. p125 C
  EXTI_BASE->PR      |= (1 << 12);                      // Clear pending interrupt on that pin. p212
  NVIC_BASE->ISER[1] |= (1 << 8);                       // Enable interrupt EXTI 10..15. From the table NVIC_EXTI_15_10 = 40. p119 C
}

void loop() 
{
  GPIOC->regs->BSRR  |= (1 << (13 + (led * 16)));       // Lower 16 bits setting while higher bits clearing. p172
  Serial.println(x);                                    // Show me how many interrupts has occurred
}

extern "C" void __irq_exti15_10(void)                   
{
  led = !led;                                           // Reverse state 
  EXTI_BASE->PR |= (1 << 12);                           // Clear pending interrupt on that pin. p212
  x++;                                                  // Count the interrupts. Remember, never try and clear the flag that caused the interrupt as the last  instruction in the ISR itself. It causes the ISR to reenter immediately.
}*/
