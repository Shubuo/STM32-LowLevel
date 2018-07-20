/*
 * watchdog.h
 */

#ifndef __WATCHDOG_H
#define __WATCHDOG_H

#include <stm32f4xx.h>

/*
 * Watchdog setup
 * APB1 = 42MHz, P ~= 24 ns
 * Twd = 0.024 * 4096 * 8 * (12 + 1) ~= 10000 us  = 10 ms
 * Twd = 0.024 * 4096 * 8 * (0x3f + 1) ~= 50331 us  = 50 ms
 */

#define WDT_VALUE   0x3f

#ifdef USE_WATCHDOG

#define WDT_SETUP() {                                   \
        RCC->APB1ENR |= RCC_APB1ENR_WWDGEN;             \
        WWDG->CFR = 0x0180 | 0x7f;                        \
        WWDG->CR = WDT_VALUE + 0x40;                    \
    }

#define ClrWdt() {                                                      \
        WWDG->CR = WDT_VALUE + 0xc0;                                    \
    }

#define WDT_ON() {                            \
        ClrWdt();                             \
        RCC->APB1ENR |= RCC_APB1ENR_WWDGEN;             \
    }

#define WDT_OFF() {                             \
        RCC->APB1ENR &= ~((uint32_t)RCC_APB1ENR_WWDGEN);        \
    }


#else

#define ClrWdt() {}
#define WDT_SETUP() {}
#define WDT_ON() {}
#define WDT_OFF() {}

#endif

#endif

