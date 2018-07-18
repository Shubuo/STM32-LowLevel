/*
 * led_flash.c
 *
 *  Created on: Oct 11, 2017
 *      Author: corrado
 */

#include "stm32_unict_lib.h"

int main()
{
	// initialize port B
	GPIO_init(GPIOB);
	// configure pin P8 as output
	GPIO_config_output(GPIOB, 8);
	// infinite loop
	for (;;) {
		GPIO_write(GPIOB, 8, 1);
		delay_ms(500);
		GPIO_write(GPIOB, 8, 0);
		delay_ms(500);
	}
}
