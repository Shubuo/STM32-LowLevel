/*
 * reaction_game.c
 *
 *  Created on: Dec 3, 2017
 *      Author: corrado
 */

#include <stdio.h>
#include "stm32_unict_lib.h"

int measure_done, timeout;

int main(void)
{
	DISPLAY_init();
	GPIO_init(GPIOB);
	GPIO_config_output(GPIOB, 0); // the LED
	GPIO_config_alternate(GPIOB, 4, 2); // button "Y" as AF2 --> TIM3,CH1

	TIM_init(TIM3);
	TIM_config_timebase(TIM3, 8400, 50000); // prescaler at 100us timeout of 5 secs
	TIM_config_IC(TIM3, 1, IC_FALLING_EDGE); // capture on falling-edge (pushbutton)
	TIM_set(TIM3, 0);
	TIM_on(TIM3);
	TIM_enable_irq(TIM3, IRQ_UPDATE | IRQ_CC_1);

	for (;;) {
		delay_ms(3000); // wait 3 seconds to start the game
		TIM_set(TIM3, 0); // clear timer and flags
		measure_done = 0;
		timeout = 0;
		GPIO_write(GPIOB, 0, 1); // turn on LED
		DISPLAY_puts(0, "    ");
		while (!measure_done && !timeout) ; // wait for capture or timeout
		if (measure_done) {
			// capture OK!
			int ms = TIM3->CCR1 / 10;
			char s[4];
			sprintf(s,"%4d", ms);
			DISPLAY_puts(0, s);
		}
		else {
			// timeout
			DISPLAY_puts(0, "tout");
		}
	}
}

void TIM3_IRQHandler(void)
{
	if (TIM_update_check(TIM3)) {
		// period match, timeout
		GPIO_write(GPIOB, 0, 0);
		timeout = 1;
		TIM_update_clear(TIM3);
	}
	if (TIM_cc_check(TIM3,1)) {
		// capture
		GPIO_write(GPIOB, 0, 0);
		measure_done = 1;
		TIM_cc_clear(TIM3,1);
	}
}
