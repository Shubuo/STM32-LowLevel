/*
*		I2C STM32 Master - Arduino Slave Example
*		by Burak Yoruk
*
*/

#include "stm32f10x.h"
#include "delay.h"
//#include "i2c_lib.h"
#include "GPIO_STM32F10X.h"

#define I2Cx_RCC        RCC_APB1ENR_I2C1EN
#define I2Cx            I2C1
#define I2C_GPIO_RCC    RCC_APB2ENR_IOPBEN
#define I2C_GPIO        GPIOB
#define FLAG_Mask			((uint32_t) 0x10000000)
#define I2C_EVENT_MASTER_MODE_SELECT           			 ((uint32_t)0x00030001)  /* BUSY, MSL and SB flag */
#define I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED     ((uint32_t)0x00070082)  /* BUSY, MSL, ADDR, TXE and TRA flags */
#define I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED        ((uint32_t)0x00030002)  /* BUSY, MSL and ADDR flags */
/* Master RECEIVER mode -----------------------------*/ 
#define  I2C_EVENT_MASTER_BYTE_RECEIVED                    ((uint32_t)0x00030040)  /* BUSY, MSL and RXNE flags */
/* Master TRANSMITTER mode --------------------------*/
//#define I2C_EVENT_MASTER_BYTE_TRANSMITTING                 ((uint32_t)0x00070080) /* TRA, BUSY, MSL, TXE flags */
#define  I2C_EVENT_MASTER_BYTE_TRANSMITTED                 ((uint32_t)0x00070084)  /* TRA, BUSY, MSL, TXE and BTF flags */


void i2c_init(void);
void i2c_start(void);
void i2c_stop(void);
void i2c_address_direction(uint8_t address, uint8_t direction);
void i2c_transmit(uint8_t byte);
uint8_t i2c_receive_ack(void);
uint8_t i2c_receive_nack(void);
void i2c_write(uint8_t address, uint8_t data);
void i2c_read(uint8_t address, uint8_t* data);

//I2C1 pins PB8-SCL PB-9-SDA | PB6-SCL PB7-SDA

#define SLAVE_ADDRESS    0x08

uint8_t receivedByte;

uint32_t lastevent = 0 ;
uint32_t flag1 = 0, flag2 = 0;
ErrorStatus status = ERROR ; 


int main(void)
{ 
    // Initialize I2C
    i2c_init();
	
	flag1 = I2C1->SR1;	
	flag2 = I2C1->SR2;
	flag2 = flag2 << 16 ;
	lastevent = (flag1 | flag2) & FLAG_Mask;
 
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

void i2c_init()
{

    // Step 1: Initialize I2C
	 RCC->APB1ENR |= RCC_APB1ENR_I2C1EN;

	 I2C1->CR2 		|= 36;								//0x101010	36 MHz APB1 Hattiniz frekansi 
	 I2C1->CCR 		|= 32948;							//	Standard Mode + Clock Control=180 (Thigh =Tpclk1(27,7ns)*180 = 5us)
	 I2C1->TRISE	|= 37;								// (1000ns/Tpclk) + 1 
	 I2C1->OAR1 	|= (I2C_OAR1_ADDMODE&0x0) ;	//7-bit adresleme
	 I2C1->CR1		|= I2C_CR1_PE;						// PE = 1
	
 
    // Step 2: Initialize GPIO as open drain alternate function
	 GPIO_PortClock(I2C_GPIO, ENABLE);

	 GPIO_PinConfigure(I2C_GPIO,8,GPIO_AF_OPENDRAIN,GPIO_MODE_OUT2MHZ);	// I2C SCL
	 GPIO_PinConfigure(I2C_GPIO,9,GPIO_AF_OPENDRAIN,GPIO_MODE_OUT2MHZ);	//I2C1 SDA
    GPIO_AFConfigure(AFIO_I2C1_NO_REMAP);
	 
}

void i2c_write(uint8_t address, uint8_t data)
{
    i2c_start();
    i2c_address_direction(address << 1, 0x0);	//transmit addres
    i2c_transmit(data);
    i2c_stop();
}

void i2c_read(uint8_t address, uint8_t* data)
{
    i2c_start();
    i2c_address_direction(address << 1, 0x1);	//receive adres
    *data = i2c_receive_nack();
    i2c_stop();
}

void i2c_start()
{
	 // Wait until I2Cx is not busy anymore
     while ((I2C1->SR2 & I2C_SR2_BUSY) != RESET);	

    // Generate start condition
    I2C1->CR1 |= I2C_CR1_START;

 	//wait BUSY, MSL and SB flag
// It means that the start condition has been correctly released on the I2C bus (the bus is free, no other devices is communicating))
	while ((lastevent & I2C_EVENT_MASTER_MODE_SELECT) != I2C_EVENT_MASTER_MODE_SELECT);	
	
}

void i2c_stop()
{
    // Generate I2C stop condition
    I2C1->CR1 |= I2C_CR1_STOP;
    // Wait until I2C stop condition is finished
    while ((I2C1->SR2 & I2C_SR1_STOPF) != RESET);	
}

void i2c_address_direction(uint8_t address, uint8_t direction)
{
	
    // Send slave address
	/* Test on the direction to set/reset the read/write bit */
  if (direction != 0x0)
  {
    /* Set the address bit0 for read */
    address |= I2C_OAR1_ADD0;				//I2C_OAR1_ADD0_Set
  }
  else
  {
    /* Reset the address bit0 for write */
    address &= (0xFFFF-I2C_OAR1_ADD0);	//I2C_OAR1_ADD0_Reset
  }
  /* Send the address */
  I2C1->DR = address;
	

    // Wait for I2C EV6
    // It means that a slave acknowledges his address
    if (direction == 0x0)
    {
		  while ((lastevent & I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED) != I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED);	

    }
    else if (direction == 0x1)
    { 
 		  while ((lastevent & I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED) != I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED);			 
    }
}

void i2c_transmit(uint8_t byte)
{
    // Send data byte
    I2C1->DR = byte ;
	
    // Wait for I2C EV8_2.
    // It means that the data has been physically shifted out and 
    // output on the bus)
    while ((lastevent & I2C_EVENT_MASTER_BYTE_TRANSMITTED) != I2C_EVENT_MASTER_BYTE_TRANSMITTED);			 

}

uint8_t i2c_receive_ack()
{
    // Enable ACK of received data
	 I2C1->CR1 |= (I2C_CR1_ACK);
    // Wait for I2C EV7
    // It means that the data has been received in I2C data register
	  while((lastevent & I2C_EVENT_MASTER_BYTE_RECEIVED) != I2C_EVENT_MASTER_BYTE_RECEIVED){};			 

    // Read and return data byte from I2C data register
    return (I2C1->DR);
}

uint8_t i2c_receive_nack()
{
    // Disable ACK of received data
    I2C1->CR1 &= (0xFFFF - I2C_CR1_ACK);
    // Wait for I2C EV7
    // It means that the data has been received in I2C data register
	 while ((lastevent & I2C_EVENT_MASTER_BYTE_RECEIVED) != I2C_EVENT_MASTER_BYTE_RECEIVED){};			 

    // Read and return data byte from I2C data register
    return (I2C1->DR);
}
