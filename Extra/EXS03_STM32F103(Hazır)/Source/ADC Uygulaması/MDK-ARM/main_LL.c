#include "stm32f10x.h"
#include "delay.h"

#define LCD_GPIO	GPIOB
#define RS	  		GPIO_Pin_10
#define E		   		GPIO_Pin_11

u16 adc;
uint8_t adcch;
char adc_bilgisi[16];

/*************************Ana Program*************************/
int main(void)
{
		//Giris Cikilari Ayarla
	GPIO_InitTypeDef GPIO_InitStructure;	    /* GPIOA-GPIOC Periph clock enable */
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);  // LCD GPIO
    /*
    ********* LCD PB8...PB15 *********
    */
    GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_10 | GPIO_Pin_11 | GPIO_Pin_12 | GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15 ;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; //Output
    GPIO_Init(GPIOB, &GPIO_InitStructure);
  //ADC Config
	  ADC_InitTypeDef ADC_InitStructure;
	  
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1 | RCC_APB2Periph_GPIOC, ENABLE);
    GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_0 | GPIO_Pin_1;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN; 		//Input
    GPIO_Init(GPIOC, &GPIO_InitStructure);
    /* ADC1 konfigurasyonlari -------------------------*/
    ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;
    ADC_InitStructure.ADC_ScanConvMode = ENABLE;
    ADC_InitStructure.ADC_ContinuousConvMode = ENABLE;
    ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;
    ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;
    ADC_InitStructure.ADC_NbrOfChannel = 1;
    ADC_Init(ADC1, &ADC_InitStructure);
    /* ADC1 kanal 10'da*/
    ADC_RegularChannelConfig(ADC1, adcch, 1, ADC_SampleTime_55Cycles5);  /*LDR 11. ADC KANALINDA*/
    ADC_Cmd(ADC1, ENABLE);
		ADC_ResetCalibration(ADC1);
  	while(ADC_GetResetCalibrationStatus(ADC1));
  	ADC_StartCalibration(ADC1);
  	while(ADC_GetCalibrationStatus(ADC1));
		ADC_SoftwareStartConvCmd(ADC1,ENABLE);
		
		    delay_init();  //Delay icin konfigurasyonlari yap
/***************************************************************************/
/***************************************************************************/
/***************************************************************************/
		GPIOC->BRR = GPIO_Pin_8;
/***************************************************************************/
/******									LCD INIT															*************/
/***************************************************************************/

    LCD_GPIO->ODR &=0x03FF;
    delay_ms(100);
    lcd_komut(0x03);
    delay_ms(1);
    lcd_enable();
    delay_ms(1);
    lcd_enable();
    lcd_komut(0x02);
    delay_ms(1);
    lcd_komut(0x33);
    lcd_komut(0x32);
    lcd_komut(0x28);
    lcd_komut(0x0C);
    for(i=0; i<=7; i++) // tr karakter tanitiliyor
    {
        lcd_komut(adres);
        for(s=0; s<=7; s++)
        {
            veri_yolla(karakter_[i][s]);
        }
        adres=adres+8;
    }
    lcd_clear();






}

