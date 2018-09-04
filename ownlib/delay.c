#include "delay.h"

static __IO uint32_t sysTickCounter;

void SysTick_Init(void) {
	/****************************************
	 *SystemFrequency/1000      1ms         *
	 *SystemFrequency/100000    10us        *
	 *SystemFrequency/1000000   1us         *
	 *****************************************/
	while (SysTick_Config(SystemCoreClock / 1000000) != 0) {
	} // One SysTick interrupt now equals 1us

}

/**
 * This method needs to be called in the SysTick_Handler
 */
void TimeTick_Decrement(void) {
	if (sysTickCounter != 0x00) {
		sysTickCounter--;
	}
}

void delay_nus(u32 n) {
	sysTickCounter = n;
	while (sysTickCounter != 0) {
	}
}

void delay_1ms(void) {
	sysTickCounter = 1000;
	while (sysTickCounter != 0) {
	}
}

void delay_nms(u32 n) {
	while (n--) {
		delay_1ms();
	}
}

/*
 *Now we have all files we need but the timer won’t work as we’re missing one part: the interrupt handler.
  Notice the static __IO uint32 sysTickCounter in delay.c, 
  this is the counter that steers the delay functions and needs to be decremented in the interrupt routine.
  The interrupt can be located anywhere in your code, just find a convenient location and add:

	Delay kullanilmasi istendigi zaman asagidaki fonksiyonun da main.c dosyasi içinde bulunmasi gereklidir
	Yoksa static tanimlandigi için degisim uygulanmaz

void SysTick_Handler(void) {
	TimeTick_Decrement();
}

	#include "misc.h"
	#include "delay.h"
*/
