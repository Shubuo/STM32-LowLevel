/*
 * adc.c
 *
 *  Created on: Oct 10, 2017
 *      Author: corrado
 */

#include "stm32_unict_lib.h"

void ADC_init(ADC_TypeDef * adc, int adc_res, int align)
{
	RCC->APB2ENR |= (1 << 8);
	adc->CR1 = adc_res << 24;
	adc->CR2 = align << 11;
}


void ADC_channel_config(ADC_TypeDef * adc, GPIO_TypeDef * port, int pin, int chan)
{
	GPIO_ANALOG(port, pin);
	// set sampling time to 480 cycles
	if (chan > 9)
		adc->SMPR1 = 7 << ((chan - 10) * 3);
	else
		adc->SMPR2 = 7 << (chan * 3);
}

void ADC_sample_channel(ADC_TypeDef * adc, int chan)
{
	adc->SQR1 = 0;
	adc->SQR3 = chan;
}

void ADC_start(ADC_TypeDef * adc)
{
	adc->CR2 |= (1 << 30);
}
