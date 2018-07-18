/*
 * ascensore.c
 *
 *  Created on: Jan 10, 2018
 *      Author: corrado
 */


#include "stm32_unict_lib.h"
#include <stdio.h>

#define IDLE		0
#define ATTESA		1
#define CHIUSURA	2
#define MARCIA		3
#define APERTURA	4

int stato = IDLE;
int piano_corrente = 0;
int piano_target;
int time_counter;
int tempo_attesa = 4;
int tempo_per_mezzo_piano = 2;

void main(void)
{
	DISPLAY_init();
	CONSOLE_init();
	ADC_init(ADC1, ADC_RES_6, ADC_ALIGN_RIGHT);
	ADC_channel_config(ADC1, GPIOC, 0, 10);
	ADC_channel_config(ADC1, GPIOC, 1, 11);
	ADC_on(ADC1);

	GPIO_init(GPIOB);
	GPIO_init(GPIOC);

	GPIO_config_output(GPIOB, 0); // red led
	GPIO_config_output(GPIOC, 2); // yellow led
	GPIO_config_output(GPIOC, 3); // green led

	GPIO_config_input(GPIOB, 10); // "X", piano 2
	GPIO_config_EXTI(GPIOB, EXTI10);
	EXTI_enable(EXTI10, FALLING_EDGE);

	GPIO_config_input(GPIOB, 4); // "Y", piano 1
	GPIO_config_EXTI(GPIOB, EXTI4);
	EXTI_enable(EXTI4, FALLING_EDGE);

	GPIO_config_input(GPIOB, 5); // "Z", piano 0
	GPIO_config_EXTI(GPIOB, EXTI5);
	EXTI_enable(EXTI5, FALLING_EDGE);

	TIM_init(TIM2);
	TIM_config_timebase(TIM2, 8400, 2500);
	TIM_set(TIM2, 0);
	TIM_enable_irq(TIM2, IRQ_UPDATE);
	TIM_on(TIM2);

	for (;;) {
		char s[3];
		sprintf(s,"%d%c",
				piano_corrente/2,
				((piano_corrente %2) == 0) ? ' ' : '-' );
		DISPLAY_puts(2, s);
		delay_ms(100);
		switch(stato) {
		case IDLE:
			ADC_sample_channel(ADC1, 10);
			ADC_start(ADC1);
			while (!ADC_completed(ADC1)) {}
			tempo_per_mezzo_piano = 1 + (7 * ADC_read(ADC1)) / 63;

			ADC_sample_channel(ADC1, 11);
			ADC_start(ADC1);
			while (!ADC_completed(ADC1)) {}
			tempo_attesa = 2 + (10 * ADC_read(ADC1)) / 63;

			printf("Piano corrente = %d, tempo_attesa = %f, tempo_per_mezzo_piano = %f\n",
					piano_corrente/2,
					tempo_attesa * 0.25,
					tempo_per_mezzo_piano * 0.25);

			break;
		case ATTESA: printf("ATTESA\n");break;
		case CHIUSURA: printf("CHIUSURA PORTE\n");break;
		case MARCIA: printf("MARCIA, piano corrente = %d, piano target = %d\n",
							piano_corrente/2, piano_target/2);break;
		case APERTURA: printf("APERTURA PORTE\n");break;
		}

	}

}

void TIM2_IRQHandler(void)
{
	if (stato == IDLE)
		time_counter = 0;
	else {
		++time_counter;
		switch (stato) {
		case ATTESA:
			if (time_counter >= tempo_attesa) {
				time_counter = 0;
				stato = CHIUSURA;
			}
			break;
		case CHIUSURA:
			GPIO_toggle(GPIOC, 2);
			if (time_counter >= 6) {
				time_counter = 0;
				stato = MARCIA;
				GPIO_write(GPIOC, 2, 0);
			}
			break;
		case MARCIA:
			GPIO_toggle(GPIOB, 0);
			if (time_counter >= tempo_per_mezzo_piano) {
				time_counter = 0;
				if (piano_corrente == piano_target) {
					stato = APERTURA;
					GPIO_write(GPIOB, 0, 0);
				}
				else if (piano_corrente < piano_target) {
					++piano_corrente;
				}
				else {
					--piano_corrente;
				}
			}
			break;
		case APERTURA:
			GPIO_toggle(GPIOC, 3);
			if (time_counter >= 6) {
				time_counter = 0;
				stato = IDLE;
				GPIO_write(GPIOC, 3, 0);
			}
			break;
		}
	}
	TIM_update_clear(TIM2);
}

// piano 2
void EXTI15_10_IRQHandler(void)
{
	if (EXTI_isset(EXTI10)) {
		if (stato == IDLE) {
			stato = ATTESA;
			piano_target = 4;
		}
		EXTI_clear(EXTI10);
	}
}

// piano 1
void EXTI4_IRQHandler(void)
{
	if (EXTI_isset(EXTI4)) {
		if (stato == IDLE) {
			stato = ATTESA;
			piano_target = 2;
		}
		EXTI_clear(EXTI4);
	}
}

// piano 0
void EXTI9_5_IRQHandler(void)
{
	if (EXTI_isset(EXTI5)) {
		if (stato == IDLE) {
			stato = ATTESA;
			piano_target = 0;
		}
		EXTI_clear(EXTI5);
	}
}
