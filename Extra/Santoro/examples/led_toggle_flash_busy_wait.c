/*
 * led_toggle.c
 *
 *  Created on: Oct 13, 2017
 *      Author: corrado
 */

#include "stm32_unict_lib.h"

int main()
{
	int ledstate = 0;
	// initialize ports
	GPIO_init(GPIOA);
	GPIO_init(GPIOB);

	GPIO_config_input(GPIOA, 10); // PA10 as input
	GPIO_config_output(GPIOB, 8); // PB8 as output
	GPIO_config_output(GPIOB, 9); // PB9 as output
	// infinite loop
	for (;;) {
		if (GPIO_read(GPIOA, 10) == 0) {
			ledstate = !ledstate;
			GPIO_write(GPIOB, 8, ledstate);
			while (GPIO_read(GPIOA, 10) == 0) {} // wait
		}
		delay_ms(200);
		GPIO_toggle(GPIOB, 9);
	}
}
