
#include "exs03.h"
/***************** Veri Tanımlamaları **********************/


/*******************Giris - Cikis Pinleri Konfigurasyonlari*********************/
void EXS03_initGPIO(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    /* GPIOA-GPIOC Periph clock enable */
#ifdef STM32F10X_MD
    /* GPIOA-GPIOC Periph clock enable */
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);  // LED, BUTTON GPIO
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);  // LCD GPIO
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);  // LED, BUTTON, BUZZER GPIO
#elif STM32F2XX
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);  // LED, BUTTON GPIO
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);  // LCD GPIO
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);  // LED, BUTTON, BUZZER GPIO
#endif
    /*
    *********LED1->PA0, LED2->PA1, LED3-> PC2,   LED4 -> PC3 *********
    */
#ifdef STM32F2XX
    GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_0 | GPIO_Pin_1 ;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT; 		//Output
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP; 		// ve PushPull
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
    GPIO_Init(GPIOA, &GPIO_InitStructure);
    GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_2 | GPIO_Pin_3;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT; 		//Output
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP; 		// ve PushPull
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
    GPIO_Init(GPIOC, &GPIO_InitStructure);
    /*
    ********* LCD PB8...PB15 *********
    */
    GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_10 | GPIO_Pin_11 | GPIO_Pin_12 | GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15 ;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT; //Output
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP; // ve PushPull
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
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
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT; //Output
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP; 		// ve PushPull
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
    GPIO_Init(GPIOC, &GPIO_InitStructure);
#elif STM32F10X_MD
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
#endif
}

void EXS03_enableButtons(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
#ifdef STM32F2XX
    GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_0 | GPIO_Pin_1 ;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN; 		//Input
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP; 		// ve PushDown
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN;
    GPIO_Init(GPIOA, &GPIO_InitStructure);
    GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_2 | GPIO_Pin_3;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN; 		//Input
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP; 		// ve PushDown
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN;
    GPIO_Init(GPIOC, &GPIO_InitStructure);
#elif STM32F10X_MD
    GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_0 | GPIO_Pin_1 ;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD; 		//Input
    GPIO_Init(GPIOA, &GPIO_InitStructure);
    GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_2 | GPIO_Pin_3;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD; 		//Input
    GPIO_Init(GPIOC, &GPIO_InitStructure);
#endif
}

void EXS03_configADC(uint8_t adcch)
{
    ADC_InitTypeDef ADC_InitStructure;
    GPIO_InitTypeDef GPIO_InitStructure;
#ifdef STM32F2XX
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);
    GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_0 | GPIO_Pin_1;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AN; 		//Input
    GPIO_InitStructure.GPIO_OType = GPIO_OType_OD; 		// ve PushDown
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
    GPIO_Init(GPIOC, &GPIO_InitStructure);
    /* ADC1 konfigurasyonlari -------------------------*/
    ADC_InitStructure.ADC_ScanConvMode = ENABLE;
    ADC_InitStructure.ADC_ContinuousConvMode = ENABLE;
    ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;
    ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;
    ADC_InitStructure.ADC_NbrOfConversion = 1;
    ADC_Init(ADC1, &ADC_InitStructure);
    /* ADC1 kanal 10'da*/
    ADC_RegularChannelConfig(ADC1, adcch, 1, ADC_SampleTime_56Cycles);  /*LDR 11. ADC KANALINDA*/
    ADC_Cmd(ADC1, ENABLE);
    ADC_SoftwareStartConv(ADC1);
#elif STM32F10X_MD
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
#endif
}

/*******************USART1 Konfigurasyonlari*********************/
void EXS03_initUSART(void)
{
	  USART_InitTypeDef USART_InitStructure;
    GPIO_InitTypeDef GPIO_InitStructure;
#ifdef STM32F2XX

    RCC_APB2PeriphClockCmd( RCC_AHB1Periph_GPIOA | RCC_APB2Periph_USART1,ENABLE);
    RCC_APB1PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);
    /* Connect PXx to USARTx_Tx*/
    GPIO_PinAFConfig(GPIOA, GPIO_PinSource9, GPIO_AF_USART1);
    /* Connect PXx to USARTx_Rx*/
    GPIO_PinAFConfig(GPIOA, GPIO_PinSource10, GPIO_AF_USART1);
    /* Configure USART Tx as alternate function  */
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &GPIO_InitStructure);
    /* Configure USART Rx as alternate function  */
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
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
#elif STM32F10X_MD
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
#endif
}


