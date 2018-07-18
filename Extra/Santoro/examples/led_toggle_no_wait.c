/*
 * led_toggle.c
 *
 *  Created on: Oct 13, 2017
 *      Author: corrado
 */

#include "stm32_unict_lib.h"

int main()
{
	int last_button_state, ledstate = 0;
	// initialize ports
	GPIO_init(GPIOA);
	GPIO_init(GPIOB);

	GPIO_config_input(GPIOA, 10); // PA10 as input
	GPIO_config_output(GPIOB, 8); // PB8 as output
	GPIO_config_output(GPIOB, 9); // PB9 as output

	last_button_state = GPIO_read(GPIOA, 10);
	for (;;) {

		int current_button_state = GPIO_read(GPIOA, 10);
		if (last_button_state == 1 && current_button_state == 0) {
			ledstate = !ledstate;
			GPIO_write(GPIOB, 8, ledstate);
		}
		last_button_state = current_button_state;

		delay_ms(200);
		GPIO_toggle(GPIOB, 9);
	}
}
