/******************** (C) COPYRIGHT 2009 Embest Info&Tech Co.,LTD. *************
 * File Name          : stm32f10x_it.c
 * Author             : Wuhan R&D Center, Embest
 * Date First Issued  : 07/12/2009
 * Description        : Main Interrupt Service Routines.
 *             		   This file provides template for all exceptions handler
 *                      and peripherals interrupt service routine.
 ********************************************************************************
 ********************************************************************************
 * History:
 * 07/12/2009		 : V1.0		   initial version
 *******************************************************************************/

/* Includes ------------------------------------------------------------------*/
#include <stdio.h>
#include "stm32f10x_it.h"
#include "stm32_eval.h"
#include "TS_I2C.h"
#include "stm32f10x_rtc.h"
#include "main.h"
#include <FreeRTOS.h>
#include "FreeRTOSConfig.h"
#include "queue.h"
#include "main.h"

#include "RTC.h"

extern volatile xQueueHandle xGLCDQueue;

/** @addtogroup STM32F10x_StdPeriph_Examples
 * @{
 */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

unsigned long ticks = 0;
unsigned char ClockLEDOn; /* On for 100 ms every 150 ms         */
unsigned char ClockLEDOff; /* On for  50 ms every 150 ms         */
unsigned char ClockANI; /* Clock for Animation 150 ms         */

void SysTick_Handler(void) { /* SysTick Interrupt Handler (10ms)   */
	ticks++;
	switch (ticks) {
	case 10:
		ClockLEDOff = 1;
		break;
	case 15:
		ticks = 0;
		ClockANI = 1;
		ClockLEDOn = 1;
	default:
		break;
	}
}

//Single Point Reading

int tch_x = 0, tch_y = 0, tch_z = 0;
int tch_int = 0, tch_on = 0, tch_on_ = 0;
GLCDQueueItem xMessageToSend;
GLCDQueueItem xMessageToSendAlarm;
portBASE_TYPE xHigherPriorityTaskWoken;
void ProcessInterrupt() {

	tch_int = TS_Read(0x0B, 1); /* Read Touch-screen interrupt status */
	//printf("hello interrupt");
	if (tch_int) { /* If Touch-screen interrupt active   */

		if (tch_int & 1) { /* If touch detected                  */
			tch_on ^= 1;
			if (tch_on) {
				tch_x = 0;
				tch_y = 0;
				tch_z = 0;
			}
		}
		if (tch_int & 2) { /* If FIFO is above threshold         */
			tch_on = 1;
			tch_z = TS_Read(0xD7, 4); /* Read coordinates                   */
			tch_x = (tch_z >> 20) & 0x00000FFF;
			tch_y = (tch_z >> 8) & 0x00000FFF;
			tch_z = (tch_z >> 0) & 0x0000000F;

		}
		TS_Write(0x0B, 1, tch_int);

		xMessageToSend.Mode = Touch;
		xMessageToSend.CoordinatesX = tch_x;
		xMessageToSend.CoordinatesY = tch_y;
		xMessageToSend.CoordinatesZ = tch_on;

		if (!(xQueueSendToBackFromISR(xGLCDQueue, &xMessageToSend, &xHigherPriorityTaskWoken))) {
			printf("blad kolejki\n");
		}

	}
}
/******************************************************************************/
/*            Cortex-M3 Processor Exceptions Handlers                         */
/******************************************************************************/

/**
 * @brief  This function handles NMI exception.
 * @param  None
 * @retval None
 */
void NMI_Handler(void) {
}

/**
 * @brief  This function handles Hard Fault exception.
 * @param  None
 * @retval None
 */
void HardFault_Handler(void) {
	/* Go to infinite loop when Hard Fault exception occurs */
	while (1) {
	}
}

/**
 * @brief  This function handles Memory Manage exception.
 * @param  None
 * @retval None
 */
void MemManage_Handler(void) {
	/* Go to infinite loop when Memory Manage exception occurs */
	while (1) {
	}
}

/**
 * @brief  This function handles Bus Fault exception.
 * @param  None
 * @retval None
 */
void BusFault_Handler(void) {
	/* Go to infinite loop when Bus Fault exception occurs */
	while (1) {
	}
}

/**
 * @brief  This function handles Usage Fault exception.
 * @param  None
 * @retval None
 */
void UsageFault_Handler(void) {
	/* Go to infinite loop when Usage Fault exception occurs */
	while (1) {
	}
}

/**
 * @brief  This function handles SVCall exception.
 * @param  None
 * @retval None
 */
void SVC_Handler(void) {
}

/**
 * @brief  This function handles Debug Monitor exception.
 * @param  None
 * @retval None
 */
void DebugMon_Handler(void) {
}

/**
 * @brief  This function handles PendSV_Handler exception.
 * @param  None
 * @retval None
 */
void PendSV_Handler(void) {
}

/**
 * @brief  This function handles SysTick Handler.
 * @param  None
 * @retval None
 */

/******************************************************************************/
/*                 STM32F10x Peripherals Interrupt Handlers                   */
/*  Add here the Interrupt Handler for the used peripheral(s) (PPP), for the  */
/*  available peripheral interrupt handler's name please refer to the startup */
/*  file (startup_stm32f10x_xx.s).                                            */
/******************************************************************************/

/**
 * @brief  This function handles PPP interrupt request.
 * @param  None
 * @retval None
 */
/*void PPP_IRQHandler(void)
 {
 }*/
