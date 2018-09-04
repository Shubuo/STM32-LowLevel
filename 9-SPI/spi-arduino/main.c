#include "stm32f10x.h"                  // Device header


#define SPI_GPIO_RCC  RCC_APB2ENR_IOPAEN
#define SPIx_RCC      RCC_APB2ENR_SPI1EN
#define SPI_PIN_SS    ((uint16_t)0x0010)  /*!< Pin 4 selected */
#define SPI_PIN_SCK   ((uint16_t)0x0020)  /*!< Pin 5 selected */
#define SPI_PIN_MISO  ((uint16_t)0x0040)  /*!< Pin 6 selected */
#define SPI_PIN_MOSI  ((uint16_t)0x0080)  /*!< Pin 7 selected */




void SPIx_Init(void);
uint8_t SPIx_Transfer(uint8_t data);

uint8_t receivedByte;


int main()
{
	//LCD Conf
	
	int i = 500000;
	
	SPIx_Init();
	
	while(1){

		GPIOA->BRR = SPI_PIN_SS;	//ENABLE Slave
			// Write command to slave to turn on LED blinking
			SPIx_Transfer((uint8_t) '1');
			while(i>1){i--;}
			i= 500000;
        // Write command to slave for asking LED blinking status
        SPIx_Transfer((uint8_t) '?');	
			while(i>1){i--;}
			i= 500000;
        // Read LED blinking status (off/on) from slave by transmitting dummy byte
        receivedByte = SPIx_Transfer(0);
        // Disable slave
			GPIOA->BSRR = SPI_PIN_SS;		//DISABLE Slave

			while(i>1){i--;}
			i= 500000;

			GPIOA->BRR = SPI_PIN_SS;	//ENABLE Slave
			SPIx_Transfer((uint8_t) '0');
			while(i>1){i--;}
			i= 500000;
        // Write command to slave for asking LED blinking status
        SPIx_Transfer((uint8_t) '?');	
			while(i>1){i--;}
			i= 500000;
			
        // Read LED blinking status (off/on) from slave by transmitting dummy byte
        receivedByte = SPIx_Transfer(0);
        // Disable slave
			GPIOA->BSRR = SPI_PIN_SS;		//DISABLE Slave
	}
	
}

void SPIx_Init(void)
{
	//RCC Configuration
	/* PCLK2 = HCLK/2 */
//	RCC->CFGR |= 0x00000400 << 3;

  /* Enable SPI1/2 clock and GPIO A/B */
	RCC-> APB2ENR 	|=  SPIx_RCC | SPI_GPIO_RCC| RCC_APB2ENR_IOPBEN  ;
	RCC-> APB1ENR	|= RCC_APB1ENR_SPI2EN;

	 //SPI1 Config
	SPI1->CR1 |= SPI_CR1_BR_1 | SPI_CR1_BR_2 | SPI_CR1_LSBFIRST ;	//SPI1->BaudRate 110 (128)
	SPI1->CR1 |= (SPI_CR1_CPOL&0x0) | (SPI_CR1_CPHA&0x0);				//CPOL_low | CPHA_1Edge
	SPI1->CR1 |= (SPI_CR1_DFF&0x0)  ;										// DataFrameFormat 8-bit
	SPI1->CR1 |= (SPI_CR1_BIDIMODE&0x0) | (SPI_CR1_RXONLY&0x0); 	//2Lines_FullDuplex
	SPI1->CR1 |= (SPI_CR1_LSBFIRST&0x0) |SPI_CR1_MSTR;					//MSB First	|MasterMode
	SPI1->CR1 |= SPI_CR1_SSM | SPI_CR1_SSI ;
		
//	SPI1->CRCPR	|= SPI_CRCPR_CRCPOLY & 0x111 ; //RESET Value is actually 7 also (same)
	
	SPI1->CR1 |= SPI_CR1_SPE;		//ENABLE

	 //  Initialize GPIO
	GPIOA->CRL = 0xBBB30000; //	pin5-6-7 AF_PP	| pin4 Out_PP
	
	GPIOA->BSRR = SPI_PIN_SS;		//DISABLE Slave
	
	
}

uint8_t SPIx_Transfer(uint8_t data){
	// Write data to be transmitted to the SPI data register
    SPI1->DR = data;
    // Wait until transmit complete
    while (!(SPI1->SR & (SPI_SR_TXE)));
    // Wait until receive complete
    while (!(SPI1->SR & (SPI_SR_RXNE)));
    // Wait until SPI is not busy anymore
    while (SPI1->SR & (SPI_SR_BSY));
    // Return received data from SPI data register
    return SPI1->DR;	

}



