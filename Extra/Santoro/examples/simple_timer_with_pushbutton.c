/*
 * simple_timer_with_pushbutton.c
 *
 *  Created on: Nov 11, 2017
 *      Author: corrado
 */


#include "stm32_unict_lib.h"

int main()
{
	int last_key_state, flashing = 0;

    // LED at PC3
    GPIO_init(GPIOC);
    GPIO_config_output(GPIOC, 3);

    // pushbutton X (PB10)
    GPIO_init(GPIOB);
    GPIO_config_input(GPIOB, 10);

    // init the timer
    TIM_init(TIM2);

    // Configure the timebase
    // Counter clock set to 0.1 ms
    TIM_config_timebase(TIM2, 8400, 5000);

    TIM_set(TIM2, 0); // resets the counter
    TIM_on(TIM2); // starts the counter

    last_key_state = GPIO_read(GPIOB, 10);
    for (;;) {
    	int current_key_state = GPIO_read(GPIOB, 10);
    	if (last_key_state == 1 && current_key_state == 0) flashing = !flashing;
    	last_key_state = current_key_state;

    	if (TIM_update_check(TIM2)) {
    		if (flashing)
    			GPIO_toggle(GPIOC, 3);
    		else
    			GPIO_write(GPIOC,3, 0);
    		TIM_update_clear(TIM2);
    	}
    }
}