void EXTI1_IRQHandler(void) {
	if (EXTI_GetITStatus(EXTI_Line1) != RESET) {
		/* Toggle LED1 */

		ProcessInterrupt();

		/* Clear the Key Button EXTI line pending bit */
		EXTI_ClearITPendingBit(EXTI_Line1);
	}
}

/**
 * @}
 */

/*******************************************************************************
 * Function Name  : RTC_IRQHandler
 * Description    : This function handles RTC global interrupt request.
 * Input          : None
 * Output         : None
 * Return         : None
 *******************************************************************************/

volatile unsigned long int stanRTC;

volatile unsigned long int godziny, minuty, sekundy;

volatile unsigned long int StanAlarm = 0;

volatile unsigned long int godzinyAlarm, minutyAlarm, sekundyAlarm;

int8_t stanRTCTekst[GLCD_TEXT_SIZE] = { "null" };
volatile uint8_t AlarmStatus = 0;
void RTC_IRQHandler(void) {

	if (RTC_GetITStatus(RTC_IT_SEC) != RESET) {
		// Czekaj na zakonczenie ewentualnych operacji zapisu do rejestrów RTC
		RTC_WaitForLastTask();

		// Wyczysc flage od przerwania RTC
		RTC_ClearITPendingBit(RTC_IT_SEC);
		//sprawdzenie czy RTC się nie przekręcił 24h
		if (RTC_GetCounter() >= (86400 - 1)) {
			RTC_SetCounter(RTC_GetCounter() % (86400 - 1));
		}
		stanRTC = RTC_GetCounter() % (86400 - 1);

		sekundy = stanRTC % 60; //liczba sekund od ostatniej pelnej minuty
		minuty = (long unsigned int) (stanRTC) / 60; //dzielenie calkowite, "usuniecie" sekund, czas pracy w samych minutach
		godziny = (long unsigned int) (minuty) / 60; //dzielenie calkowite, "usuniecie" minut, czas pracy w samych godzinach
		minuty = minuty % 60; //liczba minut od ostatniej pelnej godziny

		sprintf((char *) stanRTCTekst, "Zegar: %3lu:%02lu:%02lu", godziny, minuty, sekundy);

		xMessageToSend.Mode = Text;
		xMessageToSend.CoordinatesX = 1;
		xMessageToSend.CoordinatesY = 0;

		xMessageToSend.Text = &stanRTCTekst;

		if (!(xQueueSendToBackFromISR(xGLCDQueue, &xMessageToSend, &xHigherPriorityTaskWoken))) {
			printf("blad kolejki\n");
		}

		if (AlarmStatus) {
			StanAlarm++;
			sekundyAlarm = StanAlarm % 60; //liczba sekund od ostatniej pelnej minuty
			minutyAlarm = (long unsigned int) (StanAlarm) / 60; //dzielenie calkowite, "usuniecie" sekund, czas pracy w samych minutach
			godzinyAlarm = (long unsigned int) (minutyAlarm) / 60; //dzielenie calkowite, "usuniecie" minut, czas pracy w samych godzinach
			minutyAlarm = minutyAlarm % 60; //liczba minut od ostatniej pelnej godziny
			sprintf((char *) stanRTCTekst, "Timer: %3lu:%02lu:%02lu", godzinyAlarm, minutyAlarm, sekundyAlarm);

			xMessageToSendAlarm.Mode = Text;
			xMessageToSendAlarm.CoordinatesX = 2;
			xMessageToSendAlarm.CoordinatesY = 0;

			xMessageToSendAlarm.Text = &stanRTCTekst;

			if (!(xQueueSendToBackFromISR(xGLCDQueue, &xMessageToSendAlarm, &xHigherPriorityTaskWoken))) {
				printf("blad kolejki\n");
			}
		}

	}
}

void RTCAlarm_IRQHandler(void) {

	if (EXTI_GetITStatus(EXTI_Line17) != RESET) {
		//wyczyszczenie flagi przerwania
		EXTI_ClearITPendingBit(EXTI_Line17);
		//ustawienie zmiennej do żądania generowania sygnału alarmu
		//odczekanie na zakończenie operacji na RTC
		RTC_WaitForLastTask();
		//wyczyszczenie flagi przerwania
		RTC_ClearITPendingBit(RTC_IT_ALR);
		//odczekanie na zakończenie operacji na RTC
		RTC_WaitForLastTask();

		//konfiguracja przerwań pochodzących od RTC Alarm
		EXTI_InitTypeDef EXTI_InitStructure;
		EXTI_ClearITPendingBit(EXTI_Line17);
		EXTI_InitStructure.EXTI_Line = EXTI_Line17;
		EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
		EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;
		EXTI_InitStructure.EXTI_LineCmd = DISABLE;
		EXTI_Init(&EXTI_InitStructure);
		//generowanie przerwania przez RTC Alarm
		RTC_ITConfig(RTC_IT_ALR, ENABLE);
		//odczekanie na zakończenie operacji
		RTC_WaitForLastTask();

		AlarmStatus = 0;

		sprintf((char *) stanRTCTekst, "Timer: 0:00:00");

		xMessageToSendAlarm.Mode = Text;
		xMessageToSendAlarm.CoordinatesX = 2;
		xMessageToSendAlarm.CoordinatesY = 0;

		xMessageToSendAlarm.Text = &stanRTCTekst;

		if (!(xQueueSendToBackFromISR(xGLCDQueue, &xMessageToSendAlarm, &xHigherPriorityTaskWoken))) {
			printf("blad kolejki\n");
		}

	}
}
/**
 * @}
 */

/************* (C) COPYRIGHT 2009 Wuhan R&D Center, Embest *****END OF FILE****/
