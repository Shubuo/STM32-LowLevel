/*
 * key_driver.c
 *
 *  Created on: Oct 13, 2017
 *      Author: corrado
 */

#include "stm32_unict_lib.h"

#define KEY_NONE		-1

#define KEY_PRESS_X 	0
#define KEY_PRESS_Y 	1
#define KEY_PRESS_Z 	2
#define KEY_PRESS_T 	3

#define KEY_RELEASE_X 	10
#define KEY_RELEASE_Y 	11
#define KEY_RELEASE_Z 	12
#define KEY_RELEASE_T 	13

int last_X_state, last_Y_state, last_Z_state, last_T_state;

// X = PB10
// Y = PB4
// Z = PB5
// T = PA10
void key_init(void)
{
	GPIO_init(GPIOA);
	GPIO_init(GPIOB);
	GPIO_config_input(GPIOA, 10);
	GPIO_config_input(GPIOB, 5);
	GPIO_config_input(GPIOB, 4);
	GPIO_config_input(GPIOB, 10);
	last_X_state = GPIO_read(GPIOB, 10);
	last_Y_state = GPIO_read(GPIOB, 4);
	last_Z_state = GPIO_read(GPIOB, 5);
	last_T_state = GPIO_read(GPIOA, 10);
}

int key_read(void)
{
	int current_state, key_event = KEY_NONE;

	current_state = GPIO_read(GPIOB, 10); // read "X"
	if (last_X_state == 1 && current_state == 0) key_event = KEY_PRESS_X;
	if (last_X_state == 0 && current_state == 1) key_event = KEY_RELEASE_X;
	last_X_state = current_state;

	current_state = GPIO_read(GPIOB, 4); // read "Y"
	if (last_Y_state == 1 && current_state == 0) key_event = KEY_PRESS_Y;
	if (last_Y_state == 0 && current_state == 1) key_event = KEY_RELEASE_Y;
	last_Y_state = current_state;

	current_state = GPIO_read(GPIOB, 5); // read "Z"
	if (last_Z_state == 1 && current_state == 0) key_event = KEY_PRESS_Z;
	if (last_Z_state == 0 && current_state == 1) key_event = KEY_RELEASE_Z;
	last_Z_state = current_state;

	current_state = GPIO_read(GPIOA, 10); // read "T"
	if (last_T_state == 1 && current_state == 0) key_event = KEY_PRESS_T;
	if (last_T_state == 0 && current_state == 1) key_event = KEY_RELEASE_T;
	last_T_state = current_state;

	return key_event;
}

int main(void)
{
	key_init();

	GPIO_config_output(GPIOB, 8);
	GPIO_config_output(GPIOB, 9);

	for (;;) {
		switch (key_read()) {
		case KEY_PRESS_X:
			GPIO_toggle(GPIOB, 8);
			break;
		case KEY_RELEASE_T:
			GPIO_toggle(GPIOB, 9);
			break;
		}
	}
}
