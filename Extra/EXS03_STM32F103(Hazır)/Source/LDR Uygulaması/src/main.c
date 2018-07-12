/*
 * Dosya    : main.c
 * Yazar    : Expkits - www.expkits.com
 * Uygulama : LDR Test Uygulaması
 * Sürüm    : v1.0
 * Tarih    : 05.04.2013
 * MCU      : STM32F10x
 * Kristal  : 8Mhz (MCU FOSC 72Mhz)
 * Kit      : EXS03
 * Açıklama : Kit üzerindeki LDR' yi okuyarak LCD üzerinde gösterir.
 * Uyarı    : Keil uVision v4.70 ile derlenmiştir.
 */

/* Includes ------------------------------------------------------------------*/
#include "stm32f10x.h"
#include "stdio.h"
#include "exs03.h" /* Expkits EXS-03 Development Kit Header File */
#include "delay.h"
#include "lcd.h"

u16 adc;
char adc_bilgisi[16];

/*************************Ana Program*************************/
int main(void)
{
    EXS03_initGPIO();//Giris Cikilari Ayarla
    delay_init();  //Delay icin konfigurasyonlari yap
    GPIO_ResetBits(GPIOC , GPIO_Pin_8);	 // Buzzer Sussun
    lcd_init();
    lcd_yaz("-LDR Uygulamasi-");
    lcd_gotoxy(2,1);
    lcd_yaz("www.ExpKits.com");
    delay_ms(400);
    lcd_gotoxy(2,1);
    lcd_yaz("                ");
    EXS03_configADC(LDR_CH);
    while(1)
    {
        if(ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC)==SET)
        {
            adc=ADC_GetConversionValue(ADC1);
            sprintf(adc_bilgisi,"  LDR = %4d",4095-adc);
            lcd_gotoxy(2,1);
            lcd_yaz(adc_bilgisi);
            delay_ms(250);
        }
    }
}


#ifdef  USE_FULL_ASSERT

/**
  * @brief  Reports the name of the source file and the source line number
  *   where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
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
