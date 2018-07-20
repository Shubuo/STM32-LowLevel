/*
 * display.c
 *
 *  Created on: Oct 8, 2017
 *      Author: corrado
 */

#include "stm32_unict_lib.h"
#include <string.h>

typedef struct {
	uint8_t pattern;
	GPIO_TypeDef * port;
	int pin;
} t_segment;

uint8_t character_table[256];

// abcdefg(dp)

t_segment segment_table[8] = {
		{ 0x80, GPIOC, 9}, 	// pin "A"
		{ 0x40, GPIOC, 10}, // pin "B"
		{ 0x20, GPIOC, 8},  // pin "C"
		{ 0x10, GPIOD, 2},  // pin "D"
		{ 0x08, GPIOA, 6},  // pin "E"
		{ 0x04, GPIOC, 6},  // pin "F"
		{ 0x02, GPIOC, 11}, // pin "G"
		{ 0x01, GPIOA, 7},  // pin "DP"
};

t_segment cathode_table[4] = {
		{ 0, GPIOB, 12},
		{ 0, GPIOA, 11},
		{ 0, GPIOC, 5},
		{ 0, GPIOA, 12}
};

int current_scheduled_digit = 0;
char current_digit_pattern[4] = { 0, 0, 0, 0};

static void setup_character_table(void)
{
	memset(character_table, 0, 256);
	character_table['"'] = 0x44; // 0100 0100
	character_table['\''] = 0x04; // 0000 0100
	character_table['-'] = 0x02; // 0000 0010
	character_table['0'] = 0xfc; // 1111 1100
	character_table['1'] = 0x60; // 0110 0000
	character_table['2'] = 0xda; // 1101 1010
	character_table['3'] = 0xf2; // 1111 0010
	character_table['4'] = 0x66; // 0110 0110
	character_table['5'] = 0xb6; // 1011 0110
	character_table['6'] = 0xbe; // 1011 1110
	character_table['7'] = 0xe0; // 1110 0000
	character_table['8'] = 0xfe; // 1111 1110
	character_table['9'] = 0xf6; // 1111 0110
	character_table['='] = 0x12; // 0001 0010
	character_table['A'] = 0xee; // 1110 1110
	character_table['a'] = 0xfa; // 1011 1110
	character_table['B'] = 0x3e; // 0011 1110
	character_table['b'] = 0x3e; // 0011 1110
	character_table['C'] = 0x9c; // 1001 1100
	character_table['c'] = 0x1a; // 0001 1010
	character_table['D'] = 0x7a; // 0111 1010
	character_table['d'] = 0x7a; // 0111 1010
	character_table['E'] = 0x9e; // 1001 1110
	character_table['e'] = 0x9e; // 1001 1110
	character_table['F'] = 0x8e; // 1000 1110
	character_table['f'] = 0x8e; // 1000 1110
	character_table['G'] = 0xbc; // 1011 1100
	character_table['g'] = 0xf6; // 1111 0110
	character_table['H'] = 0x6e; // 0110 1110
	character_table['h'] = 0x2e; // 0010 1110
	character_table['I'] = 0x60; // 0110 0000
	character_table['i'] = 0x20; // 0010 0000
	character_table['J'] = 0x70; // 0111 0000
	character_table['j'] = 0x30; // 0011 0000
	character_table['K'] = 0x6e; // 0110 1110
	character_table['k'] = 0x2e; // 0010 1110
	character_table['L'] = 0x1c; // 0001 1100
	character_table['l'] = 0x1c; // 0001 1100
	character_table['N'] = 0x2a; // 0010 1010
	character_table['n'] = 0x2a; // 0010 1010
	character_table['O'] = 0xfc; // 1111 1100
	character_table['o'] = 0x3a; // 0011 1010
	character_table['P'] = 0xce; // 1100 1110
	character_table['p'] = 0xce; // 1100 1110
	character_table['Q'] = 0xe6; // 1110 0110
	character_table['q'] = 0xe6; // 1110 0110
	character_table['R'] = 0x0a; // 0000 1010
	character_table['r'] = 0x0a; // 0000 1010
	character_table['S'] = 0xb6; // 1011 0110
	character_table['s'] = 0xb6; // 1011 0110
	character_table['T'] = 0x1e; // 0001 1110
	character_table['t'] = 0x1e; // 0001 1110
	character_table['U'] = 0x7c; // 0111 1100
	character_table['u'] = 0x38; // 0011 1000
	character_table['Z'] = 0xda; // 1101 1010
	character_table['z'] = 0xda; // 1101 1010
}

void DISPLAY_init(void)
{
	int i;
	for (i = 0; i < 8; i++) {
		GPIO_init(segment_table[i].port);
		GPIO_config_output(segment_table[i].port, segment_table[i].pin);
		GPIO_write(segment_table[i].port,segment_table[i].pin, 0);
	}
	for (i = 0; i < 4; i++) {
		GPIO_init(cathode_table[i].port);
		GPIO_config_output(cathode_table[i].port, cathode_table[i].pin);
		GPIO_write(cathode_table[i].port, cathode_table[i].pin, 0);
	}
	setup_character_table();
	TIM_init(TIM5);
	TIM_config_timebase(TIM5, 42000, 1);
	TIM_enable_irq(TIM5, IRQ_UPDATE);
	TIM_on(TIM5);
}

static void DISPLAY_set(uint8_t bit_pattern)
{
	int i;
	for (i = 0;i < 8;i++) {
		t_segment * s = segment_table + i;
		if (bit_pattern & s->pattern)
			GPIO_write(s->port,s->pin, 1);
		else
			GPIO_write(s->port,s->pin, 0);
	}
}

static void digit_on_off(int digit, int value)
{
	GPIO_write(cathode_table[digit].port, cathode_table[digit].pin, value);
}

void TIM5_IRQHandler(void)
{
	digit_on_off(current_scheduled_digit, 0);
	current_scheduled_digit = (current_scheduled_digit + 1) % 4;
	DISPLAY_set(current_digit_pattern[current_scheduled_digit]);
	digit_on_off(current_scheduled_digit, 1);
	TIM_update_clear(TIM5);
}


void DISPLAY_putc(int digit, char c)
{
	char dp = current_digit_pattern[digit] & 1;
	current_digit_pattern[digit] = character_table[(int)c] | dp;
}


void DISPLAY_dp(int digit, char dp)
{
	char c = current_digit_pattern[digit] & 0xfe;
	current_digit_pattern[digit] = c | dp;
}


void DISPLAY_puts(int digit, char * s)
{
	int i;
	for (i = 0;i < strlen(s);i++)
		DISPLAY_putc(digit+i,s[i]);
}
