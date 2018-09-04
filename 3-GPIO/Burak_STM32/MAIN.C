/*
 * Dosya    : main.c
 * Yazar    : Expkits - www.expkits.com
 * Uygulama : Buzzer Test Uygulamasi
 * Sürüm    : v1.0
 * Tarih    : 05.04.2013
 * MCU      : STM32F10x
 * Kristal  : 8Mhz (MCU FOSC 72Mhz)
 * Kit      : EXS03
 * Açiklama : Kit üzerindeki buzzer ile ses üretir.
 * Uyari    : Keil uVision v4.70 ile derlenmistir.
 *
 */
 
/* Includes ------------------------------------------------------------------*/
#include "stm32f10x.h"
#include "gpio_by.h" 	/* Expkits EXS-03 Development Kit Header File */
#include "delay.h"
#include "lcd.h"

u16 kont=0;
u32 temp=0;

/*************************Ana Program*************************/
int main(void)
{
    BY_initGPIO();	//Giris Cikilari Ayarla
    delay_init();  	//Delay icin konfigurasyonlari yap
    GPIO_ResetBits(GPIOC , GPIO_Pin_8);	 // Buzzer Sussun
	
	 lcd_init();
    lcd_yaz("Selam Burak");
    lcd_gotoxy(2,1);
    lcd_yaz("LCD Hazir");
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

