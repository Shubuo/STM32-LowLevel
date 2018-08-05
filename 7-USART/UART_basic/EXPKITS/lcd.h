/******************** (C) ExpKits 2011 STM32 MCU   **************************
*
* Dosya Adi          : lcd.h Header
* Yazar				 : Coskun ERGAN ( X-Fi )
* Version            : V1.0.2
* Tarih              : 25/Mart/2011
*
****************************************************************************/

#define LCD_GPIO	GPIOB
#define RS	  	GPIO_Pin_10
#define E		    GPIO_Pin_11


void veri_yolla(unsigned char byte);
void lcd_clear(void);
void lcd_yaz(const char * s);
void lcd_gotoxy(unsigned char x, unsigned char y);
void lcd_init(void);
void lcd_komut(unsigned char c);
