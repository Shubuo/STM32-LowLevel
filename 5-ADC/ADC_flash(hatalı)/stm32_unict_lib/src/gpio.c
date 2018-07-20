/*
 * gpio.c
 *
 *  Created on: Sep 28, 2017
 *      Author: corrado
 */

#include "gpio.h"

void GPIO_init(GPIO_TypeDef * gpio)
{
	if (gpio == GPIOA)
		RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN;
	else if (gpio == GPIOB)
		RCC->AHB1ENR |= RCC_AHB1ENR_GPIOBEN;
	else if (gpio == GPIOC)
		RCC->AHB1ENR |= RCC_AHB1ENR_GPIOCEN;
	else if (gpio == GPIOD)
		RCC->AHB1ENR |= RCC_AHB1ENR_GPIODEN;
	else if (gpio == GPIOE)
		RCC->AHB1ENR |= RCC_AHB1ENR_GPIOEEN;
	else if (gpio == GPIOH)
		RCC->AHB1ENR |= RCC_AHB1ENR_GPIOHEN;
}

void GPIO_config_input(GPIO_TypeDef * gpio, int pin)
{
	GPIO_INPUT(gpio,pin);
	GPIO_NO_PUPD(gpio,pin);
	GPIO_SPEED_HIGH(gpio,pin);
}

void GPIO_config_output(GPIO_TypeDef * gpio, int pin)
{
	GPIO_OUTPUT(gpio,pin);
	GPIO_NO_PUPD(gpio,pin);
	GPIO_PUSH_PULL(gpio,pin);
	GPIO_SPEED_HIGH(gpio,pin);
}

void GPIO_config_alternate(GPIO_TypeDef * gpio, int pin, int af)
{
	GPIO_ALTERNATE(gpio,pin);
	GPIO_NO_PUPD(gpio,pin);
	GPIO_PUSH_PULL(gpio,pin);
	GPIO_SPEED_HIGH(gpio,pin);
	if (pin < 8) {
		GPIO_AFL(gpio,pin,af);
	}
	else {
		GPIO_AFH(gpio,pin,af);
	}
}

void GPIO_write(GPIO_TypeDef * gpio, int pin, int value)
{
	if (value) {
		GPIO_ON(gpio,pin);
	}
	else {
		GPIO_OFF(gpio,pin);
	}
}

void GPIO_toggle(GPIO_TypeDef * gpio, int pin)
{
	if (GPIO_OVAL(gpio,pin)) {
		GPIO_OFF(gpio,pin);
	}
	else {
		GPIO_ON(gpio,pin);
	}
}

int GPIO_read(GPIO_TypeDef * gpio, int pin)
{
	return GPIO_VAL(gpio,pin);
}


void GPIO_config_EXTI(GPIO_TypeDef * gpio, EXTI_line exti)
{
	int pat;
	if (gpio == GPIOA) pat = 0x00;
	else if (gpio == GPIOB) pat = 0x01;
	else if (gpio == GPIOC) pat = 0x02;
	else if (gpio == GPIOD) pat = 0x03;
	else if (gpio == GPIOE) pat = 0x04;
	else if (gpio == GPIOH) pat = 0x07;
	else return;

	RCC->APB2ENR |= RCC_APB2ENR_SYSCFGEN;

	switch (exti) {
	case EXTI0:
		SYSCFG->EXTICR[0] = (SYSCFG->EXTICR[0] & ~(uint32_t)0x000f) | pat;
		break;
	case EXTI1:
		SYSCFG->EXTICR[0] = (SYSCFG->EXTICR[0] & ~(uint32_t)0x00f0) | (pat << 4);
		break;
	case EXTI2:
		SYSCFG->EXTICR[0] = (SYSCFG->EXTICR[0] & ~(uint32_t)0x0f00) | (pat << 8);
		break;
	case EXTI3:
		SYSCFG->EXTICR[0] = (SYSCFG->EXTICR[0] & ~(uint32_t)0xf000) | (pat << 12);
		break;

	case EXTI4:
		SYSCFG->EXTICR[1] = (SYSCFG->EXTICR[1] & ~(uint32_t)0x000f) | pat;
		break;
	case EXTI5:
		SYSCFG->EXTICR[1] = (SYSCFG->EXTICR[1] & ~(uint32_t)0x00f0) | (pat << 4);
		break;
	case EXTI6:
		SYSCFG->EXTICR[1] = (SYSCFG->EXTICR[1] & ~(uint32_t)0x0f00) | (pat << 8);
		break;
	case EXTI7:
		SYSCFG->EXTICR[1] = (SYSCFG->EXTICR[1] & ~(uint32_t)0xf000) | (pat << 12);
		break;

	case EXTI8:
		SYSCFG->EXTICR[2] = (SYSCFG->EXTICR[2] & ~(uint32_t)0x000f) | pat;
		break;
	case EXTI9:
		SYSCFG->EXTICR[2] = (SYSCFG->EXTICR[2] & ~(uint32_t)0x00f0) | (pat << 4);
		break;
	case EXTI10:
		SYSCFG->EXTICR[2] = (SYSCFG->EXTICR[2] & ~(uint32_t)0x0f00) | (pat << 8);
		break;
	case EXTI11:
		SYSCFG->EXTICR[2] = (SYSCFG->EXTICR[2] & ~(uint32_t)0xf000) | (pat << 12);
		break;

	case EXTI12:
		SYSCFG->EXTICR[3] = (SYSCFG->EXTICR[3] & ~(uint32_t)0x000f) | pat;
		break;
	case EXTI13:
		SYSCFG->EXTICR[3] = (SYSCFG->EXTICR[3] & ~(uint32_t)0x00f0) | (pat << 4);
		break;
	case EXTI14:
		SYSCFG->EXTICR[3] = (SYSCFG->EXTICR[3] & ~(uint32_t)0x0f00) | (pat << 8);
		break;
	case EXTI15:
		SYSCFG->EXTICR[3] = (SYSCFG->EXTICR[3] & ~(uint32_t)0xf000) | (pat << 12);
		break;
	}
	int val = SYSCFG->EXTICR[1];
}


