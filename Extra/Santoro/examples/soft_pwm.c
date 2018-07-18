/*
 * soft_pwm.c
 *
 *  Created on: Dec 3, 2017
 *      Author: corrado
 */

#include <stdio.h>
#include "stm32_unict_lib.h"

#define PERIOD_PWM   100

int t_on, t_off;

void main(void)
{
	DISPLAY_init();
	GPIO_init(GPIOB);
	GPIO_config_output(GPIOB, 0);

	ADC_init(ADC1, ADC_RES_8, ADC_ALIGN_RIGHT);
	ADC_channel_config(ADC1, GPIOC, 0, 10);
	ADC_on(ADC1);
	ADC_sample_channel(ADC1, 10);

	t_on = 50;
	t_off = PERIOD_PWM - t_on;

	TIM_init(TIM2);
	TIM_config_timebase(TIM2, 4200, t_on);
	TIM_set(TIM2, 0);
	TIM_enable_irq(TIM2, IRQ_UPDATE);
	TIM_on(TIM2);

	for (;;) {
		ADC_start(ADC1);
		while (!ADC_completed(ADC1)) {}
		int value = ADC_read(ADC1);
		t_on = (value * 100)/255;
		if (t_on == 0) t_on = 1;
		if (t_on == 100) t_on = 99;
		t_off = PERIOD_PWM - t_on;
		char s[4];
		sprintf(s, "%4d", t_on); // we will display the milliseconds
		DISPLAY_puts(0,s);
	}
}

void TIM2_IRQHandler(void)
{
	if (TIM_update_check(TIM2)) {
		TIM_update_clear(TIM2);
		if (GPIO_read(GPIOB, 0) == 0) {
			GPIO_write(GPIOB, 0, 1);
			TIM2->ARR = t_on;
		}
		else {
			GPIO_write(GPIOB, 0, 0);
			TIM2->ARR = t_off;
		}
	}
}
