/**
  ******************************************************************************
  * @file    RTC/Calendar/main.c 
  * @author  MCD Application Team
  * @version V3.5.0
  * @date    08-April-2011
  * @brief   Main program body
  ******************************************************************************/
#include "stm32f10x.h"                  // Device header
#include "stm32f10x_rtc.h"              // Keil::Device:StdPeriph Drivers:RTC
#include "stm32f10x_rcc.h"              // Keil::Device:StdPeriph Drivers:RCC
#include "stm32f10x_pwr.h"              // Keil::Device:StdPeriph Drivers:PWR
#include "stm32f10x_usart.h"            // Keil::Device:StdPeriph Drivers:USART
#include "stm32f10x_bkp.h"              // Keil::Device:StdPeriph Drivers:BKP
#include "usart.h"
#include <stdio.h>

#define RTCClockOutput_Enable  /* RTC Clock/64 is output on tamper pin(PC.13) */

__IO uint32_t TimeDisplay = 0;
USART_InitTypeDef USART_InitStructure;

//#include <time.h>
uint32_t Time_Regulate(void);
uint8_t USART_Scanf(uint32_t value);
void Time_Display(uint32_t TimeVar);
void RTC_IRQHandler(void);

#ifdef __GNUC__
/* With GCC/RAISONANCE, small printf (option LD Linker->Libraries->Small printf
   set to 'Yes') calls __io_putchar() */
#define PUTCHAR_PROTOTYPE int __io_putchar(int ch)
#else
#define PUTCHAR_PROTOTYPE int fputc(int ch, FILE *f)
#endif /* __GNUC__ */


//typedef struct
//{
//	uint8_t hours;
//	uint8_t minutes;
//	uint8_t seconds;
//} time_t;

// Main program

