/*
 * systick.cpp
 */

#include <stm32f10x.h>

#include "defines.h"
#include "systick.h"
#include "watchdog.h"

volatile uint32_t ticks;

#define SYSTICK_RESOLUTION         2000
#define SYSTICK_RESOLUTION_FACTOR  2
#define MS_TO_SYSTICK(v)           (v*SYSTICK_RESOLUTION_FACTOR)


void systick_init(void)
{
    ticks = 0;
    SysTick_Config(FCLOCK / SYSTICK_RESOLUTION);
    NVIC_SetPriority(SysTick_IRQn, 0x0f);
}

void SysTick_Handler(void)
{
    ++ticks;
}

void delay_ms(uint32_t ms)
{
    uint32_t v = ticks;
    ms *= SYSTICK_RESOLUTION_FACTOR;
    while ( (ticks - v) < ms) {
        ClrWdt();
    }
}

