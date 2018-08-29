/*
 * 			rtc.h
 *
 *  		Created on: 13.08.2018
 *      Author: Burak Yoruk
 */

#include "rtc.h"

#define RTC_LSB_MASK     ((uint32_t)0x0000FFFF)  /*!< RTC LSB Mask */
#define PRLH_MSB_MASK    ((uint32_t)0x000F0000)  /*!< RTC Prescaler MSB Mask */

/*
 * RTC_Config()
 * Setup RTC clock and start RTC
 */

void RTC_Clock_Config()
{
	// Enable the PWR clock
	RCC->APB1ENR |= RCC_APB1ENR_PWREN;

	// Enable access to RTC and Backup registers
	PWR->CR |= PWR_CR_DBP;

	// Resets Backup Domain Config
	RCC->BDCR |= RCC_BDCR_BDRST;
	RCC->BDCR &= ~RCC_BDCR_BDRST;  //set

	// Start LSE clock
	RCC->BDCR |= RCC_BDCR_LSEON;

	// Wait until LSE is ready
	while ( (RCC->BDCR & RCC_BDCR_LSERDY) != RCC_BDCR_LSERDY);

	// Select LSE as RTC clock source
	RCC->BDCR |= RCC_BDCR_RTCSEL_LSE;

	// Enable RTC clock
	RCC->BDCR |= RCC_BDCR_RTCEN;
	
//Poll the RSF bit in the RTC_CRL register until the RTC registers are synchronized (if a 
//50/60 Hz external clock source is used this step may take up to a minute to complete)
	while ( (RTC->CRL & RTC_CRL_RSF) != RTC_CRL_RSF);
	
//Poll the RTOFF bit in the RTC_CRL register until the last operation on the RTC registers is over
	while ( (RTC->CRL & RTC_CRL_RTOFF) != RTC_CRL_RTOFF);
	
	RTC->CRH |= RTC_CRH_SECIE;
	//  while ( (RTC->CRL & RTC_CRL_RSF) != RTC_CRL_RSF);

	

}

static void SystemClock_Config(void) 
{
	
	//--- Use PA8 as LSE output ---

	// Set MCO source as LSE
	RCC->CFGR |=  RCC_CFGR_MCO_SYSCLK;     // Change here

	
	// Enable GPIOA clock
	RCC->APB2ENR |= RCC_APB2ENR_IOPAEN;

	// Configure PA8 as Alternate function
	GPIOA->CRH |= GPIO_CRH_MODE8_0 | (GPIO_CRH_CNF8&0x0) | GPIO_CRH_CNF8_1;

//	// Set to AF0 (MCO output)
//	GPIOA->AFR[1] &= ~(0x0000000F);
//	GPIOA->AFR[1] |=  (0x00000000);

	
}

/*
 * 	 RTC_SetTime(uint32_t time)
 * - Sets RTC Prescalers
 * - Sets RTC time
 */

void RTC_SetTime(time_t *ptime)
{
	uint32_t	bcdtime;

//	bcdtime = 	( (byte2bcd(ptime->hours))   <<16U) |
//			( (byte2bcd(ptime->minutes)) <<8U)  |
//			( (byte2bcd(ptime->seconds)) <<0U);

//Poll the RSF bit in the RTC_CRL register until the RTC registers are synchronized (if a 50/60 Hz 
//external clock source is used this step may take up to a minute to complete)	
		while ( (RTC->CRL & RTC_CRL_RSF) != RTC_CRL_RSF);

//	Poll the RTOFF bit in the RTC_CRL register until the last operation on the RTC registers is over
		while ( (RTC->CRL & RTC_CRL_RTOFF) == RTC_CRL_RTOFF);	//initial 1

	//Enable the RTC second global interrupt
		RTC->CRH &= RTC_CRH_SECIE ;
	
//	
//RTC prescaler value using the following formula:
//fTRCLK = fRTCCLK/(PRL[19:0]+1), where:
//– fRTCCLK is the input clock frequency
//– fTRCLK is the time base generated from the prescaler block	
//	
	RTC->PRLL |= 0x7FFF;
//	RTC->DIVL |= 0x7FFF;
	
	//EnterConfig
	RTC->CRL |= RTC_CRL_CNF;
	
	//Set Counter
	RTC->CNTH = bcdtime >> 16;
	RTC->CNTL = (bcdtime & RTC_LSB_MASK);
	
	//ExitConfig
	RTC->CRL &= (uint16_t)~((uint16_t)RTC_CRL_CNF); 

}

/*
 * RTC_GetTime()
 * Retreive current RTC time in binary format
*/

void RTC_GetTime(time_t *ptime)
{
   ptime->hours   = bcd2byte(RTC->CNTL/3600);
   ptime->minutes = bcd2byte((RTC->CNTL%3600)/60);
   ptime->seconds = bcd2byte((RTC->CNTL%3600)%60);
}

/*
 * Converts 2 digit Decimal to BCD format
 * param: 	Byte to be converted.
 * retval: BCD Converted byte
 */

static uint16_t byte2bcd(uint16_t byte)
{
  uint16_t bcdhigh = 0;

  while (byte >= 10)
  {
    bcdhigh++;
    byte -= 10;
  }

  return  ((uint16_t)(bcdhigh << 4) | byte);
}

/*
 * Convert from 2 digit BCD to Binary
 * param  BCD value to be converted.
 * retval Converted word
 */

static uint16_t bcd2byte(uint16_t bcd)
{
  uint16_t tmp = 0;

  tmp = ((uint16_t)(bcd & (uint16_t)0xF0) >> (uint16_t)0x4) * 10;

  return (tmp + (bcd & (uint16_t)0x0F));
}
