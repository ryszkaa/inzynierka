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

#define RTCClockOutput_Enable  /* RTC Clock/64 is output on tamper pin(PC.13) */

void RTC_Configuration(void) {
//  /* Allow access to BKP Domain */
//  PWR_BackupAccessCmd(ENABLE);
//
//  /* Reset Backup Domain */
//  BKP_DeInit();
//
//  /* Enable LSE */
//  RCC_LSEConfig(RCC_LSE_ON);
//  /* Wait till LSE is ready */
//  while (RCC_GetFlagStatus(RCC_FLAG_LSERDY) == RESET)
//  {}
//
//  /* Select LSE as RTC Clock Source */
//  RCC_RTCCLKConfig(RCC_RTCCLKSource_LSE);
//
//  /* Enable RTC Clock */
//  RCC_RTCCLKCmd(ENABLE);
//
//  /* Wait for RTC registers synchronization */
//  RTC_WaitForSynchro();
//
//  /* Wait until last write operation on RTC registers has finished */
//  RTC_WaitForLastTask();
//
//  /* Enable the RTC Second */
//  RTC_ITConfig(RTC_IT_SEC, ENABLE);
//
//  /* Wait until last write operation on RTC registers has finished */
//  RTC_WaitForLastTask();
//
//  /* Set RTC prescaler: set RTC period to 1sec */
//  RTC_SetPrescaler(32767); /* RTC period = RTCCLK/RTC_PR = (32.768 KHz)/(32767+1) */
//
//  /* Wait until last write operation on RTC registers has finished */
//  RTC_WaitForLastTask();
//  RCC_ClearFlag();
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
}


unsigned char stanRTCTekst[17] = { "null" };
void get_time(void) {

	unsigned long int godziny, minuty, sekundy;

	sekundy = stanRTC % 60; //liczba sekund od ostatniej pelnej minuty
	minuty = (long int) (stanRTC) / 60; //dzielenie calkowite, "usuniecie" sekund, czas pracy w samych minutach
	godziny = (long int) (minuty) / 60; //dzielenie calkowite, "usuniecie" minut, czas pracy w samych godzinach
	minuty = minuty % 60; //liczba minut od ostatniej pelnej godziny

	sprintf((char *) stanRTCTekst, "Zegar: %3i:%02i:%02i", godziny, minuty, sekundy);
}
