/******************************************************************************
 * Project        : HAN ESE PRJ2, PRJ1V & PRJ1D
 * File           : USART driver header file
 * Copyright      : 2013 HAN Embedded Systems Engineering
 ******************************************************************************
  Change History:

    Version 1.0 - April 2013
    > Initial revision

******************************************************************************/
#ifndef _USART_H_
#define _USART_H_

#include "stdint.h"

/******************************************************************************
  Defines
******************************************************************************/


/******************************************************************************
  Function prototypes
******************************************************************************/
void USART_init(void);
void USART_putc(char c);
char USART_getc(void);
void USART_putstr(char *str);
void USART_getstr(char *str);
void USART_clearscreen(void);

#endif // _USART_H_
