/*
 * Dosya    : main.c
 * Yazar    : Expkits - www.expkits.com
 * Uygulama : LED Blink Test Uygulaması
 * Sürüm    : v1.0
 * Tarih    : 05.04.2013
 * MCU      : STM32F10x
 * Kristal  : 8Mhz (MCU FOSC 72Mhz)
 * Kit      : EXS03
 * Açıklama : Kit üzerindeki LED'ler ile yürüyen ılık efekti yapar.
 * Uyarı    : Keil uVision v4.70 ile derlenmiştir.
 *						JP1 jumperının takılı olduğundan emin olun.
 */

/* Includes ------------------------------------------------------------------*/
#include "stm32f10x_it.h"
#include "exs03.h" /* Expkits EXS-03 Development Kit Header File */
#include "delay.h"


/*************************Ana Program*************************/
int main(void)
{
    int i=0;
    EXS03_initGPIO();//Giris Cikilari Ayarla
    delay_init(); 	 //Delay icin konfigurasyonlari yap
    while (1)
    {
        switch(i)
        {
        case 0:
            GPIO_SetBits(GPIOA   , GPIO_Pin_0);
            GPIO_ResetBits(GPIOA , GPIO_Pin_1);
            GPIO_ResetBits(GPIOC , GPIO_Pin_2);
            GPIO_ResetBits(GPIOC , GPIO_Pin_3);
            break;
        case 1:
            GPIO_ResetBits(GPIOA , GPIO_Pin_0);
            GPIO_SetBits(GPIOA   , GPIO_Pin_1);
            GPIO_ResetBits(GPIOC , GPIO_Pin_2);
            GPIO_ResetBits(GPIOC , GPIO_Pin_3);
            break;
        case 2:
            GPIO_ResetBits(GPIOA , GPIO_Pin_0);
            GPIO_ResetBits(GPIOA , GPIO_Pin_1);
            GPIO_SetBits(GPIOC   , GPIO_Pin_2);
            GPIO_ResetBits(GPIOC , GPIO_Pin_3);
            break;
        case 3:
            GPIO_ResetBits(GPIOA , GPIO_Pin_0);
            GPIO_ResetBits(GPIOA , GPIO_Pin_1);
            GPIO_ResetBits(GPIOC , GPIO_Pin_2);
            GPIO_SetBits(GPIOC   , GPIO_Pin_3);
            break;
        }
        delay_ms(250); // 0.25sn
        i++;
        if(i==4)
            i=0;
    }
}

#ifdef  USE_FULL_ASSERT

/**
  * @brief  Reports the name of the source file and the source line number
  *   where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
    /* User can add his own implementation to report the file name and line number,
       ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
    /* Infinite loop */
    while (1)
    {
    }
}
#endif

/*********************************************************************************************************
      END FILE
*********************************************************************************************************/




