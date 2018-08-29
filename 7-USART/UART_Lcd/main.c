
/*********************************
*
*		USART LCD ve LED Uygulamasi
*
**********************************/

#include "stm32f10x.h"                  // Device header
#include "delay.h"
#include "lcd.h"
#include <stdio.h>
#include "usart.h"
#include <string.h>


char str[50];
unsigned char ch[50];

int main(){
	uint16_t AD_value;	int n = 0;
	char adc_bilgisi[16];
	unsigned char key; char keystr[20];
	
	RCC->APB2ENR |= RCC_APB2ENR_IOPAEN | RCC_APB2ENR_IOPCEN | RCC_APB2ENR_IOPBEN;
	GPIOB->CRH	= 0x33333300 ; 	// pin 10~15 50MHZ Output pushpull
	GPIOA->CRL	= 0x33;
	GPIOC->CRL	= 0x3300;
	delay_init();  //Delay icin konfigurasyonlari yap

	lcd_init();
	lcd_yaz(" RS232 Testi");
	lcd_gotoxy(2,1);
	lcd_yaz(" USART ");
	delay_ms(1500);
	
	lcd_clear();
 	lcd_yaz("LedON için L");
 	lcd_gotoxy(2,1);
	
	usart_init();	

	adc_init();

	while(1){
//	
		key = getch();
		
		while (!(USART1->SR & USART_SR_RXNE));

//		ch[n] = USART1->DR;
//		n++;
//		
		
//		keystr[4] = getch_string();
//			while((ADC1->SR & ADC_SR_EOC)== ADC_SR_EOC);
//			AD_value = ADC1->DR;
//				AD_value=4095-AD_value;
//			sprintf(adc_bilgisi,"  LDR = %4d",AD_value);
//			usart_string(adc_bilgisi);
//			lcd_clear();
//			lcd_yaz(adc_bilgisi);
//	   ret = strncmp(keystr, "Led", 4);

		if( key == 'L')
			{
					sprintf(str, "\rLed'i actiniz\t\r ");  
					putch_string(str); 	
					GPIOA->ODR	|= GPIO_ODR_ODR0; 
				
					lcd_clear();
					lcd_yaz(str);
					lcd_gotoxy(2,1);
					sprintf(str, "LedOFF icin bas");
					lcd_yaz(str);
			}
		
		else if(key == 'A')
		{
			while((ADC1->SR & ADC_SR_EOC)== ADC_SR_EOC){};
			AD_value = ADC1->DR;
				AD_value=4095-AD_value;
			sprintf(adc_bilgisi,"  LDR = %4d",AD_value);
			putch_string(adc_bilgisi);
			lcd_clear();
			lcd_yaz(adc_bilgisi);
		}	
		else
			{
					lcd_clear();
					putch_string("\rLed OFF\r");
					GPIOA->BRR	|= GPIO_BRR_BR0;
					lcd_yaz("LED OFF");
			}
		delay_ms(5000);
	
	}
	
}

////#define assert_param(expr) ((expr) ? (void)0 : assert_failed((uint8_t *)__FILE__, __LINE__))
//#define IS_ADC_ALL_PERIPH(PERIPH) (((PERIPH) == ADC1) || \
//                                   ((PERIPH) == ADC2) || \
//                                   ((PERIPH) == ADC3))	 
//void assert_failed(uint8_t* file, uint32_t line)
//{ 
//  /* User can add his own implementation to report the file name and line number,
//     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */

//  /* Infinite loop */
//  while (1)
//  {
//  }
//}

//FlagStatus ADC_GetResetCalibrationStatus(ADC_TypeDef* ADCx)
//{
//  FlagStatus bitstatus = RESET;
//  /* Check the parameters */
//  assert_param(IS_ADC_ALL_PERIPH(ADCx));
//  /* Check the status of RSTCAL bit */
//  if ((ADCx->CR2 & ADC_CR2_RSTCAL) != (uint32_t)RESET)
//  {
//    /* RSTCAL bit is set */
//    bitstatus = SET;
//  }
//  else
//  {
//    /* RSTCAL bit is reset */
//    bitstatus = RESET;
//  }
//  /* Return the RSTCAL bit status */
//  return  bitstatus;
//}

//FlagStatus ADC_GetCalibrationStatus(ADC_TypeDef* ADCx)
//{
//  FlagStatus bitstatus = RESET;
//  /* Check the parameters */
//  assert_param(IS_ADC_ALL_PERIPH(ADCx));
//  /* Check the status of CAL bit */
//  if ((ADCx->CR2 & ADC_CR2_CAL) != (uint32_t)RESET)
//  {
//    /* CAL bit is set: calibration on going */
//    bitstatus = SET;
//  }
//  else
//  {
//    /* CAL bit is reset: end of calibration */
//    bitstatus = RESET;
//  }
//  /* Return the CAL bit status */
//  return  bitstatus;
//}
