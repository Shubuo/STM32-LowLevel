/*
 * d_clock.c
 *
 *  Created on: Nov 6, 2017
 *      Author: corrado
 */

#include "stm32_unict_lib.h"

#define OFF		0
#define BLINK	1
#define STEADY	2

#define EVT_NONE	0
#define EVT_CLICK	1
#define EVT_DCLICK	2

int flash_counter = 0;

int state_off(int evt)
{
	if (evt == EVT_CLICK) {
		flash_counter = 0;
		return BLINK;
	}
	else if (evt == EVT_DCLICK)
		return STEADY;
	else {
		GPIO_write(GPIOB, 0, 0);
		return OFF;
	}
}

int state_blink(int evt)
{
	if (evt == EVT_CLICK)
		return OFF;
	else if (evt == EVT_DCLICK)
		return STEADY;
	else {
		++flash_counter;
		if (flash_counter >= 25) {
			GPIO_toggle(GPIOB, 0);
			flash_counter = 0;
		}
		return BLINK;
	}
}

int state_steady(int evt)
{
	if (evt == EVT_CLICK)
		return OFF;
	else {
		GPIO_write(GPIOB, 0, 1);
		return STEADY;
	}
}

int main(void)
{
	int state, event;
	int current_button_state, last_button_state;
	int global_timer, last_click_timer;

	GPIO_init(GPIOB);
	GPIO_config_input(GPIOB, 10);
	GPIO_config_output(GPIOB, 0);

	state = OFF;
	flash_counter = 0;
	global_timer = 0;
	last_click_timer = 0;
	last_button_state = GPIO_read(GPIOB, 10);

	for (;;) {

		delay_ms(10);
		++global_timer;

		event = EVT_NONE;
		current_button_state = GPIO_read(GPIOB, 10);
		if (last_button_state == 1 && current_button_state == 0) event = EVT_CLICK;
		last_button_state = current_button_state;

		if (event == EVT_CLICK) {
			// check double click (d_click time = 500ms)
			if ( (global_timer - last_click_timer) < 50) event = EVT_DCLICK;
			last_click_timer = global_timer;
		}

		switch (state) {
			case OFF:
				state = state_off(event);
				break;
			case BLINK:
				state = state_blink(event);
				break;
			case STEADY:
				state = state_steady(event);
				break;
		}
	}

}
