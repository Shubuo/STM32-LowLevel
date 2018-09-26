/* Includes ------------------------------------------------------------------*/
//#include "stm32f10x.h"                  // Device header
#include "stmio.h" /* STM32 Expkits EXS-03 Development Kit Header File */
//#include "lcd.h"
//#include "delay.h"
//#include "stm32f10x_gpio.h"             // Keil::Device:StdPeriph Drivers:GPIO
//#include "stm32f10x_rcc.h"              // Keil::Device:StdPeriph Drivers:RCC
//#include "misc.h"                       // Keil::Device:StdPeriph Drivers:Framework



/*************************Ana Program*************************/
int main(void)
{
    initGPIO();//Giris Cikilari Ayarla
    delay_init();  //Delay icin konfigurasyonlari yap
    GPIO_ResetBits(GPIOC , GPIO_Pin_8);	 // Buzzer Sussun
    /*******LED'leri AC*******/
    GPIO_SetBits(GPIOA , GPIO_Pin_0);
    GPIO_SetBits(GPIOA , GPIO_Pin_1);
    GPIO_SetBits(GPIOC , GPIO_Pin_2);
    GPIO_SetBits(GPIOC , GPIO_Pin_3);
    delay_ms(500);
    /******LED'leri KAPAT*****/
    GPIO_ResetBits(GPIOA , GPIO_Pin_0);
    GPIO_ResetBits(GPIOA , GPIO_Pin_1);
    GPIO_ResetBits(GPIOC , GPIO_Pin_2);
    GPIO_ResetBits(GPIOC , GPIO_Pin_3);
    delay_ms(500);
    lcd_init();
    lcd_yaz("-LCD Uygulamasi-");
    lcd_gotoxy(2,1);
    lcd_yaz(" LCD Hazir ");
    while(1);
}
