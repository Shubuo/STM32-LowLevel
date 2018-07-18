/*
 * button_exti.c
 *
 *  Created on: Nov 12, 2017
 *      Author: corrado
 */


#include "stm32_unict_lib.h"

int main()
{
    // LED at PC3
    GPIO_init(GPIOC);
    GPIO_config_output(GPIOC, 3);

    // pushbutton Y (PB4)
    GPIO_init(GPIOB);
    GPIO_config_input(GPIOB, 4);

    GPIO_config_EXTI(GPIOB, EXTI4);
    EXTI_enable(EXTI4, RISING_EDGE);

    for (;;) {
    	/*
    	 * do nothing
    	 */
    }
}

void EXTI4_IRQHandler(void)
{
	if (EXTI_isset(EXTI4)) {
		GPIO_toggle(GPIOC, 3);
		EXTI_clear(EXTI4);
	}
}