int main()
{
//	time_t		now;
	
// USART1 configured
	usart_init();

	
	__NVIC_SetPriorityGrouping((uint32_t)0x600);		/*!< 1 bits for pre-emption priority
                                                        3 bits for subpriority */
	NVIC_SetPriority(RTC_IRQn,1);
	NVIC_EnableIRQ(RTC_IRQn);
	
//	//NVIC configuration 
//	NVIC_InitTypeDef NVIC_InitStructure;

//  /* Configure one bit for preemption priority */
//  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);

//  /* Enable the RTC Interrupt */
//  NVIC_InitStructure.NVIC_IRQChannel = RTC_IRQn;
//  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
//  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
//  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
//  NVIC_Init(&NVIC_InitStructure);
	
	
	
	
	if (BKP_ReadBackupRegister(BKP_DR1) != 0xA5A5)			// BKP KULLANIMINI ÖGRENMEN LAZIM !
  {
    /* Backup data register value is not correct or not yet programmed (when
       the first time the program is executed) */

    printf("\r\n\n RTC not yet configured....");

    /* RTC Configuration */
    // Configure System Clock
	RCC->CFGR |=  RCC_CFGR_MCO_SYSCLK;     // Change here	

	RCC->APB2ENR |= RCC_APB2ENR_IOPAEN;		// Enable GPIOA clock

	GPIOA->CRH |= GPIO_CRH_MODE8_0 | (GPIO_CRH_CNF8&0x0) | GPIO_CRH_CNF8_1;			// Configure PA8 as Alternate function
	

	// Configure RTC Clock
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR | RCC_APB1Periph_BKP, ENABLE);		// Enable the PWR clock

	PWR_BackupAccessCmd(ENABLE);			// Enable access to RTC and Backup registers

	RCC_BackupResetCmd(ENABLE);				// Resets Backup Domain Config
	RCC_BackupResetCmd(DISABLE);				// Resets Backup Domain Config


	RCC_LSEConfig(RCC_LSE_ON);				// Start LSE clock

	while ( (RCC->BDCR & RCC_BDCR_LSERDY) != RCC_BDCR_LSERDY);	// Wait till LSE is ready
//	while (RCC_GetFlagStatus(RCC_FLAG_LSERDY) == RESET){}

	RCC_RTCCLKConfig(RCC_RTCCLKSource_LSE);		// Select LSE as RTC clock source

	RCC_RTCCLKCmd(ENABLE);								// Enable RTC clock

 /* Wait for RTC registers synchronization */
  RTC_WaitForSynchro();
					//    /* Clear RSF flag */
					//  RTC->CRL &= (uint16_t)~RTC_FLAG_RSF;
					//  /* Loop until RSF flag is set */
					//  while ((RTC->CRL & RTC_FLAG_RSF) == (uint16_t)RESET)

  /* Wait until last write operation on RTC registers has finished rtc.c*/
  RTC_WaitForLastTask();
					//    /* Loop until RTOFF flag is set */
					//  while ((RTC->CRL & RTC_FLAG_RTOFF) == (uint16_t)RESET)

  /* Enable the RTC Second */
  RTC_ITConfig(RTC_IT_SEC, ENABLE);			//RTC Second interrupt enable

  /* Wait until last write operation on RTC registers has finished */
  RTC_WaitForLastTask();

  /* Set RTC prescaler: set RTC period to 1sec */
  RTC_SetPrescaler(32767); /* RTC period = RTCCLK/RTC_PR = (32.768 KHz)/(32767+1) */

  /* Wait until last write operation on RTC registers has finished */
  RTC_WaitForLastTask();
  
  

    printf("\r\n RTC configured....");

    /* Adjust time by values entered by the user on the hyperterminal */
   /* Wait until last write operation on RTC registers has finished */
  RTC_WaitForLastTask();
  /* Change the current time */
  RTC_SetCounter(Time_Regulate());
  /* Wait until last write operation on RTC registers has finished */
  RTC_WaitForLastTask();

    BKP_WriteBackupRegister(BKP_DR1, 0xA5A5);
  }
  else
  {
    /* Check if the Power On Reset flag is set */
    if (RCC_GetFlagStatus(RCC_FLAG_PORRST) != RESET)
    {
      printf("\r\n\n Power On Reset occurred....");
    }
    /* Check if the Pin Reset flag is set */
    else if (RCC_GetFlagStatus(RCC_FLAG_PINRST) != RESET)
    {
      printf("\r\n\n External Reset occurred....");
    }

    printf("\r\n No need to configure RTC....");
    /* Wait for RTC registers synchronization */
    RTC_WaitForSynchro();

    /* Enable the RTC Second */
    RTC_ITConfig(RTC_IT_SEC, ENABLE);
    /* Wait until last write operation on RTC registers has finished */
    RTC_WaitForLastTask();
  }
	
	#ifdef RTCClockOutput_Enable
  /* Enable PWR and BKP clocks */
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR | RCC_APB1Periph_BKP, ENABLE);

  /* Allow access to BKP Domain */
  PWR_BackupAccessCmd(ENABLE);

  /* Disable the Tamper Pin */
  BKP_TamperPinCmd(DISABLE); /* To output RTCCLK/64 on Tamper pin, the tamper
                                 functionality must be disabled */

  /* Enable RTC Clock Output on Tamper Pin */
  BKP_RTCOutputConfig(BKP_RTCOutputSource_CalibClock);
#endif

  /* Clear reset flags */
  RCC_ClearFlag();

  /* Display time in infinite loop */	
  while (1)
  {
    /* If 1s has been elapsed */
    if (TimeDisplay == 1)
    {
      /* Display current time */
      Time_Display(RTC_GetCounter());
      TimeDisplay = 0;
    }
  }

}




//	// Set RTC time
//	now.hours   = 12;
//	now.minutes = 00;
//	now.seconds = 00;
//	
//	RTC_SetCounter(&now);
	

//	// Main loop
//	while(1)
//	{
////	   BSP_RTC_GetTime(&now);

//     //Suanki time yazdir      my_printf("RTC Time is %02d:%02d:%02d\r", now.hours, now.minutes, now.seconds);

