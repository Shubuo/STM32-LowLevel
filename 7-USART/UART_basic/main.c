#include "stm32f10x.h"                  // Device header
#include <stdio.h>
#include "usart.h"

int main(){	
	
 usart_init(); 
 
 usart2_string("Burak Yoruk"); 
 
 for(;;) 
 putch(getch()); 
	
}

