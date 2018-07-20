/*
 * gpio.h
 */

#ifndef __GPIO_H
#define __GPIO_H

#include <stm32f4xx.h>

#define GPIO_MODER(g,bit,val)  g->MODER = (g->MODER & (~((uint32_t)3 << (bit*2)))) | ((uint32_t)val << (bit*2))
#define GPIO_INPUT(g,bit)      GPIO_MODER(g,bit,0)
#define GPIO_OUTPUT(g,bit)     GPIO_MODER(g,bit,1)
#define GPIO_ALTERNATE(g,bit)  GPIO_MODER(g,bit,2)
#define GPIO_ANALOG(g,bit)     GPIO_MODER(g,bit,3)

#define GPIO_PUSH_PULL(g,bit)   g->OTYPER &= ~(1 << bit)
#define GPIO_OPEN_DRAIN(g,bit)  g->OTYPER |= (1 << bit)

#define GPIO_SPEED(g,bit,val)  g->OSPEEDR = (g->OSPEEDR & (~((uint32_t)3 << (bit*2)))) | ((uint32_t)val << (bit*2))
#define GPIO_SPEED_LOW(g,bit)  GPIO_SPEED(g,bit,0)
#define GPIO_SPEED_MID(g,bit)  GPIO_SPEED(g,bit,1)
#define GPIO_SPEED_FAST(g,bit) GPIO_SPEED(g,bit,2)
#define GPIO_SPEED_HIGH(g,bit) GPIO_SPEED(g,bit,3)

#define GPIO_PUPD(g,bit,val)  g->PUPDR = (g->PUPDR & (~((uint32_t)3 << (bit*2)))) | ((uint32_t)val << (bit*2))
#define GPIO_NO_PUPD(g,bit)   GPIO_PUPD(g,bit,0)
#define GPIO_PULL_UP(g,bit)   GPIO_PUPD(g,bit,1)
#define GPIO_PULL_DOWN(g,bit) GPIO_PUPD(g,bit,2)

#define GPIO_AFL(g,bit,val) g->AFR[0] = (g->AFR[0] & (~((uint32_t)0xf << (bit*4)))) | ((uint32_t)val << (bit*4))
#define GPIO_AFH(g,bit,val) g->AFR[1] = (g->AFR[1] & (~((uint32_t)0xf << ((bit-8)*4)))) | ((uint32_t)val << ((bit-8)*4))

#define GPIO_ON(gp,bit)  gp->BSRR = (1 << bit)
#define GPIO_OFF(gp,bit) gp->BSRR = (1 << (bit+16))
#define GPIO_VAL(gp,bit) ((gp->IDR & (1 << bit)) != 0)
#define GPIO_OVAL(gp,bit) ((gp->ODR & (1 << bit)) != 0)


#define AF_SYSTEM     0
#define AF_TIM1_2     1
#define AF_TIM3_5     2
#define AF_TIM9_11    3
#define AF_I2C1_3     4
#define AF_SPI1_4     5
#define AF_SPI3       6
#define AF_USART1_2   7
#define AF_USART6     8
#define AF_I2C2_3     9
#define AF_OTG_FS     10
#define AF_SDIO       12
#define AF_EVENTOUT   15

typedef enum {
	EXTI0 = 0,
	EXTI1,
	EXTI2,
	EXTI3,
	EXTI4,
	EXTI5,
	EXTI6,
	EXTI7,
	EXTI8,
	EXTI9,
	EXTI10,
	EXTI11,
	EXTI12,
	EXTI13,
	EXTI14,
	EXTI15
} EXTI_line;

typedef enum {
	RISING_EDGE = 0,
	FALLING_EDGE,
	BOTH_EDGES
} edge_type;

void GPIO_init(GPIO_TypeDef * gpio);
void GPIO_config_input(GPIO_TypeDef * gpio, int pin);
void GPIO_config_output(GPIO_TypeDef * gpio, int pin);
void GPIO_config_alternate(GPIO_TypeDef * gpio, int pin, int af);
void GPIO_write(GPIO_TypeDef * gpio, int pin, int value);
void GPIO_toggle(GPIO_TypeDef * gpio, int pin);
int GPIO_read(GPIO_TypeDef * gpio, int pin);
void GPIO_config_EXTI(GPIO_TypeDef * gpio, EXTI_line exti);
void EXTI_enable(EXTI_line exti, edge_type edge);
void EXTI_disable(EXTI_line exti);
void EXTI_clear(EXTI_line exti);
int EXTI_isset(EXTI_line exti);


#endif
