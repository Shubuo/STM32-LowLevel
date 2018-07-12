#include "gpio_by.h"
/***************** Veri Tanýmlamalarý **********************/


/*******************Giris - Cikis Pinleri Konfigurasyonlari*********************/
void BY_initGPIO(void)
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
    GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_8 ; //((uint16_t)0x0100)  !< Pin 8 selected 
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; //Output
    GPIO_Init(GPIOC, &GPIO_InitStructure);
	}

	void BY_enableButtons(void)
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