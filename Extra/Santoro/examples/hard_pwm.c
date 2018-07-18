/*
 * hard_pwm.c
 *
 *  Created on: Dec 3, 2017
 *      Author: corrado
 */

#include <stdio.h>
#include "stm32_unict_lib.h"

#define PERIOD_PWM   1000

int main(void)
{
	DISPLAY_init();

	GPIO_init(GPIOB);
	GPIO_config_alternate(GPIOB, 0, 2); // AF2 --> TIM3, TIM4, TIM5

	ADC_init(ADC1, ADC_RES_8, ADC_ALIGN_RIGHT);
	ADC_channel_config(ADC1, GPIOC, 0, 10);
	ADC_on(ADC1);
	ADC_sample_channel(ADC1, 10);

	TIM_init(TIM3);
	TIM_config_timebase(TIM3, 420, PERIOD_PWM);
	TIM_set(TIM3, 0);
	TIM_config_PWM(TIM3, 3);
	TIM_on(TIM3);

	for (;;) {
		ADC_start(ADC1);
		while (!ADC_completed(ADC1)) {}
		int value = ADC_read(ADC1);
		int t_on = (value * 1000)/255;
		TIM3->CCR3 = t_on;
		char s[4];
		sprintf(s, "%4d", t_on); // we will display the milliseconds
		DISPLAY_puts(0,s);
	}
	return 0;
}
