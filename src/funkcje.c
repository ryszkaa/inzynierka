#include <stdio.h>
#include "stm32f10x.h"
#include "stm32_eval.h"
#include "stm32f10x_gpio.h"
#include "stm32f10x_adc.h"
#include "stm32f10x_spi.h"
#include <FreeRTOS.h>
#include "task.h"
#include "itoa.h"
#include "funkcje.h"






int TC77_temperature = 8;
int temperature_temp = 16;
int humidity, ch4, co;
char buffer[15];



/********ADC*********/

//void ADC_conf(void){
//
//    ADC_InitTypeDef ADC_InitStruc;
//
//    ADC_InitStruct.ADC_Mode = ADC_Mode_Independent;
//    ADC_InitStruct.ADC_ScanConvMode = DISABLE;
//    ADC_InitStruct.ADC_ContinuousConvMode = ENABLE;
//    ADC_InitStruct.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;
//    ADC_InitStruct.ADC_DataAlign = ADC_DataAlign_Right;
//    ADC_InitStruct.ADC_NbrOfChannel= 1;
//    ADC_StructInit(&ADC_InitStruct);
//
//    ADC_RegularChannelConfig(ADC1, ADC_Channel_14, 1, ADC_SampleTime_71Cycles5);
//
//    ADC_DMACmd(ADC1, ENABLE);
//    ADC_Cmd(ADC1, ENABLE);
//
//    ADC_ResetCalibration(ADC1);
//    while(ADC_GetCalibrationStatus(ADC1));
//    ADC_StartCalibration(ADC1);
//    while(ADC_GetCalibrationStatus(ADC1));
//
//    ADC_SoftwareStartConvCmd(ADC1, ENABLE);
//
//
//}

//TC77
//void SPI_conf(void) {
//
//	SPI_InitTypeDef SPI_InitStructure;
//
//	SPI_StructInit(&SPI_InitStructure);
//
//	//Wlacz NSS
//	SPI_SSOutputCmd(SPI2, ENABLE);
//
//	//wlacz SPI1
//	SPI_Cmd(SPI2, ENABLE);
//
//	//Odbieranie danych
//
//	while (1) {
//		//spi w tryb master=nss w stan niski
//		SPI_InitStructure.SPI_Mode = SPI_Mode_Master;
//		SPI_Init(SPI2, &SPI_InitStructure);
//
//		//czekaj na dane
//		while (SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_RXNE) == RESET);
//
//
//		TC77_temperature = SPI_I2S_ReceiveData(SPI2) >> 3;
//
//		//spi w tyrb slave = nss stan wysoki
//		SPI_InitStructure.SPI_Mode = SPI_Mode_Slave;
//		SPI_Init(SPI2, &SPI_InitStructure);
//
//		//obliczanie temp
//		temperature_temp = TC77_temperature ;
//
//		sprintf(buffer, "Temp. : %d *C", temperature_temp);
//		GLCD_DisplayString(7, 0, buffer);
//
//	}
//}


/***********/

