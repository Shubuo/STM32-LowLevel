/******************** (C) ExpKits 2011 STM32 MCU   **************************
*
* Dosya Adi          : lcd.c Header
* Yazar				 : Burak YÃ¶rÃ¼k
* Version            : V1.0.0
* Tarih              : 17/EylÃ¼l/2018
*
****************************************************************************/

#include "stmio.h"
//#include "lcd.h"
//#include "delay.h"
/***************** Veri Tanýmlamalarý **********************/
#if defined  (HSE_VALUE)
/* HSE deðerini yeniden tanýmlýyoruz. Bizim (EXS-03 Kartýmýzdaki kristal 8Mhz. 
 Ancak ST' nin kütüphanesinde bu deðer öntanýmlý olarak 25Mhz olarak gelmektedir.*/
 #undef HSE_VALUE
 #define HSE_VALUE    ((uint32_t)8000000)	/* Kristalimiz ne ise burada o tanýmlanmalý */
#endif /* HSE_VALUE */

#define CTRL_TICKINT_Set      ((u32)0x00000002)
#define CTRL_TICKINT_Reset    ((u32)0xFFFFFFFD)

/* Private variables ---------------------------------------------------------*/	 
static uint8_t  delay_fac_us = 0;  
static uint16_t delay_fac_ms = 0;  
static FlagStatus  Status;
/************************** SysTick Fonksiyonlarý******************************/

void SysTick_SetReload(u32 Reload)
{
  /* Check the parameters */
  assert_param(IS_SYSTICK_RELOAD(Reload));

  SysTick->LOAD = Reload;
}


void SysTick_CounterCmd(u32 SysTick_Counter)
{
  /* Check the parameters */
  assert_param(IS_SYSTICK_COUNTER(SysTick_Counter));

  if (SysTick_Counter == SysTick_Counter_Enable)
  {
    SysTick->CTRL |= SysTick_Counter_Enable;
  }
  else if (SysTick_Counter == SysTick_Counter_Disable) 
  {
    SysTick->CTRL &= SysTick_Counter_Disable;
  }
  else /* SysTick_Counter == SysTick_Counter_Clear */
  {
    SysTick->VAL = SysTick_Counter_Clear;
  }    
}

void SysTick_ITConfig(FunctionalState NewState)
{
  /* Check the parameters */
  assert_param(IS_FUNCTIONAL_STATE(NewState));

  if (NewState != DISABLE)
  {
    SysTick->CTRL |= CTRL_TICKINT_Set;
  }
  else
  {
    SysTick->CTRL &= CTRL_TICKINT_Reset;
  }
}

u32 SysTick_GetCounter(void)
{
  return(SysTick->VAL);
}

FlagStatus SysTick_GetFlagStatus(u8 SysTick_FLAG)
{
  u32 statusreg = 0, tmp = 0 ;
  FlagStatus bitstatus = RESET;

  /* Check the parameters */
  assert_param(IS_SYSTICK_FLAG(SysTick_FLAG));

  /* Get the SysTick register index */
  tmp = SysTick_FLAG >> 3;

  if (tmp == 2) /* The flag to check is in CTRL register */
  {
    statusreg = SysTick->CTRL;
  }
  else          /* The flag to check is in CALIB register */
  {
    statusreg = SysTick->CALIB;
  }

  if ((statusreg & ((u32)1 << SysTick_FLAG)) != (u32)RESET)
  {
    bitstatus = SET;
  }
  else
  {
    bitstatus = RESET;
  }
  return bitstatus;
}
/***********************Gecikme Rutinleri***************************************/

void delay_init(void)
 {
  RCC_ClocksTypeDef RCC_ClocksStatus;
  RCC_GetClocksFreq(&RCC_ClocksStatus);
  SysTick_CLKSourceConfig(SysTick_CLKSource_HCLK_Div8);  /* HCLK/8 */  
  SysTick_ITConfig(DISABLE);
  delay_fac_us = RCC_ClocksStatus.HCLK_Frequency / 8000000;	// 8Mhz Kristal için
  delay_fac_ms = RCC_ClocksStatus.HCLK_Frequency / 8000;    // 8Mhz Kristal için  
 }

