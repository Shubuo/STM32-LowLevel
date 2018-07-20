#include "stm32f10x.h"                  // Device header
#include "exs03.h" /* Expkits EXS-03 Development Kit Header File */
#include "delay.h"
#include "lcd.h"

int main(void)
{
    EXS03_initGPIO();//Giris Cikilari Ayarla
    delay_init();  //Delay icin konfigurasyonlari yap
    GPIO_ResetBits(GPIOC , GPIO_Pin_8);	 // Buzzer Sussun
	
		lcd_init();
	  lcd_yaz("Selam sana");
    lcd_gotoxy(2,1);
    lcd_yaz("www.ExpKits.com");
    delay_ms(400);
    lcd_gotoxy(2,1);
    lcd_yaz("                ");
 /* Butonlar Aktif */
	
	GPIO_InitTypeDef GPIO_InitStructure;
		GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_0 | GPIO_Pin_1 ;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD; 		//Input
    GPIO_Init(GPIOA, &GPIO_InitStructure);
   
		GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_2 | GPIO_Pin_3;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD; 		//Input
    GPIO_Init(GPIOC, &GPIO_InitStructure);
	
    while(1)
    {
        if(GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_0))
				{
            lcd_gotoxy(1,1);
            lcd_yaz("SW3'e  Bastiniz!");
        }
        else if(GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_1))
        {
            lcd_gotoxy(1,1);
            lcd_yaz("SW2'ye Bastiniz!");
        }
        else if(GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_2))
        {
            lcd_gotoxy(1,1);
            lcd_yaz("SW5'e  Bastiniz!");
        }
        else if(GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_3))
        {
            lcd_gotoxy(1,1);
            lcd_yaz("SW4'e  Bastiniz!");
        }
        else
        {
            lcd_gotoxy(1,1);
            lcd_yaz("-Butona Basiniz-");
        }
        delay_ms(50);
    }
}