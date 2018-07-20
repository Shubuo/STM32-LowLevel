/*
 * usart.h
 */

#ifndef __USART_H
#define __USART_H

#include <stm32f10x.h>

// defines with 42MHz clock of peripheral clock
#define B19200                  0
#define B19200_MANTISSA         136
#define B19200_FRACTIONAL       12

// B57600 = 42000000.0/(16 * (45 + 9.0/16))
#define B57600                  1
#define B57600_MANTISSA         45
#define B57600_FRACTIONAL       9

// B115200 = 42000000.0/(16 * (22 + 13.0/16))
#define B115200                 2
#define B115200_MANTISSA        22
#define B115200_FRACTIONAL      13

void USART_init(USART_TypeDef * usart, int baud_rate);
void USART_putc(USART_TypeDef * usart, char c);
char USART_getc(USART_TypeDef * usart);
int USART_ready(USART_TypeDef * usart);


#endif