void delay_ms(uint16_t nms)
{    
    uint32_t temp = delay_fac_ms * nms;

    if (temp > 0x00ffffff)
    {
        temp = 0x00ffffff;
    }
    SysTick_SetReload(temp);		             
    SysTick_CounterCmd(SysTick_Counter_Clear);	 
    SysTick_CounterCmd(SysTick_Counter_Enable);	 
    do
    {
        Status = SysTick_GetFlagStatus(SysTick_FLAG_COUNT);
    }while (Status != SET);				         
    SysTick_CounterCmd(SysTick_Counter_Disable); 
	SysTick_CounterCmd(SysTick_Counter_Clear);	 
}

void delay_us(uint32_t nus)
{ 
    SysTick_SetReload(delay_fac_us * nus);       
    SysTick_CounterCmd(SysTick_Counter_Clear);	
    SysTick_CounterCmd(SysTick_Counter_Enable);	
    do
    {
        Status = SysTick_GetFlagStatus(SysTick_FLAG_COUNT);
    }while (Status != SET);						
    SysTick_CounterCmd(SysTick_Counter_Disable);
	SysTick_CounterCmd(SysTick_Counter_Clear);	
}
/*******************Giris - Cikis Pinleri Konfigurasyonlari*********************/
void initGPIO(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    /* GPIOA-GPIOC Periph clock enable */
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);  // LED, BUTTON GPIO
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);  // LCD GPIO
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE); 
	  /*
    *********LED1->PA0, LED2->PA1, LED3-> PC2,   LED4 -> PC3 *********
    */
		GPIO_InitStructure.GPIO_Pin 	= GPIO_Pin_0 | GPIO_Pin_1 ;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode 	= GPIO_Mode_Out_PP; 		//Output
    GPIO_Init(GPIOA, &GPIO_InitStructure);
    GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_2 | GPIO_Pin_3;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		//Output
    GPIO_Init(GPIOC, &GPIO_InitStructure);
    /*
    ********* LCD PB8...PB15 *********
    */
    GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_10 | GPIO_Pin_11 | GPIO_Pin_12 | GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15 ;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; //Output
    GPIO_Init(GPIOB, &GPIO_InitStructure);
    /*
    *********SCK PIN OUTPUT (SHT 11) *********
    */
    GPIO_InitStructure.GPIO_Pin =GPIO_Pin_8;// SCK pin out
    GPIO_Init(GPIOB, &GPIO_InitStructure);
    /*
    *********BUZZER PC8*********
    */
    GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_8 ;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; //Output
    GPIO_Init(GPIOC, &GPIO_InitStructure);
	}

	void enableButtons(void)
	{
	  GPIO_InitTypeDef GPIO_InitStructure;
		GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_0 | GPIO_Pin_1 ;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD; 		//Input
    GPIO_Init(GPIOA, &GPIO_InitStructure);
    GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_2 | GPIO_Pin_3;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD; 		//Input
    GPIO_Init(GPIOC, &GPIO_InitStructure);
	}

	void configADC(uint8_t adcch)
{
    ADC_InitTypeDef ADC_InitStructure;
    GPIO_InitTypeDef GPIO_InitStructure;
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
}