/*
#include "stm32f10x.h"
#include "stm32f10x_rcc.h"
#include "stm32f10x_gpio.h"
#include "stm32f10x_spi.h"
#include "delay.h"
#include "lcd16x2.h"

#define SPIx_RCC      RCC_APB2Periph_SPI1
#define SPIx          SPI1
#define SPI_GPIO_RCC  RCC_APB2Periph_GPIOA
#define SPI_GPIO      GPIOA


void SPIx_Init(void);
uint8_t SPIx_Transfer(uint8_t data);
void SPIx_EnableSlave(void);
void SPIx_DisableSlave(void);
 
uint8_t receivedByte;

int main(void)
{
    DelayInit();
    lcd16x2_init(LCD16X2_DISPLAY_ON_CURSOR_OFF_BLINK_OFF);

    SPIx_Init();

    while (1)
    {
        // Enable slave
        SPIx_EnableSlave();
        // Write command to slave to turn on LED blinking
        SPIx_Transfer((uint8_t) '1');
        DelayUs(10);
        // Write command to slave for asking LED blinking status
        SPIx_Transfer((uint8_t) '?');
        DelayUs(10);
        // Read LED blinking status (off/on) from slave by transmitting 
        // dummy byte
        receivedByte = SPIx_Transfer(0);
        // Disable slave
        SPIx_DisableSlave();
        // Display LED blinking status
        lcd16x2_clrscr();
        if (receivedByte == 0)
        {
            lcd16x2_puts("LED Blinking Off");
        }
        else if (receivedByte == 1)
        {
            lcd16x2_puts("LED Blinking On");
        }
        DelayMs(2500);

        // Enable slave
        SPIx_EnableSlave();
        // Write command to slave to turn off LED blinking
        SPIx_Transfer((uint8_t) '0');
        DelayUs(10);
        // Write command to slave for asking LED blinking status
        SPIx_Transfer((uint8_t) '?');
        DelayUs(10);
        // Read LED blinking status (off/on) from slave by transmitting 
        // dummy byte
        receivedByte = SPIx_Transfer(0);
        // Disable slave
        SPIx_DisableSlave();
        // Display LED blinking status
        lcd16x2_clrscr();
        if (receivedByte == 0)
        {
            lcd16x2_puts("LED Blinking Off");
        }
        else if (receivedByte == 1)
        {
            lcd16x2_puts("LED Blinking On");
        }
        DelayMs(2500);
    }
}

void SPIx_Init()
{
    // Initialization struct
    SPI_InitTypeDef SPI_InitStruct;
    GPIO_InitTypeDef GPIO_InitStruct;

    // Step 1: Initialize SPI
    RCC_APB2PeriphClockCmd(SPIx_RCC, ENABLE);
    SPI_InitStruct.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_128;
    SPI_InitStruct.SPI_CPHA = SPI_CPHA_1Edge;
    SPI_InitStruct.SPI_CPOL = SPI_CPOL_Low;
    SPI_InitStruct.SPI_DataSize = SPI_DataSize_8b;
    SPI_InitStruct.SPI_Direction = SPI_Direction_2Lines_FullDuplex;
    SPI_InitStruct.SPI_FirstBit = SPI_FirstBit_MSB;
    SPI_InitStruct.SPI_Mode = SPI_Mode_Master;
    SPI_InitStruct.SPI_NSS = SPI_NSS_Soft | SPI_NSSInternalSoft_Set;
    SPI_Init(SPIx, &SPI_InitStruct); 
    SPI_Cmd(SPIx, ENABLE);

    // Step 2: Initialize GPIO
    RCC_APB2PeriphClockCmd(SPI_GPIO_RCC, ENABLE);
    // GPIO pins for MOSI, MISO, and SCK
    GPIO_InitStruct.GPIO_Pin = SPI_PIN_MOSI | SPI_PIN_MISO | SPI_PIN_SCK;
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(SPI_GPIO, &GPIO_InitStruct);
    // GPIO pin for SS
    GPIO_InitStruct.GPIO_Pin = SPI_PIN_SS;
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(SPI_GPIO, &GPIO_InitStruct);

    // Disable SPI slave device
    SPIx_DisableSlave();
}

uint8_t SPIx_Transfer(uint8_t data)
{
    // Write data to be transmitted to the SPI data register
    SPIx->DR = data;
    // Wait until transmit complete
    while (!(SPIx->SR & (SPI_I2S_FLAG_TXE)));
    // Wait until receive complete
    while (!(SPIx->SR & (SPI_I2S_FLAG_RXNE)));
    // Wait until SPI is not busy anymore
    while (SPIx->SR & (SPI_I2S_FLAG_BSY));
    // Return received data from SPI data register
    return SPIx->DR;
}

void SPIx_EnableSlave()
{
    // Set slave SS pin low
    SPI_GPIO->BRR = SPI_PIN_SS;
}

void SPIx_DisableSlave()
{
    // Set slave SS pin high
    SPI_GPIO->BSRR = SPI_PIN_SS;
}*/