//	   //Led AÇ
//	   //Delay koy
//	}



/**
  * @brief  Returns the time entered by user, using Hyperterminal.
  * @param  None
  * @retval Current time RTC counter value
  */
uint32_t Time_Regulate(void)
{
  uint32_t Tmp_HH = 0xFF, Tmp_MM = 0xFF, Tmp_SS = 0xFF;

  printf("\r\n==============Time Settings================");
  printf("\r\n  Please Set Hours");

  while (Tmp_HH == 0xFF)
  {
    Tmp_HH = USART_Scanf(23);
  }
  printf(":  %d", Tmp_HH);
  printf("\r\n  Please Set Minutes");
  while (Tmp_MM == 0xFF)
  {
    Tmp_MM = USART_Scanf(59);
  }
  printf(":  %d", Tmp_MM);
  printf("\r\n  Please Set Seconds");
  while (Tmp_SS == 0xFF)
  {
    Tmp_SS = USART_Scanf(59);
  }
  printf(":  %d", Tmp_SS);

  /* Return the value to store in RTC counter register */
  return((Tmp_HH*3600 + Tmp_MM*60 + Tmp_SS));
}

/**
  * @brief  Gets numeric values from the hyperterminal.
  * @param  None
  * @retval None
  */
uint8_t USART_Scanf(uint32_t value)
{
  uint32_t index = 0;
  uint32_t tmp[2] = {0, 0};

  while (index < 2)
  {
    /* Loop until RXNE = 1 */
    while (USART_GetFlagStatus(USART1, USART_FLAG_RXNE) == RESET)
    {}
    tmp[index++] = (USART_ReceiveData(USART1));
    if ((tmp[index - 1] < 0x30) || (tmp[index - 1] > 0x39))
    {
      printf("\n\rPlease enter valid number between 0 and 9");
      index--;
    }
  }
  /* Calculate the Corresponding value */
  index = (tmp[1] - 0x30) + ((tmp[0] - 0x30) * 10);
  /* Checks */
  if (index > value)
  {
    printf("\n\rPlease enter valid number between 0 and %d", value);
    return 0xFF;
  }
  return index;
}

/**
  * @brief  Displays the current time.
  * @param  TimeVar: RTC counter value.
  * @retval None
  */
void Time_Display(uint32_t TimeVar)
{
  uint32_t THH = 0, TMM = 0, TSS = 0;
  
  /* Reset RTC Counter when Time is 23:59:59 */
  if (RTC_GetCounter() == 0x0001517F)
  {
     RTC_SetCounter(0x0);
     /* Wait until last write operation on RTC registers has finished */
     RTC_WaitForLastTask();
  }
  
  /* Compute  hours */
  THH = TimeVar / 3600;
  /* Compute minutes */
  TMM = (TimeVar % 3600) / 60;
  /* Compute seconds */
  TSS = (TimeVar % 3600) % 60;

  printf("Time: %0.2d:%0.2d:%0.2d\r", THH, TMM, TSS);
}

/**
  * @brief  Retargets the C library printf function to the USART.
  * @param  None
  * @retval None
  */
PUTCHAR_PROTOTYPE
{
  /* Place your implementation of fputc here */
  /* e.g. write a character to the USART */
  USART_SendData(USART1, (uint8_t) ch);

  /* Loop until the end of transmission */
  while (USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET)
  {}

  return ch;
}

void RTC_IRQHandler(void)
{
  if (RTC_GetITStatus(RTC_IT_SEC) != RESET)
  {
    /* Clear the RTC Second interrupt */
    RTC_ClearITPendingBit(RTC_IT_SEC);

    /* Enable time update */
    TimeDisplay = 1;

    /* Wait until last write operation on RTC registers has finished */
    RTC_WaitForLastTask();
    
  }
}


#ifdef  USE_FULL_ASSERT

/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t* file, uint32_t line)
{
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */

  /* Infinite loop */
  while (1)
  {}
}

#endif
