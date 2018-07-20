/*
 * display.h
 */


#ifndef __DISPLAY_H

#define __DISPLAY_H


#include "gpio.h"

void DISPLAY_init(void);

void DISPLAY_putc(int digit, char c);

void DISPLAY_puts(int digit, char * s);

void DISPLAY_dp(int digit, char dp);




#endif
