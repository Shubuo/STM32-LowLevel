#include "stm32f10x.h"
#include "gpio_by.h" /* Expkits EXS-03 Development Kit Header File */
#include "delay.h"

/*************************Ana Program*************************/
int main(void)
{
    BY_initGPIO();//Giris Cikilari Ayarla
    delay_init();  //Delay icin konfigurasyonlari yap
    GPIO_ResetBits(GPIOC , GPIO_Pin_8);	 // Buzzer Sussun