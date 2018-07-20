/*
 * timers.h
 */

#ifndef __TIMERS_H
#define __TIMERS_H

#include <stm32f4xx.h>

typedef enum {
	IC_RISING_EDGE,
	IC_FALLING_EDGE,
	IC_BOTH_EDGES
} IC_Mode;


#define IRQ_UPDATE  0x0001
#define IRQ_CC_1	0x0002
#define IRQ_CC_2	0x0004
#define IRQ_CC_3	0x0008
#define IRQ_CC_4	0x0010

void TIM_init(TIM_TypeDef * tim);
void TIM_config_timebase(TIM_TypeDef * tim, uint16_t prescaler, uint32_t autoreload);
void TIM_enable_irq(TIM_TypeDef * tim, int irq_type);
void TIM_config_IC(TIM_TypeDef * tim, int chan, IC_Mode icmode);
void TIM_config_PWM(TIM_TypeDef * tim, int chan);

#define TIM_set(tim,c) 		{ tim->CNT = c; }
#define TIM_get(tim) 		tim->CNT
#define TIM_on(tim) 		{ tim->CR1 |= 1; tim->SR = 0; }
#define TIM_off(tim)		{ tim->CR1 &= 0xfffe; }
#define TIM_update_check(tim) 	((tim->SR & 1) == 1)
#define TIM_update_clear(tim) 	{ tim->SR &= 0xfffe; }
#define TIM_cc_check(tim,chan)	((tim->SR & (1 << chan)) != 0 && (tim->DIER & (1 << chan)) != 0)
#define TIM_cc_clear(tim,chan) 	{ tim->SR &= ~(uint32_t)(1 << chan); }

#endif
