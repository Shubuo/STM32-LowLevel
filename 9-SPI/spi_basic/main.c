/*
		AYni islemcide olmadi kardas WAAAy ?
*/
#include "stm32f10x.h"                  // Device header

typedef enum {FAILED = 0, PASSED = !FAILED} TestStatus;

/* Private define ------------------------------------------------------------*/
#define BufferSize 32

uint8_t SPI1_Buffer_Tx[BufferSize] = {0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07,
                                      0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E,
                                      0x0F, 0x10, 0x11, 0x12, 0x13, 0x14, 0x15,
                                      0x16, 0x17, 0x18, 0x19, 0x1A, 0x1B, 0x1C,
                                      0x1D, 0x1E, 0x1F, 0x20};
uint8_t SPI2_Buffer_Tx[BufferSize] = {0x51, 0x52, 0x53, 0x54, 0x55, 0x56, 0x57,
                                      0x58, 0x59, 0x5A, 0x5B, 0x5C, 0x5D, 0x5E,
                                      0x5F, 0x60, 0x61, 0x62, 0x63, 0x64, 0x65, 
                                      0x66, 0x67, 0x68, 0x69, 0x6A, 0x6B, 0x6C,
                                      0x6D, 0x6E, 0x6F, 0x70};

uint8_t SPI1_Buffer_Rx[BufferSize], SPI2_Buffer_Rx[BufferSize];
__IO uint8_t TxIdx = 0, RxIdx = 0, k = 0;
volatile TestStatus TransferStatus1 = FAILED, TransferStatus2 = FAILED;

TestStatus Buffercmp(uint8_t* pBuffer1, uint8_t* pBuffer2, uint16_t BufferLength);

int main ()
	{
		//RCC Configuration
	/* PCLK2 = HCLK/2 */
	RCC->CFGR |= 0x00000400 << 3;

  /* Enable SPI1/2 clock and GPIO A/B/C */
	RCC-> APB2ENR 	|= RCC_APB2ENR_IOPAEN | RCC_APB2ENR_IOPBEN | RCC_APB2ENR_SPI1EN | RCC_APB2ENR_IOPCEN ;
	RCC-> APB1ENR	|= RCC_APB1ENR_SPI2EN;
		
		//GPIO COnfiguration
	/* Configure SPI_MASTER pins: SCK and MOSI, MISO in pin6 */
	  GPIOA->CRL = 0xB4B00000 ;	//pin 5-7 AF_PP, pin 6 IN_Floating // else 0x4B400000 pin5-7 input floating, pin6 Master

	/* Configure SPI_SLAVE pins: SCK and MOSI, MISO in pin14 */
	  GPIOB->CRH = 0x4B400000 ;	//	else 0xB4B00000 pin 13-15 AF PP, pin 14 Floating In
	  
		GPIOC->CRL = 0x3300;	// Pin 2-3 OUT_PP
		
	 //SPI1 Config
		SPI1->CR1 |= (SPI_CR1_BIDIMODE&0x0) | SPI_CR1_MSTR | (SPI_CR1_DFF&0x0) | (SPI_CR1_CPOL&0x0) | SPI_CR1_CPHA ;
		SPI1->CR1 |= SPI_CR1_BR_0 | SPI_CR1_LSBFIRST ;
		SPI1->CRCPR	|= SPI_CRCPR_CRCPOLY & 0x7 ; //RESET Value is actually 7 also (same)
		
		SPI1->CR1 |= SPI_CR1_SPE;		//ENABLE
	 
	 //SPI2 Config
		SPI2->CR1 |= (SPI_CR1_MSTR & 0x0);
		SPI2->CR1 |=	SPI_CR1_CPHA | SPI_CR1_LSBFIRST |SPI_CR1_BR_0 ;
		

		SPI2->CR1 |= SPI_CR1_SPE;

 /* Transfer procedure */
  while (TxIdx < BufferSize)
  {
    /* Wait for SPI1 Tx buffer empty */
    while ((SPI1->SR & SPI_SR_TXE) == RESET);
    /* Send SPI2 data */
    SPI2 -> DR = SPI2_Buffer_Tx[TxIdx];
    /* Send SPI1 data */
    SPI1 -> DR =  SPI1_Buffer_Tx[TxIdx++];
    /* Wait for SPI2 data reception */
    while ((SPI2->SR & SPI_SR_RXNE) == RESET);
    /* Read SPI2 received data */
    SPI2_Buffer_Rx[RxIdx] = SPI2 -> DR ;
    /* Wait for SPI1 data reception */
    while ((SPI1->SR & SPI_SR_RXNE) == RESET);
    /* Read SPI1 received data */
    SPI1_Buffer_Rx[RxIdx++] = SPI1->DR;
  }
  /* Check the correctness of written dada */
  TransferStatus1 = Buffercmp(SPI2_Buffer_Rx, SPI1_Buffer_Tx, BufferSize);
  TransferStatus2 = Buffercmp(SPI1_Buffer_Rx, SPI2_Buffer_Tx, BufferSize);
  /* TransferStatus1, TransferStatus2 = PASSED, if the transmitted and received data are equal */
  /* TransferStatus1, TransferStatus2 = FAILED, if the transmitted and received data are different */
  
  while (1){}
	}
	
	
TestStatus Buffercmp(uint8_t* pBuffer1, uint8_t* pBuffer2, uint16_t BufferLength)
{
  while (BufferLength--)
  {
    if (*pBuffer1 != *pBuffer2)
    {
		GPIOC->BRR = 0xC;
//	 GPIOC->BSRR = 0xC << 16;

      return FAILED;
    }

    pBuffer1++;
    pBuffer2++;
  }
	
	GPIOC->BSRR = 0xC;
  
  return PASSED;
}

