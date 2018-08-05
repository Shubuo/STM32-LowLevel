#include "stm32f10x.h"                  // Device header
#include "delay.h"
#include "lcd.h"
#include <stdio.h>

#define assert_param(expr) ((expr) ? (void)0 : assert_failed((uint8_t *)__FILE__, __LINE__))
#define IS_ADC_ALL_PERIPH(PERIPH) (((PERIPH) == ADC1) || \
                                   ((PERIPH) == ADC2) || \
                                   ((PERIPH) == ADC3))	 
void assert_failed(uint8_t* file, uint32_t line)
{ 
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */

  /* Infinite loop */
  while (1)
  {
  }
}

FlagStatus ADC_GetResetCalibrationStatus(ADC_TypeDef* ADCx)
{
  FlagStatus bitstatus = RESET;
  /* Check the parameters */
  assert_param(IS_ADC_ALL_PERIPH(ADCx));
  /* Check the status of RSTCAL bit */
  if ((ADCx->CR2 & ADC_CR2_RSTCAL) != (uint32_t)RESET)
  {
    /* RSTCAL bit is set */
    bitstatus = SET;
  }
  else
  {
    /* RSTCAL bit is reset */
    bitstatus = RESET;
  }
  /* Return the RSTCAL bit status */
  return  bitstatus;
}

FlagStatus ADC_GetCalibrationStatus(ADC_TypeDef* ADCx)
{
  FlagStatus bitstatus = RESET;
  /* Check the parameters */
  assert_param(IS_ADC_ALL_PERIPH(ADCx));
  /* Check the status of CAL bit */
  if ((ADCx->CR2 & ADC_CR2_CAL) != (uint32_t)RESET)
  {
    /* CAL bit is set: calibration on going */
    bitstatus = SET;
  }
  else
  {
    /* CAL bit is reset: end of calibration */
    bitstatus = RESET;
  }
  /* Return the CAL bit status */
  return  bitstatus;
}


int main(){
	delay_init();  //Delay icin konfigurasyonlari yap
	
	RCC->APB1ENR |= RCC_APB1ENR_USART2EN;
	RCC->APB2ENR |= RCC_APB2ENR_IOPAEN ;
	
/*******************USART1 Konfigurasyonlari*********************/

		USART2->CR1 |= (0 << 12);  	//Wordlength 8 bit, 12.bit

	
	///////				INIT	USART		////////
// Baud rate formula:
// - IntegerDivider = ((PCLKx) / (16 * (USART_InitStruct->USART_BaudRate)))
// - FractionalDivider = ((IntegerDivider - ((u32) IntegerDivider)) * 16) + 0.5 */
	
//  USART BaudRate Calculation for 9600
	USART2->BRR |= 0xEA << 4;		//USARTDIV = 234,375 ~ 234,38	
	USART2->BRR |= 0x6 ;			//DIV_Mantissa = 234, DIV_Fraction = 16*0,38 = 6 (nearest real number)
	// BRR = 0xEA6 = 12bit Mantissa + 4bit Fraction
	
// Configure USART Tx as alternate function  /
	GPIOA->CRL |= GPIO_CRL_MODE2 ; // OUTPUT 50Mhz
	GPIOA->CRL |= GPIO_CRL_CNF2_1 ; //AF out PP
	
// Configure USART Rx as alternate function  /
	GPIOA->CRL |= GPIO_CRL_CNF3_1 ; //AF out PP
	GPIOA->CRL |= GPIO_CRL_MODE3 ; // OUTPUT 50Mhz


	USART2->CR2 |= (0 << 12) | (0 << 13);		// 1-stop bit
	
	USART2->CR1 |= (0 << 10) | (0 << 9) ;    // ParityControlEnable =0 ParitySelection=0 > No parity
	
	USART2->CR3 |= (0 << 9) | (0 << 8)	; 		// RTS&CTS Disable
	
	USART2->CR1 |= USART_CR1_TE | USART_CR1_RE ;		//Usart Tx/Rx Enable
		
	USART2->CR1 |= USART_CR1_UE ; // USART2 Enable
//////////////////////

	lcd_init();
	lcd_yaz("  RS232 Testi");
	lcd_gotoxy(2,1);
	lcd_yaz("USART ");
	delay_ms(1500);
	
	lcd_clear();
 	lcd_yaz("Gonderiliyor...");
 	lcd_gotoxy(2,1);
	printf("0");

/////*********EXS03_configADC*//////////
	RCC->APB2ENR |= RCC_APB2ENR_ADC1EN | RCC_APB2ENR_IOPCEN ;
	GPIOC->CRL |= 0xFFFFFF00;	//Pin0|1 AnalogIN
	ADC1->CR1 |= ADC_CR1_SCAN;
	ADC1->CR2 |= ADC_CR2_CONT | (0UL << 11) | ADC_CR2_EXTTRIG | ADC_CR2_EXTSEL;
	ADC1->SMPR1 |= ADC_SMPR1_SMP11_0 | ADC_SMPR1_SMP11_2;
	ADC1->CR2 |= ADC_CR2_ADON;
	ADC1->CR2 |= ADC_CR2_RSTCAL;
	while(ADC_GetResetCalibrationStatus(ADC1));
	ADC1->CR2 |= ADC_CR2_CAL;
	while(ADC_GetCalibrationStatus(ADC1));

	
	
}

