	/******************** 2018 STM32 MCU   **************************
*
* Dosya Adi          : usart.h Header
* Yazar							 : Burak YÖRÜK
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

#define USART2_BAUD_RATE				9600 	//USART2->BRR |= 0xEA6 			//12bit Mantissa + 4bit Fraction
#define USART2_PCLK1						36000000
#define USART2_M								0			//Wordlength 0:1Start,8Data,nStop		1:1start,9Data,nStopBits	 
#define USART2_PCE							0						   
#define USART2_PS								0						
#define USART2_STOP							0			//00:1 Stop, 01:0.5 Stop, 10:2 Stop, 11: 1.5 Stop



void usart_init(void);
void putch(unsigned char d);
char getch(void);
void usart2_string(const char *st);
