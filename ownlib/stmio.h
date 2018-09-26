#ifdef STM32F10X_MD
	#include "stm32f10x.h"
	#include "stm32f10x_gpio.h"             // Keil::Device:StdPeriph Drivers:GPIO
	#include "stm32f10x_rcc.h"              // Keil::Device:StdPeriph Drivers:RCC
	#include "misc.h"                       // Keil::De
#endif


	/************** Genel Tanýmlamalar *************************/
#define BUZZER(x) ((x) ?   GPIO_SetBits(GPIOC , GPIO_Pin_8) :  GPIO_ResetBits(GPIOC , GPIO_Pin_8) )

#define POT_CH 10
#define LDR_CH 11

#define KEY_BUTTON_EXTI_LINE             EXTI_Line15

    typedef enum
    {
        BUTTON_SW2 = 1,  // GPIOA
        BUTTON_SW3 = 0,  // GPIOA
        BUTTON_SW4 = 3,  // GPIOC
        BUTTON_SW5 = 2,  // GPIOC
    }
    Button_TypeDef;
		
		    /************** Genel Fonksiyon Prototipleri ***************/
   void initGPIO(void);
   void enableButtons(void);
	void configADC(unsigned char);
	void initUSART(void);
	
//////////////			DELAY
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __DELAY_H
#define __DELAY_H

#define IS_SYSTICK_CLK_SOURCE(SOURCE) (((SOURCE) == SysTick_CLKSource_HCLK) || \
                                       ((SOURCE) == SysTick_CLKSource_HCLK_Div8))

/* SysTick counter state */
#define SysTick_Counter_Disable        ((u32)0xFFFFFFFE)
#define SysTick_Counter_Enable         ((u32)0x00000001)
#define SysTick_Counter_Clear          ((u32)0x00000000)

#define IS_SYSTICK_COUNTER(COUNTER) (((COUNTER) == SysTick_Counter_Disable) || \
                                     ((COUNTER) == SysTick_Counter_Enable)  || \
                                     ((COUNTER) == SysTick_Counter_Clear))

/* SysTick Flag */
#define SysTick_FLAG_COUNT             ((u32)0x00000010)
#define SysTick_FLAG_SKEW              ((u32)0x0000001E)
#define SysTick_FLAG_NOREF             ((u32)0x0000001F)

#define IS_SYSTICK_FLAG(FLAG) (((FLAG) == SysTick_FLAG_COUNT) || \
                               ((FLAG) == SysTick_FLAG_SKEW)  || \
                               ((FLAG) == SysTick_FLAG_NOREF))

#define IS_SYSTICK_RELOAD(RELOAD) (((RELOAD) > 0) && ((RELOAD) <= 0xFFFFFF))

/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */

void delay_init(void);
void delay_ms(unsigned short int );
void delay_us(unsigned int);

//////////////		LCD
void veri_yolla(unsigned char byte);
void lcd_clear(void);
void lcd_yaz(const char * s);
void lcd_gotoxy(unsigned char x, unsigned char y);
void lcd_init(void);
void lcd_komut(unsigned char c);

/////////////////GPIO
// EX //PA0 = 0x4444444A
//GPIOA->CRL = (GPIOA->CRL & CONFMASKL(0)) | GPIOPINCONFL(0, GPIOCONF(GPIO_MODE_OUTPUT2MHz, GPIO_CNF_AFIO_PUSHPULL)); 
	#define GPIO_CNF_INPUT_ANALOG		   0
	#define GPIO_CNF_INPUT_FLOATING		1
	#define GPIO_CNF_INPUT_PULLUPDOWN	2

	#define GPIO_CNF_OUTPUT_PUSHPULL		0
	#define GPIO_CNF_OUTPUT_OPENDRAIN	1
	#define GPIO_CNF_AFIO_PUSHPULL		2
	#define GPIO_CNF_AFIO_OPENDRAIN		3

	#define GPIO_MODE_INPUT				0
	#define GPIO_MODE_OUTPUT10MHz		1
	#define GPIO_MODE_OUTPUT2MHz		2
	#define GPIO_MODE_OUTPUT50MHz		3

	#define GPIOCONF(mode, cnf)	((cnf << 2) | (mode))
	#define GPIOPINCONFL(pin, conf) (conf << (pin * 4))
	#define GPIOPINCONFH(pin, conf) (conf << ((pin - 8) * 4))

	#define CONFMASKL(pin) ((u32)~(15 << (pin * 4)))
	#define CONFMASKH(pin) ((u32)~(15 << ((pin - 8) * 4)))

#endif	
