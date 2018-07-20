/*
 * adc_test.c
 *
 *  Created on: Dec 3, 2017
 *      Author: corrado
 */

#include <stdio.h>
#include "stm32_unict_lib.h"

int new_arr_value = 100;

void main(void)
{
	GPIO_init(GPIOC);
	GPIO_config_output(GPIOC, 2);

	ADC_init(ADC2, ADC_RES_8, ADC_ALIGN_RIGHT);
	ADC_channel_config(ADC2, GPIOC, 3, 10);
	ADC_on(ADC1);
	ADC_sample_channel(ADC2, 10);

	TIM_init(TIM3);
	TIM_config_timebase(TIM3, 42000, 100);
	TIM_set(TIM3, 0);
	TIM_enable_irq(TIM3, IRQ_UPDATE);
	TIM_on(TIM3);

	for (;;) {
		ADC_start(ADC2);
		while (!ADC_completed(ADC2)) {}
		int value = ADC_read(ADC2);
		new_arr_value = value * 900/255 + 100;
	}
}

void TIM3_IRQHandler(void)
{
	if (TIM_update_check(TIM3)) {
		GPIO_toggle(GPIOB, 0);
		TIM_update_clear(TIM3);
		TIM3->ARR = new_arr_value;
		// update the autoreload register with new value
	}
}
