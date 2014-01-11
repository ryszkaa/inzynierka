#include "stm32f10x_gpio.h"
#include "stm32f10x_rcc.h"
#include "stm32f10x_flash.h"
#include "stm32f10x_adc.h"
#include "stm32f10x_spi.h"
#include "stm32f10x_rtc.h"
#include "stm32f10x.h"
#include "stm32_eval.h"
#include "init_board.h"
#include "RTC.h"

ErrorStatus HSEStartUpStatus;
void USART_Configuration(void);
void prvSetupHardware(void) {
	RCC_Conf();

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE); //wlacz taktowanie portu GPIO A
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE); //wlacz taktowanie portu GPIO B
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE); //wlacz taktowanie portu GPIO C
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOE, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI1, ENABLE); //wlacz taktowanie SPI
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_SPI2, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE); //wlacz taktowanie ADC1
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR | RCC_APB1Periph_BKP, ENABLE); //rtc
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);
//	RCC_APB1PeriphClockCmd(RCC_APB)

	//Initialization peripheral and modules

	GPIO_Conf();
	NVIC_Conf();
	USART_Configuration();
	TouchPanel_Conf();
	RTC_Configuration();

}

void ADC_Conf(void) {

	ADC_InitTypeDef ADC_InitStructure;

	ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;
	ADC_InitStructure.ADC_ScanConvMode = DISABLE;
	ADC_InitStructure.ADC_ContinuousConvMode = ENABLE;
	ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;
	ADC_InitStructure.ADC_NbrOfChannel = 1;

	ADC_Init(ADC1, &ADC_InitStructure);

	ADC_RegularChannelConfig(ADC1, ADC_Channel_10, 1, ADC_SampleTime_55Cycles5);
	//ADC_RegularChannelConfig(ADC1, ADC_Channel_11, 1, ADC_SampleTime_71Cycles5);

	ADC_Cmd(ADC1, ENABLE);

	ADC_ResetCalibration(ADC1);
	while (ADC_GetCalibrationStatus(ADC1))
		;
	ADC_StartCalibration(ADC1);
	while (ADC_GetCalibrationStatus(ADC1))
		;

	ADC_SoftwareStartConvCmd(ADC1, ENABLE);

}

/***configuration GPIO***/

void GPIO_Conf(void) {

	GPIO_InitTypeDef GPIO_InitStructure;
	EXTI_InitTypeDef EXTI_InitStructure;

	/* Configure the GPIO_LED pin */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2 | GPIO_Pin_3 | GPIO_Pin_4;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOE, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8 | GPIO_Pin_9 | GPIO_Pin_10 | GPIO_Pin_11 | GPIO_Pin_12 | GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_Init(GPIOB, &GPIO_InitStructure);

	/* Configure Button pin as input floating (PB1, interrupt from Touch Screen) */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	/* Connect Button EXTI Line to Button GPIO Pin */
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOB, GPIO_PinSource1);

	// SPI1 : SCK, MISO and MOSI
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4 | GPIO_Pin_5 | GPIO_Pin_6 | GPIO_Pin_7;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	//spi2:
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12 | GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_Init(GPIOB, &GPIO_InitStructure);

	//adc
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN; //wejscie analogowe
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOC, &GPIO_InitStructure);

	/* Configure Button EXTI line */
	EXTI_InitStructure.EXTI_Line = EXTI_Line1;
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;

	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;

	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
	EXTI_Init(&EXTI_InitStructure);




}

void NVIC_Conf(void) {

	NVIC_InitTypeDef NVIC_InitStructure;

	/* Configure one bit for preemption priority */
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);

	/* Enable the RTC Interrupt */
	NVIC_InitStructure.NVIC_IRQChannel = RTC_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	/* Enable and set Button EXTI Interrupt to the lowest priority */
	NVIC_InitStructure.NVIC_IRQChannel = EXTI1_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x0F;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x0F;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);

	//konfiguracja przerwań pochodzących od RTC Alarm
	NVIC_InitStructure.NVIC_IRQChannel = RTCAlarm_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);

