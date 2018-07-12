/*
 * Dosya    : main.c
 * Yazar    : Expkits - www.expkits.com
 * Uygulama : SHT11 Sensor Uygulamasi
 * Sürüm    : v1.0
 * Tarih    : 05.04.2013
 * MCU      : STM32F10x
 * Kristal  : 8Mhz (MCU FOSC 72Mhz)
 * Kit      : EXS03
 * Açıklama : Sensirion SHT1x sensorlerini okuyarak LCD ekranda sicaklik ve nem
 *            bilgisi gosterir.
 * Uyarı    : Keils uVision v470 ile derlenmiştir.
 *            EX8 Kullanıcıları için;
 *            - Sensörü geliştirme kitine takarken besleme voltajının doğru
 *              olduğundan emin olunuz. Aksi halde sensör zarar görebilir.
 *						- SWDIP1 üzerindeki tüm anahtarların kapalı olduğundan emin olun.
 */

/* Includes ------------------------------------------------------------------*/
#include "stm32f10x.h"
#include "delay.h"
#include "lcd.h"
#include "SHT11.h"
#include "stdio.h"
#include "exs03.h"

float  isi,nem;
char isi_bilgisi[16];
char nem_bilgisi[16];


/*************************Ana Program*************************/
int main(void)
{
    EXS03_initGPIO();//Giris Cikilari Ayarla
    delay_init();  //Delay icin konfigurasyonlari yap
    GPIO_ResetBits(GPIOC , GPIO_Pin_8);	 // Buzzer Sussun
    lcd_init();
    lcd_yaz("     SHT11      ");
    lcd_gotoxy(2,1);
    lcd_yaz("www.ExpKits.com ");
    delay_ms(1500);
    reset_sht11();
    lcd_clear();
    while(1)
    {
        while(!sht11_oku(&isi,&nem)) {lcd_clear(); lcd_yaz("   Sens�r Yok"); delay_ms(1000);}
        sprintf(isi_bilgisi," Is�: %3.1f%cC  ",isi,223);
        sprintf(nem_bilgisi," Nem: %%%3.1fRH  ",nem);
        lcd_gotoxy(1,1);
        lcd_yaz(isi_bilgisi);
        lcd_gotoxy(2,1);
        lcd_yaz(nem_bilgisi);
        delay_ms(280);
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
