/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __EXS03_H
#define __EXS03_H
#ifdef __cplusplus
extern "C" {
#endif
	
#ifdef STM32F10X_MD
	#include "stm32f10x.h"
#elif STM32F2XX
	#include "stm32f2xx.h"
#endif

    /************** Genel Tanimlamalar *************************/
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
    void EXS03_initGPIO(void);
    void EXS03_enableButtons(void);
    void EXS03_configADC(uint8_t adcch);
    void EXS03_initUSART(void);
#ifdef __cplusplus
}
#endif
#endif
