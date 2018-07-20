/*
 * usart.c
 *
 *  Created on: Sep 30, 2017
 *      Author: corrado
 */

#include "usart.h"

int baud_rates[3][2] = { {B19200_MANTISSA, B19200_FRACTIONAL},
                         {B57600_MANTISSA, B57600_FRACTIONAL},
                         {B115200_MANTISSA, B115200_FRACTIONAL} };


void USART_init(USART_TypeDef * usart, int baud_rate)
{
	if (usart == USART2) {
		/* Enable USART2 clock */
		RCC->APB1ENR |= RCC_APB1ENR_USART2EN;
	}

    usart->CR1 = 0x0; // first disable all

    // baud rate
    usart->BRR = (baud_rates[baud_rate][0] << 4) | baud_rates[baud_rate][1];

    // CR1
    // -----------------------------------------
    //  OVER8 = 0 - oversampling by 16
    // Unused = 0
    //     UE = 0 - Not enabled
    //      M = 0 - 8 bits

    //   WAKE = 0
    //    PCE = 0 - no parity
    //     PS = 0 - parity even, but unused
    //   PEIE = 0

    //  TXEIE = 0 - no TX interrupt
    //   TCIE = 0 - no Tx Complete interrupt
    //  RXEIE = 0 - no RX interrupt
    // IDLEIE = 0 - no IDLE interrupt

    //     TE = 1 - Transmitter Enabled
    //     RE = 1 - Receiver Enabled
    //    RWU = 0 - Receiver in active mode
    //    BRK = 0 - No break
    // -----------------------------------------
    // CR1 = 0000 0000 0000 1100
    usart->CR1 = 0x000c;

    // CR2
    // -----------------------------------------
    // Unused = 0
    //  LINEN = 0 - No LIN mode
    //   STOP = 0
    //        = 0 - 1 stop bits

    //  CLKEN = 0 - no CK pin
    //   CPOL = 0 - clock polarity
    //   CPHA = 0 - clock phase
    //   LBCL = 0 - last clock pulse

    //    Res = 0 -
    //  LBDIE = 0 - no LIN break interrupt
    //   LBDL = 0 - LIN break lenght
    //   Res  = 0 -

    //   ADDR = 0000 - USART address
    // -----------------------------------------
    usart->CR2 = 0x0;


    // CR3
    // -----------------------------------------
    // Unused = 00000
    //  CTSIE = 0 - No CTS interrupt
    //   CTSE = 0 - No CTS flow control
    //   RTSE = 0 - No RTS flow control
    //   DMAT = 0 - No TX DMA
    //   DMAR = 0 - No RX DMA
    //   SCEN = 0 - No Smart cart
    //   NACK = 0 - (for smart cart)
    //  HDSEL = 0 - Half duplex not selected
    //   IRLP = 0 -
    //   IREN = 0 - No IrDA
    //    EIE = 0 - No Error interrupt
    // -----------------------------------------
    usart->CR3 = 0x0;

    usart->CR1 |= 0x2000; // enable USART
}

void USART_putc(USART_TypeDef * usart, char c)
{
    // TXE is bit 7, if it is 0 then the Data register is full
    while ((usart->SR & 0x80) == 0) {}
    usart->DR = c;
}

char USART_getc(USART_TypeDef * usart)
{
    while ((usart->SR & 0x20) == 0) {}
    return usart->DR;
}

int USART_ready(USART_TypeDef * usart)
{
	return ((usart->SR & 0x20) != 0);
}
