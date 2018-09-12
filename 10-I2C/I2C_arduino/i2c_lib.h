#ifndef __I2CLIB_H
#define __I2CLIB_H

#include "stm32f10x.h"


#define I2Cx_RCC        RCC_APB1ENR_I2C1EN
#define I2Cx            I2C1
#define I2C_GPIO_RCC    RCC_APB2ENR_IOPBEN
#define I2C_GPIO        GPIOB
#define FLAG_Mask			((uint32_t) 0x00FFFFFF)
#define I2C_FLAG_BUSY                   ((uint32_t)0x00020000)
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
ErrorStatus I2C_CheckEvent(I2C_TypeDef*, uint32_t);
void I2C_GenerateSTART(I2C_TypeDef*, FunctionalState);
FlagStatus I2C_GetFlagStatus(I2C_TypeDef*, uint32_t );


#endif