//	NVIC_InitTypeDef NVIC_config;
//	NVIC_config.NVIC_IRQChannel = ADC1_2_IRQn;
//	NVIC_config.NVIC_IRQChannelSubPriority = 0;
//	NVIC_config.NVIC_IRQChannelCmd = ENABLE;
//	NVIC_Init(&NVIC_config);

//#ifdef  VECT_TAB_RAM
//	// Jezeli tablica wektorow w RAM, to ustaw jej adres na 0x20000000
//	NVIC_SetVectorTable(NVIC_VectTab_RAM, 0x0);
//#else  // VECT_TAB_FLASH
//	// W przeciwnym wypadku ustaw na 0x08000000
//	NVIC_SetVectorTable(NVIC_VectTab_FLASH, 0x0);
//#endif
}

void RCC_Conf(void) {
//	ErrorStatus HSEStartUpStatus;
//
//	RCC_DeInit();/* SYSCLK, HCLK, PCLK2 and PCLK1 configuration -----------------------------*/
//	/* RCC system reset(for debug purpose) */
//	RCC_HSEConfig(RCC_HSE_ON); /* Enable HSE */
//
//	HSEStartUpStatus = RCC_WaitForHSEStartUp();/* Wait till HSE is ready */
//
//	if (HSEStartUpStatus == SUCCESS) {
//		FLASH_PrefetchBufferCmd(FLASH_PrefetchBuffer_Enable);/* Enable Prefetch Buffer */
//		FLASH_SetLatency(FLASH_Latency_2);/* Flash 2 wait state */

	RCC_HCLKConfig(RCC_SYSCLK_Div1);/* HCLK = SYSCLK */
	RCC_PCLK2Config(RCC_HCLK_Div1);/* PCLK2 = HCLK */
	RCC_PCLK1Config(RCC_HCLK_Div2);/* PCLK1 = HCLK/2 */
	RCC_ADCCLKConfig(RCC_PCLK2_Div8); // ADCCLK = PCLK2/4 = 56 MHz /4 = 14 MHz (maksymalna mozliwa)

//#ifdef STM32F10X_CL
//
//		RCC_PREDIV2Config(RCC_PREDIV2_Div5);/* Configure PLLs */
//		RCC_PLL2Config(RCC_PLL2Mul_8);/* PLL2 configuration: PLL2CLK = (HSE / 5) * 8 = 40 MHz */
//
//		RCC_PLL2Cmd(ENABLE);
//
//		while (RCC_GetFlagStatus(RCC_FLAG_PLL2RDY) == RESET) {
//		}
//
//		RCC_PREDIV1Config(RCC_PREDIV1_Source_PLL2, RCC_PREDIV1_Div5);/* PLL configuration: PLLCLK = (PLL2 / 5) * 9 = 72 MHz */
//		RCC_PLLConfig(RCC_PLLSource_PREDIV1, RCC_PLLMul_9);
//#else

	//RCC_PLLConfig(RCC_PLLSource_PREDIV1, RCC_PLLMul_7);/* PLLCLK = 8MHz * 7 = 56 MHz */
//#endif
//		RCC_PLLCmd(ENABLE);
//
//		while (RCC_GetFlagStatus(RCC_FLAG_PLLRDY) == RESET) {
//		}
//
//		RCC_SYSCLKConfig(RCC_SYSCLKSource_PLLCLK);/* Select PLL as system clock source */
//
//		while (RCC_GetSYSCLKSource() != 0x08) {
//		}
//
//	} else { /* If HSE fails to start-up, the application will have wrong clock configuration.
//	 User can add here some code to deal with this error */
//
//		/* Go to infinite loop */
//		while (1) {
//		}
//	}
}

void USART_Configuration(void) {
	USART_InitTypeDef USART_InitStructure;
	/* USARTx configuration ------------------------------------------------------*/
	/* USARTx configured as follow:
	 - BaudRate = 9600 baud
	 - Word Length = 8 Bits
	 - Two Stop Bit
	 - Odd parity
	 - Hardware flow control disabled (RTS and CTS signals)
	 - Receive and transmit enabled
	 */
	USART_InitStructure.USART_BaudRate = 115200;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;

	STM_EVAL_COMInit(COM1, &USART_InitStructure);
}

/**
 * @brief  Redefine fputc function.
 * @param  ...
 * @retval ...
 */

