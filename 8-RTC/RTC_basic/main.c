#include "stm32f10x.h"                  // Device header
#include "rtc.h"
#include "lcd.h"
#include	"delay.h"
#include <stdio.h>
#include "stm32f10x_gpio.h"             // Keil::Device:StdPeriph Drivers:GPIO
#include "stm32f10x_rcc.h"              // Keil::Device:StdPeriph Drivers:RCC

// Main program

int main()
{
	time_t		now;
	char str[50];

//	// Configure System Clock
//	SystemClock_Config();

	// Configure RTC Clock
	RTC_Clock_Config();

	// LCD init
	lcd_init();
	lcd_clear();
	lcd_yaz("Haazirim!");
	
	// Set RTC time
	now.hours   = 16;
	now.minutes = 00;
	now.seconds = 00;
	RTC_SetTime(&now);
	// Main loop
	while(1)
	{
	   RTC_GetTime(&now);

					lcd_clear();
					lcd_yaz("RTC Time is");
					lcd_gotoxy(2,1);
					sprintf(str,"%02d:%02d:%02d", now.hours, now.minutes, now.seconds);
					lcd_yaz(str);

	   delay_ms(100);
	}
}
