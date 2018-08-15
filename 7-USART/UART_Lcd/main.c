#include "stm32f10x.h"                  // Device header
#include "delay.h"
#include "lcd.h"
#include <stdio.h>
#include "usart.h"

uint16_t AD_value;

char adc_bilgisi[16];



int main(){
	
	gpio_init();//Giris Cikilari Ayarla
	delay_init();  //Delay icin konfigurasyonlari yap

	lcd_init();
	lcd_yaz("  RS232 Testi");
	lcd_gotoxy(2,1);
	lcd_yaz("USART ");
	delay_ms(1500);
	
	lcd_clear();
 	lcd_yaz("Gonderiliyor...");
 	lcd_gotoxy(2,1);
	
	usart_init();
	printf("0");

/////*********EXS03_configADC*//////////
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

	while(1){
	
		while(!(ADC1->SR & ADC_SR_EOC));
		AD_value = ADC1->DR;
			AD_value=4095-AD_value;
		printf("%d\r\n",AD_value);
		sprintf(adc_bilgisi,"  LDR = %4d",AD_value);
		lcd_gotoxy(2,1);
		lcd_yaz(adc_bilgisi);
		delay_ms(50);
	
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
