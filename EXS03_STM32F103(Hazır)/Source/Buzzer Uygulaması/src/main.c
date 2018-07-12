/*
 * Dosya    : main.c
 * Yazar    : Expkits - www.expkits.com
 * Uygulama : Buzzer Test Uygulaması
 * Sürüm    : v1.0
 * Tarih    : 05.04.2013
 * MCU      : STM32F10x
 * Kristal  : 8Mhz (MCU FOSC 72Mhz)
 * Kit      : EXS03
 * Açıklama : Kit üzerindeki buzzer ile ses üretir.
 * Uyarı    : Keil uVision v4.70 ile derlenmiştir.
 *
 */
 
/* Includes ------------------------------------------------------------------*/
#include "stm32f10x.h"
#include "exs03.h" /* Expkits EXS-03 Development Kit Header File */
#include "delay.h"
#include "lcd.h"

u16 kont=0;
u32 temp=0;

/*************************Ana Program*************************/
int main(void)
{
    EXS03_initGPIO();//Giris Cikilari Ayarla
    delay_init();  //Delay icin konfigurasyonlari yap
    GPIO_ResetBits(GPIOC , GPIO_Pin_8);	 // Buzzer Sussun
    lcd_init();
    lcd_yaz("Buzzer Test");
    lcd_gotoxy(2,1);
    lcd_yaz("www.ExpKits.com");
    delay_ms(400);
    lcd_gotoxy(2,1);
    lcd_yaz("                ");
    while (1)
    {
        temp+=3;
        delay_ms(temp);
        while(kont--)BUZZER(1);
        delay_ms(temp);
        while(kont--)BUZZER(0);
        lcd_gotoxy(2,(unsigned char) (temp/7) +1);
        lcd_yaz("*");
        if(temp>107)
        {
            temp=3;
            lcd_gotoxy(2,1);
            lcd_yaz("                ");
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
