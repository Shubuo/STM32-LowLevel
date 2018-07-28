#include "DAC.h"
#include "GPIO.h"
#include "stm32f10x.h"                  // Device header
#include <math.h>
#include "stm32f10x_dac.h"              // Keil::Device:StdPeriph Drivers:DAC
#include "delay.h"

#define radian_per_degree              0.0174532925


void setup(void);
void GPIO_setup(void);
void DAC_setup(void);

		 
		 
void main() 
{
     signed int temp1 = 0;
     signed int temp2 = 0;
     unsigned int degree = 0;
		unsigned int delay  = 50000;
     setup();
     
     while(1)
     {	   // RCC->APB1ENR = 0x20000000;//(1UL << 29);
						//delay_us(50);
					
         for(degree = 0; degree < 360; degree++)
         {
             temp1 = (2047 * cos(radian_per_degree * degree));
             temp1 = (2048 - temp1);
             DAC->DHR12R1 = ((unsigned int)temp1);
             temp2 = (2047 * sin(radian_per_degree * degree));
             temp2 = (2048 - temp2);
             DAC->DHR12R2 = ((unsigned int)temp2);
							 RCC->APB1ENR = 0x20000000;//(1UL << 29);
         }
     }
}


void setup()
{
    GPIO_setup();
    DAC_setup();
}


void GPIO_setup()
{
    RCC->APB2ENR |= (1UL << 2);  //gpioa en
     pin_configure_low(GPIOA->CRL, 4, analog_input);
     pin_configure_low(GPIOA->CRL, 5, analog_input);
}


void DAC_setup()
{
		 unsigned int delay  = 50000;
     RCC->APB1ENR = 0x20000000;//(1UL << 29);
			while(delay--){};
			delay = 50000;
     DAC->CR = 0x00000000;
     DAC->CR = 0x03;  		//DAC boff 1, ch1
     DAC->CR = 0x30000;  	//dac boff 2, ch2
}
