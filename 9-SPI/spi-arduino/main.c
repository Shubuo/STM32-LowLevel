
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
	
	
	SPIx_Init();
	
	while(1){

			// Enable Slave - Set slave SS pin low
			GPIOA->BRR = SPI_PIN_SS;
		
			// Write command to slave to turn on LED blinking
			SPIx_Transfer((uint8_t) '1');

        // Write command to slave for asking LED blinking status
        SPIx_Transfer((uint8_t) '?');	

        // Read LED blinking status (off/on) from slave by transmitting dummy byte
        receivedByte = SPIx_Transfer(0);
        
			//  Disable slave - Set slave SS pin high
			GPIOA->BSRR = SPI_PIN_SS;

			// Set slave SS pin low
			GPIOA->BRR = SPI_PIN_SS;
			
			SPIx_Transfer((uint8_t) '0');

        // Write command to slave for asking LED blinking status
        SPIx_Transfer((uint8_t) '?');	
			
        // Read LED blinking status (off/on) from slave by transmitting dummy byte
        receivedByte = SPIx_Transfer(0);
			
			//  Disable slave - Set slave SS pin high
			GPIOA->BSRR = SPI_PIN_SS;
			
//			while(i>1){i--;}
//			i= 1000000;
	}
	
}

void SPIx_Init(void)
{
	//RCC Configuration , PCLK2 = HCLK/2 
		RCC->CFGR |= 0x00000400 << 3;

  /* Enable SPI1 clock and GPIO A */
		RCC-> APB2ENR 	|=  SPIx_RCC | SPI_GPIO_RCC  ;

	 //SPI1 Config
	SPI1->CR1 |= SPI_CR1_BR_1 | SPI_CR1_BR_2 ;							//SPI1->BaudRate 110 (128)
	SPI1->CR1 |= (SPI_CR1_CPOL&0x0) | (SPI_CR1_CPHA&0x0);				//CPOL_low | CPHA_1Edge
	SPI1->CR1 |= (SPI_CR1_DFF&0x0)  ;										//DataFrameFormat 8-bit
	SPI1->CR1 |= (SPI_CR1_BIDIMODE&0x0) | (SPI_CR1_RXONLY&0x0); 	//2Lines_FullDuplex
	SPI1->CR1 |= (SPI_CR1_LSBFIRST&0x0) |SPI_CR1_MSTR;					//MSB First	|MasterMode
	SPI1->CR1 |= SPI_CR1_SSM | SPI_CR1_SSI ;
		
	SPI1->CRCPR	|= SPI_CRCPR_CRCPOLY & 0x111 ; //RESET Value is actually 7 also (same)
	
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
