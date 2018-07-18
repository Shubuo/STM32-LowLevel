/*
 * simple_timer.c
 *
 *  Created on: Nov 11, 2017
 *      Author: corrado
 */


#include "stm32_unict_lib.h"

int main()
{
    // LED at PC3
    GPIO_init(GPIOC);
    GPIO_config_output(GPIOC, 3);

    // init the timer
    TIM_init(TIM2);

    // Configure the timebase
    // Counter clock set to 0.1 ms
    TIM_config_timebase(TIM2, 8400, 5000);

    TIM_set(TIM2, 0); // resets the counter
    TIM_on(TIM2); // starts the counter

    // infinite loop
    for (;;) {
    	if (TIM_update_check(TIM2)) {
    		GPIO_toggle(GPIOC, 3);
    		TIM_update_clear(TIM2);
    	}
    }
}

