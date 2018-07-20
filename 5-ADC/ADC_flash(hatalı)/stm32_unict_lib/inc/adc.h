/*
 * adc.h
 */

#ifndef __ADCX_H
#define __ADCX_H

#include <stm32f10x.h>

// adc resolution
#define ADC_RES_12	0
#define ADC_RES_10	1
#define ADC_RES_8	2
#define ADC_RES_6	3

// adc alignment
#define ADC_ALIGN_RIGHT	0
#define ADC_ALIGN_LEFT	1

void ADC_init(ADC_TypeDef * adc, int adc_res, int align);
void ADC_channel_config(ADC_TypeDef * adc, GPIO_TypeDef * port, int pin, int chan);
void ADC_sample_channel(ADC_TypeDef * adc, int chan);
void ADC_start(ADC_TypeDef * adc);

#define ADC_on(adc) { adc->CR2 |= 1; }
#define ADC_off(adc) { adc->CR2 &= ~(uint32_t)1; }

#define ADC_completed(adc)	((adc->SR & 2) != 0)
#define ADC_read(adc)	adc->DR

#endif