/*******************USART1 Konfigurasyonlari*********************/
void initUSART(void)
{
	  USART_InitTypeDef USART_InitStructure;
    GPIO_InitTypeDef GPIO_InitStructure;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_USART1,ENABLE);
    /* Configure USART Tx as alternate function  */
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &GPIO_InitStructure);
    /* Configure USART Rx as alternate function  */
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
    GPIO_Init(GPIOA, &GPIO_InitStructure);
    USART_InitStructure.USART_BaudRate = 9600; // 9600 baud
    USART_InitStructure.USART_WordLength = USART_WordLength_8b;
    USART_InitStructure.USART_StopBits = USART_StopBits_1;
    USART_InitStructure.USART_Parity = USART_Parity_No;
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
    USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
    /* USART configuration */
    USART_Init(USART1, &USART_InitStructure);
    /* Enable USART */
    USART_Cmd(USART1, ENABLE);

}



				/* LCD Fonksiyonlari */

void lcd_enable(void);

/***************************************************************************/
unsigned char i,s,adres=64;
unsigned char karakter_[8][8]=
{
    /* TR Karakterler */
    { 0,14,16,16,17,14, 4, 0},//ç
    { 0, 0,12, 4, 4, 4,14, 0},//I
    {10, 0,14,17,17,17,14, 0},//ö
    { 0,15,16,14, 1,30, 4, 0},//$
    {10, 0,17,17,17,17,14, 0},//ü
    /* özel isaretler */
    {2, 6,14,30,14, 6,  2, 0},//<	 veri_yolla(05);
    { 0, 4, 4, 4, 4, 4, 4, 0},//|	 veri_yolla(06);
    { 0, 16, 8, 4, 2, 1, 0,0} //\//	 veri_yolla(07);
};
/***************************************************************************/
void lcd_busy(void)
{
    unsigned long h=20000;	 // delay 32Mhz = 2000
    while(h--);
}
/***************************************************************************/
void lcd_komut(unsigned char c)
{
    GPIOB->ODR &=0x3FF;
    GPIOB->ODR |=(c&0xF0)<<8;
    lcd_enable();
    lcd_busy();
    GPIOB->ODR &=0x3FF;
    GPIOB->ODR |= (c<<0x0C);
    lcd_enable();
    lcd_busy();
}
/***************************************************************************/
void veri_yolla(unsigned char byte)
{
    switch (byte)
    {
    case 'Ç' :
        byte=0x00;
        break;
    case 'Ý' :
        byte=0x01;
        break;
    case 'Ö' :
        byte=0x02;
        break;
    case 'Þ' :
        byte=0x03;
        break;
    case 'Ü' :
        byte=0x04;
        break;
    case 'ç' :
        byte=0x00;
        break;
    case 'ý' :
        byte=0x01;
        break;
    case 'ö' :
        byte=0x02;
        break;
    case 'þ' :
        byte=0x03;
        break;
    case 'ü' :
        byte=0x04;
        break;
    default :
        break;
    }
    GPIOB->ODR &=0x3FF;
    GPIOB->ODR |= ((byte&0xF0)<<8)|GPIO_Pin_10;	//RS
    lcd_enable();
    lcd_busy();
    GPIOB->ODR &=0x3FF;
    GPIOB->ODR |= (byte<<0x0C)|GPIO_Pin_10;
    lcd_enable();
    lcd_busy();
}
/***************************************************************************/
void lcd_clear(void)
{
    lcd_komut(0x1);
    delay_ms(5);
}
/***************************************************************************/
void lcd_yaz(const char *s)
{
    while(*s)
        veri_yolla(*s++);
}
/***************************************************************************/
void lcd_gotoxy(unsigned char x,unsigned char y)
{
    if(x==1)
        lcd_komut(0x80+((y-1)%16));
    else
        lcd_komut(0xC0+((y-1)%16));
}
/***************************************************************************/
void lcd_enable(void)
{
    GPIOB->ODR|=GPIO_Pin_11;	//E
    lcd_busy();
    GPIOB->ODR&=~(GPIO_Pin_11);
}
/***************************************************************************/
/***************************************************************************/
/***************************************************************************/
void lcd_init()
{
    GPIOB->ODR &=0x03FF;
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

/***************************************************************************/
/***************************************************************************/
/***************************************************************************/

