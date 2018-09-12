/*
*		I2C STM32 Master - Arduino Slave Example
*		by Burak Yoruk
*
*/

#include "stm32f10x.h"
#include "delay.h"
#include "misc.h"                       // Keil::Device:StdPeriph Drivers:Framework
#include "i2c_lib.h"

void SysTick_Handler(void) ;
//I2C1 pins PB8-SCL PB-9-SDA | PB6-SCL PB7-SDA

#define SLAVE_ADDRESS    0x08

uint8_t receivedByte;


int main(void)
{ 
	 SysTick_Init() ;
    // Initialize I2C
    i2c_init();
	
    while (1)
    {
        // Write 0x01 to slave (turn on LED blinking)
        i2c_write(SLAVE_ADDRESS, 0x01);
        delay_nms(5);
        // Read LED blinking status (off/on)
        i2c_read(SLAVE_ADDRESS, &receivedByte);
        // Display LED blinking status
     
        delay_nms(2500);
  
        // Write 0x00 to slave (turn off LED blinking)
        i2c_write(SLAVE_ADDRESS, 0x00);
        delay_nms(5);
        // Read LED blinking status (off/on)
        i2c_read(SLAVE_ADDRESS, &receivedByte);
        // Display LED blinking status

        delay_nms(2500);
    }
}

void SysTick_Handler(void) {
	TimeTick_Decrement();
}
