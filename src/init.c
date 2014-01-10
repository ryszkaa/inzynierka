#include <stdio.h>
#include "stm32f10x.h"
#include "stm32_eval.h"
#include "stm32f10x_gpio.h"
#include "stm32f10x_adc.h"

void init (){

	GPIO_InitTypeDef GPIO_InitStructure;
	  EXTI_InitTypeDef EXTI_InitStructure;
	  NVIC_InitTypeDef NVIC_InitStructure;


	  SystemInit();

	  SysTick_Config(SystemFrequency/100-1);/* Generate interrupt every 10 ms     */

	  //STM_EVAL_PBInit(Button_KEY, Mode_EXTI);
	  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB | RCC_APB2Periph_AFIO, ENABLE);

	  /* Configure Button pin as input floating */
	  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;
	  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	  GPIO_Init(GPIOB, &GPIO_InitStructure);

	  /* Connect Button EXTI Line to Button GPIO Pin */
	  GPIO_EXTILineConfig(GPIO_PortSourceGPIOB, GPIO_PinSource1);

	  /* Configure Button EXTI line */
	  EXTI_InitStructure.EXTI_Line = EXTI_Line1;
	  EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;

	  EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;

	  EXTI_InitStructure.EXTI_LineCmd = ENABLE;
	  EXTI_Init(&EXTI_InitStructure);

	  /* Enable and set Button EXTI Interrupt to the lowest priority */
	  NVIC_InitStructure.NVIC_IRQChannel = EXTI1_IRQn;
	  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x0F;
	  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x0F;
	  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;

	  NVIC_Init(&NVIC_InitStructure);




	  /* USART configuration ------------------------------------------------------*/
	  USART_Configuration();

	  printf(" Prepare for TS ! \n");



	  RCC->APB2ENR |=  1 <<  2;             /* Enable GPIOA clock                 */
	  RCC->APB2ENR |=  1 <<  3;             /* Enable GPIOB clock                 */
	  RCC->APB2ENR |=  1 <<  4;             /* Enable GPIOC clock                 */
	  GPIOA->CRL   &= 0xFFFFFFF0;
	  GPIOA->CRL   |= 0x00000004;
	  GPIOB->CRL   &= 0x0FFFFFFF;
	  GPIOB->CRL   |= 0x40000000;
	  GPIOC->CRH   &= 0xFF0FFFFF;
	  GPIOC->CRH   |= 0x00400000;

	  	/* Configure the GPIO for Joystick                                          */
	  RCC->APB2ENR |=  1 <<  5;             /* Enable GPIOD clock                 */
	  GPIOD->CRH   &= 0x00000FFF;
	  GPIOD->CRH   |= 0x44444000;

	  	/* Setup GPIO for LEDs                                                      */
	  RCC->APB2ENR |=  1 <<  6;             /* Enable GPIOE clock                 */
	  GPIOE->CRH    = 0x33333333;           /* Configure the GPIO for LEDs        */

	  	/* Setup and initialize ADC converter                                       */
	  RCC->APB2ENR |=  1 <<  9;             /* Enable ADC1 clock                  */
	  GPIOC->CRL   &= 0xFFF0FFFF;           /* Configure PC4 as ADC.14 input      */
	  ADC1->SQR1    = 0x00000000;           /* Regular channel 1 conversion       */
	  ADC1->SQR2    = 0x00000000;           /* Clear register                     */
	  ADC1->SQR3    = 14 <<  0;             /* SQ1 = channel 14                   */
	  ADC1->SMPR1   =  5 << 12;             /* Channel 14 sample time is 55.5 cyc */
	  ADC1->SMPR2   = 0x00000000;           /* Clear register                     */
	  ADC1->CR1     =  1 <<  8;             /* Scan mode on                       */
	  ADC1->CR2     = (1 << 20) |           /* Enable external trigger            */
	                  (7 << 17) |           /* EXTSEL = SWSTART                   */
	                  (1 <<  1) |           /* Continuous conversion              */
	                  (1 <<  0) ;           /* ADC enable                         */
	  ADC1->CR2    |=  1 <<  3;             /* Initialize calibration registers   */
	  while (ADC1->CR2 & (1 << 3));         /* Wait for initialization to finish  */
	  ADC1->CR2    |=  1 <<  2;             /* Start calibration                  */
	  while (ADC1->CR2 & (1 << 2));         /* Wait for calibration to finish     */
	  ADC1->CR2    |=  1 << 22;             /* Start first conversion             */


};



