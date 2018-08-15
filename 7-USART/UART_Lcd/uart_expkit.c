/*
 * Yazar    : Expkits - www.expkits.com
 * Uygulama : RS232 UART Test
 * Sürüm    : v1.0
 * Tarih    : 05.04.2013
 * MCU      : STM32F10x
 * Kristal  : 8Mhz (MCU FOSC 72Mhz)
 * Kit      : EXS03
 * Açıklama : Amaç RS232 üzerinden PC ile haberleşmektir.
 *						ExpkitsUI programından haberleşme kanalı olarak RS232
 *						seçilmeli ve uygun PORT işaretlenmelidir.
 *
 * Uyarı    : Keil uVision v4.70 ile derlenmiştir.
 *						EXS03 kullanıcıları;
 *						- SWDIP1 üzerindeki tüm anahtarlar OFF konumunda olmalıdır.
 */

/* Includes ------------------------------------------------------------------*/
#include "stm32f10x.h"
#include "exs03.h" /* Expkits EXS-03 Development Kit Header File */
#include "delay.h"
#include "lcd.h"
#include <stdio.h>

#ifdef __GNUC__
  /* With GCC/RAISONANCE, small printf (option LD Linker->Libraries->Small printf
     set to 'Yes') calls __io_putchar() */
  #define PUTCHAR_PROTOTYPE int __io_putchar(int ch)
#else
  #define PUTCHAR_PROTOTYPE int fputc(int ch, FILE *f)
#endif /* __GNUC__ */

/* Global Degiskenler--- -----------------------------------------------------*/
uint16_t AD_value;

char adc_bilgisi[16];


/*************************Ana Program*************************/
int main(void)
{
	delay_init();  //Delay icin konfigurasyonlari yap
	EXS03_initGPIO();//Giris Cikilari Ayarla
	GPIO_ResetBits(GPIOC , GPIO_Pin_8);	 // Buzzer Sussun
	  
	lcd_init();
	lcd_yaz("  RS232 Testi");
	lcd_gotoxy(2,1);
	lcd_yaz("www.ExpKits.com");
	delay_ms(1500);
	
	lcd_clear();
 	lcd_yaz("Gonderiliyor...");
  lcd_gotoxy(2,1);

	EXS03_initUSART();
	printf("0");
	EXS03_configADC(LDR_CH);
   
		/* Sonsuz Dongu */
 while (1)
	{	  										  
	  
	if(ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC)==SET)
   {
    AD_value=ADC_GetConversionValue(ADC1);
		 AD_value=4095-AD_value;
		printf("%d\r\n",AD_value);
		sprintf(adc_bilgisi,"  LDR = %4d",AD_value);
		lcd_gotoxy(2,1);
		lcd_yaz(adc_bilgisi);
		delay_ms(50);
   }
	
	}

}




PUTCHAR_PROTOTYPE
{
  /* Place your implementation of fputc here */
  /* e.g. write a character to the USART */
  USART_SendData(USART1, (uint8_t) ch);

  /* Loop until the end of transmission */
  while (USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET)
  {}

  return ch;
}

#ifdef  USE_FULL_ASSERT

/**
  * @brief  Reports the name of the source file and the source line number
  *   where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t* file, uint32_t line)
{ 
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */

  /* Infinite loop */
  while (1)
  {
  }
}
#endif

/*********************************************************************************************************
      END FILE
*********************************************************************************************************/
