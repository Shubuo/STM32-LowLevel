#include "stm32f10x.h"                  // Device header

int adc_1, adc_2;

unsigned int adc_read(unsigned char channel) 
{ 
 unsigned int i; 
 // ADC blogu açiliyor 
	RCC->APB2ENR	|= (1 << 10);  	//ADC2 Clock Enable 
	RCC->CFGR	= 0x8000;					//RCC_CFGR_ADCPRE_DIV6 72/6 = 12Mhz max

	ADC2->CR2 = 0x0011 	;	
	//ADC2 ON	Continuous mode	//0x00200300 | ((0x01)<<channel); 
	//The A/D converter is operational. 200
	// CLKDIV /12 											300
	//data allignment CR->2 11.bit 0 dayken right enable oldugu için girilmedi. 

 // A/D conversion start now
 ADC2 ->CR2 |= 0x011; 
 
 while(!(AD0GDR& 0x80000000)); 
 i = AD0GDR; // Çevrim bilgisi okunuyor 
 
 return ((i>>6) & 0x03FF); // 15:6 arasindaki 10 bitlik veri ADC data 
} 




//ADC1 Initialize - routine example by STM - Core - Adresses :

//__INLINE static void ADC1_Config_free(void) {
//  //ADC_InitStructure.ADC_Mode = ADC_Mode_Independent; //0: Independent mode
//  ADC1->CR1 = 0x00000000;
//  //ADC_InitStructure.ADC_ScanConvMode = Disable;
//  ADC1->CR1 |= (0 < < 8); // of course (SQR3 = 0x00000008; //Kanal 8 als 1 Conversion
//  ADC1->SMPR2 |= (5 < CR2 |= (0 < CR2 |= (1 < CR2 |= (1 < CR2)& (1 < 0);

//  // Start ADC1 calibration
//  //ADC_StartCalibration(ADC1);
//  ADC1->CR2 |= (1 < CR2)& (1 < 0);

//  //ADC_SoftwareStartConvCmd(ADC1, ENABLE);
//  ADC1->CR2 |= (1 < CR2 |= (1 << 22); //SW-Start
//}



////This is the ADC-init-function. i tried to translate the StdPeriph_Lib-functions to the STM32-register-calls:

////ADC_InitStructure.ADC_ScanConvMode = Disable;
//ADC1->CR1 |= (0<CR2 |= (0<CR2 |= (1<CR2 |= (7<CR2 |= (0<CR1 |= (0<SQR1 = 0x00100000;
//ADC1->SQR3 = 0x00000008; //Kanal 8 als 1 Conversion

