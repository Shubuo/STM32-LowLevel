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
	// configure pin PB9 as output
	GPIO_config_output(GPIOB, 9);

	// infinite loop
	for (;;) {
		int pinval = (GPIOA->IDR & (1 << 10)) != 0;
		if (pinval == 0)
			GPIOB->ODR |= (1 << 8); // PB8 = 1
		else
			GPIOB->ODR &= ~(int32_t)(1 << 8); // PB8 = 0
	}
}
