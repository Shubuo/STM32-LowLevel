#include "stm32f10x.h"                  // Device header
#include "stdio.h"
#include "exs03.h" /* Expkits EXS-03 Development Kit Header File */
#include "delay.h"
#include "stm32f10x_gpio.h"             // Keil::Device:StdPeriph Drivers:GPIO
#include "lcd.h"

float adc;
char adc_bilgisi[16];

unsigned int adc_read(unsigned char channel) 
{ 
 unsigned int i; 
 // ADC blogu açiliyor 
	RCC->APB2ENR	|= (1 << 9);  	//ADC1 Clock Enable 
	RCC->CFGR	 |= RCC_CFGR_ADCPRE_DIV6;  //0x8000 RCC_CFGR_ADCPRE_DIV6 72/6 = 12Mhz max

	ADC1->CR1 |= (0 << 8); // scan mode disable
	//ADC1->SQR1 = 0x100000; // L:0001, 2 conversions oldupu için hiç dokunulmadi
	// POT pini PC0 = ADC12_IN10 (ADC 1 veya 2 CH 10)
	ADC1->SQR3 = 0x0A; //1st Conversion Ch10
	ADC1->SMPR1 = 0x0 | ( 0x07 << channel*3); 	//SMP10 239.5 cycles 
	//12Mhz=83ns, 83*239.5~ 20us rcc bölümlerinden sonra 22 ms.
	
	// Start ADC1 ON, EXTI_Trig ON for Swstart ON,	Continuous mode, 11.bit 0 right allign
 ADC1 -> CR2  =  0x5E0003	 ; 
 ADC1 -> CR1 |= (1UL << 5 );  //EOCIE
 
 if(ADC1->SR & 0x02 )  //while(!(ADC1->DR & 0x8000)) 
 i = ADC1->DR; // Çevrim bilgisi okunuyor 
 
 return (i); // 11.bite kadar 12 bitlik ADC data 
} 

int main(void) 
{ 
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC->APB2ENR |= (1UL << 2);   	/* Enable GPIOA clock  */
	RCC->APB2ENR |=  1 <<  4;   		/* Enable GPIOC clock  */
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);  // LCD GPIO

 GPIOA->CRL   = 0x33;  		//0/1SET 
 GPIOC->CRL   = 0x3304;   //2/3SET, 0 floating input
//unsigned long int sayac = 50000; //(153 de yaniyor)
	
	/********** LCD PB8...PB15 *********    */
	GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_10 | GPIO_Pin_11 | GPIO_Pin_12 | GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15 ;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; //Output
	GPIO_Init(GPIOB, &GPIO_InitStructure);
		
		delay_init();  //Delay icin konfigurasyonlari yap
		lcd_init();
    lcd_yaz("-ADC Uygulamasý-");
    lcd_gotoxy(2,1);
		lcd_yaz("                ");
    //EXS03_configADC(POT_CH);
	
 for(;;) 
 { 
 adc = adc_read(0); 
 if(adc>2482) 	//2^11, 2 volttan büyük ise
	{
	 GPIOA->BSRR  = 0x00000003;		// 0/1 		Ledleri yak 
	 GPIOC->BSRR  = 0x0000000C;		// 2/3 ON 
	}
 else 
		{
	 GPIOA->BSRR  = 0x00030000; // 0/1 Ledleri söndür 
	 GPIOC->BSRR  = 0x000C0000;	// 2/3	OFF
		}
		//LCD Kullan
				//adc=4095-adc;
				adc = (adc*3.3)/4096 ;
        sprintf(adc_bilgisi,"Voltaj = %1.2f",adc);
        lcd_gotoxy(2,1);
        lcd_yaz(adc_bilgisi);
        delay_ms(250);			
	} 
 
} 


