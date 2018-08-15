/*
 * 			rtc.h
 *
 *  		Created on: 13.08.2018
 *      Author: Burak Yoruk
 */

#include "rtc.h"

/*
 * RTC_Config()
 * Setup RTC clock and start RTC
 */

void BSP_RTC_Clock_Config()
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
}

static void SystemClock_Config() 
{
	
	/*--- Use PA8 as LSE output ---*/

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
 * BSP_RTC_SetTime(uint32_t time)
 * - Sets RTC Prescalers
 * - Sets RTC time
 */

void BSP_RTC_SetTime(time_t *ptime)
{
	uint32_t	bcdtime;

	bcdtime = 	( (byte2bcd(ptime->hours))   <<16U) |
			( (byte2bcd(ptime->minutes)) <<8U)  |
			( (byte2bcd(ptime->seconds)) <<0U);

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
	
	

}


//	// Enable Write access for RTC registers
//	RTC->WPR = 0xCA;
//	RTC->WPR = 0x53;

//	// Enter Init
//	RTC->ISR |= RTC_ISR_INIT;
//	while ((RTC->ISR & RTC_ISR_INITF) != RTC_ISR_INITF);

//	// Setup prescalers for 1s RTC clock
//	RTC->PRER = 0x007F00FF;

//	// Set time
//	RTC->TR = bcdtime;

//	// Exit Init
//	RTC->ISR &= ~RTC_ISR_INIT;

//	// Disable Write access for RTC registers
//	RTC->WPR = 0xFE;
//	RTC->WPR = 0x64;
