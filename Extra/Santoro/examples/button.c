/*
 * button.c
 *
 *  Created on: Oct 11, 2017
 *      Author: corrado
 */

#include "stm32_unict_lib.h"

int main()
{
	// pushbutton on PA10; LED on PB8
	// initialize ports
	GPIO_init(GPIOA);
	GPIO_init(GPIOB);

	// configure pin PA10 as input
	GPIO_config_input(GPIOA, 10);
	// configure pin PB8 as output
	GPIO_config_output(GPIOB, 8);

	// infinite loop
	for (;;) {
		int pinval = GPIO_read(GPIOA, 10);
		GPIO_write(GPIOB, 8, !pinval);
	}
}
