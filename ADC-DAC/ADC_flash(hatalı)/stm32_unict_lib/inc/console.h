/*
 * console.h
 */

#ifndef __CONSOLE_H
#define __CONSOLE_H

#include <stm32f10x.h>

void CONSOLE_init(void);
int kbhit(void);
char readchar(void);


#endif
