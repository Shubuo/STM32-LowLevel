#ifdef STM32F10X_MD
	#include "stm32f10x.h"
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
    void BY_initGPIO(void);
    void BY_enableButtons(void);
		
		