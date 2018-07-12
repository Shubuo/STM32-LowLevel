/*
 * Dosya    : main.c
 * Yazar    : Expkits - www.expkits.com
 * Uygulama : DS18B20 Is Sensörü Test Uygulaması
 * Sürüm    : v1.0
 * Tarih    : 05.04.2013
 * MCU      : STM32F10x
 * Kristal  : 8Mhz (MCU FOSC 72Mhz)
 * Kit      : EXS03
 * Açıklama : Kit üzerindeki DS1820 soketine takılan sensörü okur ve LCD'de gösterir.
 * Uyarı    : Keil uVision v4.70 ile derlenmiştir.
 *
 */

/* Includes ------------------------------------------------------------------*/
#include "stm32f10x.h"
#include "delay.h"
#include "lcd.h"
#include "ds18b20.h"
#include "stdio.h"

#include "exs03.h"

float sicaklik;
char sicaklik_bilgisi[16]="";
static vu32 TimingDelay;

/* Fonksiyon Prototipleri-------------------------------------------------------*/


/*************************Ana Program*************************/
int main(void)
{
    EXS03_initGPIO();//Giris Cikilari Ayarla
    delay_init();  //Delay icin konfigurasyonlari yap
    GPIO_ResetBits(GPIOC , GPIO_Pin_8);	 // Buzzer Sussun
    lcd_init();
    lcd_yaz("    DS18B20     ");
    lcd_gotoxy(2,1);
    lcd_yaz("www.ExpKits.com ");
    delay_ms(1500);
    lcd_gotoxy(2,1);
    lcd_yaz("                ");
    while (1)
    {
        sicaklik=ds18b20_read();
        if(sicaklik>4095) {sprintf(sicaklik_bilgisi,"   Sens�r Yok   ");}
        else { sprintf(sicaklik_bilgisi,"  Is�: %3.1f%cC", sicaklik,223);}
        lcd_gotoxy(2,1);
        lcd_yaz(sicaklik_bilgisi);
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
