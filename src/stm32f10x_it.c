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


extern  int tch_x    , tch_y  , tch_z ;
extern  int tch_int , tch_on  , tch_on_ ;
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
unsigned char ClockLEDOn;               /* On for 100 ms every 150 ms         */
unsigned char ClockLEDOff;              /* On for  50 ms every 150 ms         */
unsigned char ClockANI;                 /* Clock for Animation 150 ms         */


void SysTick_Handler (void) {           /* SysTick Interrupt Handler (10ms)   */
  ticks++;
  switch (ticks) {
    case 10:
      ClockLEDOff = 1;
      break;
  case 15:
      ticks       = 0;
      ClockANI    = 1;
      ClockLEDOn  = 1;
    default:
      break;
  }
}


//Single Point Reading
void ProcessInterrupt()
{
  tch_int = TS_Read (0x0B, 1);    /* Read Touch-screen interrupt status */
  //printf("hello interrupt");
  if (tch_int)
  {                  /* If Touch-screen interrupt active   */

    if (tch_int & 1)
	{            /* If touch detected                  */
      tch_on ^= 1;
      if (tch_on)
	  {
        tch_x = 0;
        tch_y = 0;
        tch_z = 0;
      }
    }
    if (tch_int & 2)
    {            /* If FIFO is above threshold         */
      tch_on   = 1;
      tch_z = TS_Read (0xD7, 4);  /* Read coordinates                   */
      tch_x = (tch_z >> 20) & 0x00000FFF;
      tch_y = (tch_z >>  8) & 0x00000FFF;
      tch_z = (tch_z >>  0) & 0x0000000F;

    }
    TS_Write (0x0B, 1, tch_int);
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
void NMI_Handler(void)
{
}

/**
  * @brief  This function handles Hard Fault exception.
  * @param  None
  * @retval None
  */
void HardFault_Handler(void)
{
  /* Go to infinite loop when Hard Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Memory Manage exception.
  * @param  None
  * @retval None
  */
void MemManage_Handler(void)
{
  /* Go to infinite loop when Memory Manage exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Bus Fault exception.
  * @param  None
  * @retval None
  */
void BusFault_Handler(void)
{
  /* Go to infinite loop when Bus Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Usage Fault exception.
  * @param  None
  * @retval None
  */
void UsageFault_Handler(void)
{
  /* Go to infinite loop when Usage Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles SVCall exception.
  * @param  None
  * @retval None
  */
void SVC_Handler(void)
{
}

/**
  * @brief  This function handles Debug Monitor exception.
  * @param  None
  * @retval None
  */
void DebugMon_Handler(void)
{
}

/**
  * @brief  This function handles PendSV_Handler exception.
  * @param  None
  * @retval None
  */
void PendSV_Handler(void)
{
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
void EXTI1_IRQHandler(void)
{
  if(EXTI_GetITStatus(EXTI_Line1) != RESET)
  {
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
void RTC_IRQHandler(void)
{



  if (RTC_GetITStatus(RTC_IT_SEC) != RESET)
  {
    // Czekaj na zakonczenie ewentualnych operacji zapisu do rejestrów RTC
    RTC_WaitForLastTask();

    // Wyczysc flage od przerwania RTC
    RTC_ClearITPendingBit(RTC_IT_SEC);

    // Zmiana stanu wyprowadzenia PB15 co 1s
//    GPIO_WriteBit(GPIOB, GPIO_Pin_15, (BitAction)(1 - GPIO_ReadOutputDataBit(GPIOB, GPIO_Pin_15)));

		stanRTC=RTC_GetCounter();
    //sprintf((char *)stanRTCTekst,"%16i\0",stanRTC);
    //LCD_WriteTextXY(stanRTCTekst,0,1);

//    LCD_WriteTextXY(stanRTCTekst,7,1);
  }
}

/**
  * @}
  */ 

/************* (C) COPYRIGHT 2009 Wuhan R&D Center, Embest *****END OF FILE****/
