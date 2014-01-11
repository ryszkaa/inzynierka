#include "stm32f10x_gpio.h"
#include "stm32f10x_rcc.h"
#include "stm32f10x_flash.h"
#include "stm32f10x_adc.h"
#include "stm32f10x_spi.h"
#include "stm32f10x_rtc.h"
#include "stm32f10x.h"
#include "stm32_eval.h"
#include "init_board.h"
#include "stm32f10x_pwr.h"
#include "stm32f10x_bkp.h"
#include "RTC.h"
#include <string.h>
#include <stdio.h>

#define RTCClockOutput_Enable  /* RTC Clock/64 is output on tamper pin(PC.13) */

void RTC_Configuration(void) {

	PWR_BackupAccessCmd(ENABLE); //wlaczenie rejestrow domeny Backup
	RCC_LSEConfig(RCC_LSE_ON);
	while (RCC_GetFlagStatus(RCC_FLAG_LSERDY) == RESET)
		;
	RCC_RTCCLKConfig(RCC_RTCCLKSource_LSE); //LSE bedzie zrodlem sygnalu zegarowego dla RTC
	RCC_RTCCLKCmd(ENABLE); //Wlaczenie taktowania RTC
	RTC_WaitForSynchro(); //Oczekiwanie na synchronizacje
	RTC_WaitForLastTask(); //Oczekiwanie az rejestry RTC zostana zapisane
	RTC_SetPrescaler(32767); //Okres RTC bedzie wynosil 1s  RTC period = RTCCLK/RTC_PR = (32.768 KHz)/(32767+1)
	RTC_WaitForLastTask();
	RTC_ITConfig(RTC_IT_SEC, ENABLE); // Wlacz przerwanie "sekundnika" RTC
	RTC_WaitForLastTask();
	RCC_ClearFlag();

	//RTC_SetAlarm();

//	  /* Wait until last write operation on RTC registers has finished */
//	  RTC_WaitForLastTask();
//	  /* Change the current time */
//	  RTC_SetCounter(84540);
//	  /* Wait until last write operation on RTC registers has finished */
//	  RTC_WaitForLastTask();



}

