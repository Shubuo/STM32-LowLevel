/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __DELAY_H
#define __DELAY_H

#ifdef __cplusplus
 extern "C" {
#endif

//#ifdef STM32F10X_MD
	#include "stm32f10x.h"
//#else
//	#include "stm32f2xx.h"
//#endif


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
void delay_ms(uint16_t nms);
void delay_us(uint32_t nus);

#ifdef __cplusplus
}
#endif
#endif

