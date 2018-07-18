/*
 * display_test.c
 *
 *  Created on: Nov 20, 2017
 *      Author: corrado
 */

#include "stm32_unict_lib.h"
#include <stdio.h>

volatile int counter = 0;

int main()
{
	// Init the display
	TIM_init(TIM2);
	DISPLAY_init();
	// init the timer 2 and configure the timebase
	// tick set to 0.1 ms
	TIM_config_timebase(TIM2, 8400, 1000);
	TIM_set(TIM2, 0);
	TIM_on(TIM2);
	TIM_enable_irq(TIM2, IRQ_UPDATE);
	for (;;) {
		char s[5];
		sprintf(s,"%4d", counter); // put counter value to string
		DISPLAY_puts(0, s); // update the display
	}
}

void TIM2_IRQHandler(void)
{
	++counter;
	TIM_update_clear(TIM2);
}