void EXTI_enable(EXTI_line exti, edge_type edge)
{
	switch (edge) {
	case RISING_EDGE:
		EXTI->FTSR = EXTI->FTSR & ~(uint32_t)(1 << exti);
		EXTI->RTSR = EXTI->RTSR | (1 << exti);
		EXTI->IMR |= (1 << exti);
		//EXTI->EMR |= (1 << exti);
		break;
	case FALLING_EDGE:
		EXTI->RTSR = EXTI->RTSR & ~(uint32_t)(1 << exti);
		EXTI->FTSR = EXTI->FTSR | (1 << exti);
		EXTI->IMR |= (1 << exti);
		//EXTI->EMR |= (1 << exti);
		break;
	case BOTH_EDGES:
		EXTI->RTSR = EXTI->RTSR | (1 << exti);
		EXTI->FTSR = EXTI->FTSR | (1 << exti);
		EXTI->IMR |= (1 << exti);
		//EXTI->EMR |= (1 << exti);
		break;
	}
	switch (exti) {
	case EXTI0:	NVIC_EnableIRQ(EXTI0_IRQn);break;
	case EXTI1:	NVIC_EnableIRQ(EXTI1_IRQn);break;
	case EXTI2:	NVIC_EnableIRQ(EXTI2_IRQn);break;
	case EXTI3:	NVIC_EnableIRQ(EXTI3_IRQn);break;
	case EXTI4:	NVIC_EnableIRQ(EXTI4_IRQn);break;
	case EXTI5:
	case EXTI6:
	case EXTI7:
	case EXTI8:
	case EXTI9:	NVIC_EnableIRQ(EXTI9_5_IRQn);break;
	case EXTI10:
	case EXTI11:
	case EXTI12:
	case EXTI13:
	case EXTI14:
	case EXTI15: NVIC_EnableIRQ(EXTI15_10_IRQn);break;
	}
}


void EXTI_disable(EXTI_line exti)
{
	EXTI->IMR &= ~(uint32_t)(1 << exti);
	switch (exti) {
	case EXTI0:	NVIC_DisableIRQ(EXTI0_IRQn);break;
	case EXTI1:	NVIC_DisableIRQ(EXTI1_IRQn);break;
	case EXTI2:	NVIC_DisableIRQ(EXTI2_IRQn);break;
	case EXTI3:	NVIC_DisableIRQ(EXTI3_IRQn);break;
	case EXTI4:	NVIC_DisableIRQ(EXTI4_IRQn);break;
	case EXTI5:
	case EXTI6:
	case EXTI7:
	case EXTI8:
	case EXTI9:	NVIC_DisableIRQ(EXTI9_5_IRQn);break;
	case EXTI10:
	case EXTI11:
	case EXTI12:
	case EXTI13:
	case EXTI14:
	case EXTI15: NVIC_DisableIRQ(EXTI15_10_IRQn);break;
	}
}

void EXTI_clear(EXTI_line exti)
{
	EXTI->PR = (1 << exti);
}

int EXTI_isset(EXTI_line exti)
{
	return (EXTI->PR & (1 << exti)) != 0;

}

