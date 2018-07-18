/*
 * key_driver.c
 *
 *  Created on: Oct 13, 2017
 *      Author: corrado
 */

#include "stm32_unict_lib.h"

#define KEY_NONE		-1

#define KEY_PRESS_BASE	0
#define KEY_PRESS_X 	0
#define KEY_PRESS_Y 	1
#define KEY_PRESS_Z 	2
#define KEY_PRESS_T 	3

#define KEY_RELEASE_BASE	10
#define KEY_RELEASE_X 	10
#define KEY_RELEASE_Y 	11
#define KEY_RELEASE_Z 	12
#define KEY_RELEASE_T 	13

#define NUMBER_OF_KEYS	4

struct key_state {
	GPIO_TypeDef * port;
	int pin;
	int last_state;
} keyboard[NUMBER_OF_KEYS] = { 	{GPIOB, 10, 1},
								{GPIOB, 4, 1},
								{GPIOB, 5, 1},
								{GPIOA, 10, 1} };
// X = PB10
// Y = PB4
// Z = PB5
// T = PA10

void key_init(void)
{
	int i;
	for (i = 0;i < NUMBER_OF_KEYS;i++) {
		GPIO_init(keyboard[i].port);
		GPIO_config_input(keyboard[i].port, keyboard[i].pin);
		keyboard[i].last_state = GPIO_read(keyboard[i].port, keyboard[i].pin);
	}
}

int key_read(void)
{
	int i, key_event = KEY_NONE;

	for (i = 0;i < NUMBER_OF_KEYS;i++) {
		int curr_state = GPIO_read(keyboard[i].port, keyboard[i].pin);
		if (keyboard[i].last_state == 1 && curr_state == 0) key_event = KEY_PRESS_BASE + i;
		if (keyboard[i].last_state == 0 && curr_state == 1) key_event = KEY_RELEASE_BASE + i;
		keyboard[i].last_state = curr_state;
	}

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
		case KEY_PRESS_Y:
			GPIO_write(GPIOB, 8, 1);
			GPIO_write(GPIOB, 9, 1);
			break;
		case KEY_PRESS_Z:
			GPIO_write(GPIOB, 8, 0);
			GPIO_write(GPIOB, 9, 0);
			break;
		case KEY_RELEASE_T:
			GPIO_toggle(GPIOB, 9);
			break;
		}
	}
}
