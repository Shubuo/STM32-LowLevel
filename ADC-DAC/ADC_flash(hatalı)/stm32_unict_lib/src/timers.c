/*
 * timers.c
 *
 *  Created on: Sep 29, 2017
 *      Author: corrado
 */

#include "timers.h"

void TIM_init(TIM_TypeDef * tim)
{
    // enable timer clock
	if (tim == TIM1)
		RCC->APB2ENR |= RCC_APB2ENR_TIM1EN;
	else if (tim == TIM2)
		RCC->APB1ENR |= RCC_APB1ENR_TIM2EN;
	else if (tim == TIM3)
		RCC->APB1ENR |= RCC_APB1ENR_TIM3EN;
	else if (tim == TIM4)
		RCC->APB1ENR |= RCC_APB1ENR_TIM4EN;
//	else if (tim == TIM5)
//		RCC->APB1ENR |= RCC_APB1ENR_TIM5EN;

	tim->CR1 = 0; // timer off
	tim->SMCR = 0;
	tim->EGR = 0;
	tim->RCR = 0;
	tim->DIER = 0;
	tim->SR = 0;
}


void TIM_config_timebase(TIM_TypeDef * tim, uint16_t prescaler, uint32_t autoreload)
{
	tim->PSC = prescaler - 1;
	tim->ARR = autoreload;
	tim->CNT = 0;
}


void TIM_config_IC(TIM_TypeDef * tim, int chan, IC_Mode icmode)
{
	uint32_t pattern;
	switch (icmode) {
	case IC_RISING_EDGE:	pattern = 0b0001; break;
	case IC_FALLING_EDGE:	pattern = 0b0011; break;
	case IC_BOTH_EDGES:		pattern = 0b1011; break;
	}

	// 1:0 = 01 (IC on TIx)
	// 3:2 = 00 (no prescaler)
	// 7:4 = 0000 (no filter)
	uint32_t ccmr_pattern = 0x01;

	switch (chan) {
	case 1:
		tim->CCMR1 = (tim->CCMR1 & 0xff00) | ccmr_pattern;
		tim->CCER = (tim->CCER & 0xfff0) | pattern;
		break;
	case 2:
		tim->CCMR1 = (tim->CCMR1 & 0x00ff) | (ccmr_pattern << 8);
		tim->CCER = (tim->CCER & 0xff0f) | (pattern << 4);
		break;
	case 3:
		tim->CCMR2 = (tim->CCMR2 & 0xff00) | ccmr_pattern;
		tim->CCER = (tim->CCER & 0xf0ff) | (pattern << 8);
		break;
	case 4:
		tim->CCMR2 = (tim->CCMR2 & 0x00ff) | (ccmr_pattern << 8);
		tim->CCER = (tim->CCER & 0x0fff) | (pattern << 12);
		break;
	}
}


void TIM_config_PWM(TIM_TypeDef * tim, int chan)
{
	switch (chan) {
	case 1:
	    // output in PWM 1 MODE
	    tim->CCMR1 = (TIM_CCMR1_OC1M_2 | TIM_CCMR1_OC1M_1) | TIM_CCMR1_OC1PE;
	    tim->CCER |= TIM_CCER_CC1E;
	    break;
	case 2:
	    // output in PWM 1 MODE
	    tim->CCMR1 = (TIM_CCMR1_OC2M_2 | TIM_CCMR1_OC2M_1) | TIM_CCMR1_OC2PE;
	    tim->CCER |= TIM_CCER_CC2E;
	    break;
	case 3:
	    // output in PWM 1 MODE
	    tim->CCMR2 = (TIM_CCMR2_OC3M_2 | TIM_CCMR2_OC3M_1) | TIM_CCMR2_OC3PE;
	    tim->CCER |= TIM_CCER_CC3E;
	    break;
	case 4:
	    // output in PWM 1 MODE
	    tim->CCMR2 = (TIM_CCMR2_OC4M_2 | TIM_CCMR2_OC4M_1) | TIM_CCMR2_OC4PE;
	    tim->CCER |= TIM_CCER_CC4E;
	    break;
	}

	tim->BDTR |= TIM_BDTR_MOE;
    tim->CR1 &= ~TIM_CR1_CMS;
    //TMR->CR1 |= TIM_CR1_CEN;
    tim->EGR |= TIM_EGR_UG;

}


void TIM_enable_irq(TIM_TypeDef * tim, int irq_type)
{
	tim->DIER |= (uint16_t)irq_type;
	if (tim == TIM2)
		NVIC_EnableIRQ(TIM2_IRQn);
	else if (tim == TIM3)
		NVIC_EnableIRQ(TIM3_IRQn);
	else if (tim == TIM4)
		NVIC_EnableIRQ(TIM4_IRQn);
//	else if (tim == TIM5)
//		NVIC_EnableIRQ(TIM5_IRQn);
}

void TIM_disable_irq(TIM_TypeDef * tim, int irq_type)
{
	tim->DIER &= ~(uint16_t)irq_type;
	if (tim == TIM2)
		NVIC_DisableIRQ(TIM2_IRQn);
	else if (tim == TIM3)
		NVIC_DisableIRQ(TIM3_IRQn);
	else if (tim == TIM4)
		NVIC_DisableIRQ(TIM4_IRQn);
//	else if (tim == TIM5)
//		NVIC_DisableIRQ(TIM5_IRQn);
}
