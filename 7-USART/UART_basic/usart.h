	/******************** 2018 STM32 MCU   **************************
*
* Dosya Adi          : usart.h Header
* Yazar							 : Burak Y�R�K
* Version            : V1
* Tarih              : 02/Agustos/2018
*
****************************************************************************/
///////				INIT	USART		////////
// Baud rate formula:
// - IntegerDivider = ((PCLKx) / (16 * (USART_InitStruct->USART_BaudRate)))
// - FractionalDivider = ((IntegerDivider - ((u32) IntegerDivider)) * 16) + 0.5 */


//DIV_Mantissa = 234, DIV_Fraction = 16*0,38 = 6 (nearest real number)
//USART2->BRR |= 0xEA << 4 | USART2->BRR |= 0x6 

#define USART1_BAUD_RATE				9600 	//USART1->BRR |= 0xEA6 			//12bit Mantissa + 4bit Fraction
#define USART1_PCLK2						72000000
#define USART1_M								0			//Wordlength 0:1Start,8Data,nStop		1:1start,9Data,nStopBits	 
#define USART1_PCE							0						   
#define USART1_PS								0						
#define USART1_STOP							0			//00:1 Stop, 01:0.5 Stop, 10:2 Stop, 11: 1.5 Stop



void usart_init(void);
char getch(unsigned char value);										//USART_Rx
char getch_string(volatile char *st);	//USART_Tx
void putch(unsigned char);					//USART_TxCh
void usart_string(const char *st);	//USART_Tx
void Usart_IRQHandler(void) __irq;
void Usart_NVIC(void);

void gpio_init(void);
