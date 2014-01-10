/* *********************************
 * *******Praca inżynierska*********
 * *********************************
 * ********Aleksandra Rys***********
 * **Elektronika i Telekomunikacja**
 * *********************************
 * *********************************
 * ********************************* */
#include <stdio.h>
#include "stm32f10x.h"
#include "stm32_eval.h"
#include "stm32f10x_gpio.h"
#include "TS_I2C.h"
#include "GLCD.h"
#include "stm32f10x_adc.h"
#include "stm32f10x_spi.h"
#include "init_board.h"
#include <FreeRTOS.h>
#include "task.h"
#include "itoa.h"
#include "touch_panel.h"
#include "FreeRTOSConfig.h"
#include "LCD.h"
#include "stm32f10x_rcc.h"
#define SR 8
void prvSetupHardware(void);

xTaskHandle xHandleLCD;
xTaskHandle xHandlesensor;
int tc77_temperatura = 5;
int tmp = 0;
volatile unsigned char temperatura[25];
volatile unsigned char wartoscADC1VTekst[25];

void temp_tc77(void) {
	//volatile unsigned long int i;
	int temperatura_temp = 4;
	SPI_InitTypeDef SPI_InitStruct;

	SPI_InitStruct.SPI_Direction = SPI_Direction_1Line_Rx; //transmisja z wykorzystaniem jednej linii, transmisja jednokierunkowa
	SPI_InitStruct.SPI_DataSize = SPI_DataSize_16b; //16-bit ramka danych
	SPI_InitStruct.SPI_CPOL = SPI_CPOL_Low; //stan sygnalu taktujacego przy braku transmisji - niski
	SPI_InitStruct.SPI_CPHA = SPI_CPHA_1Edge; //aktywne zbocze sygnalu taktujacego - 1-sze zbocze
	SPI_InitStruct.SPI_NSS = SPI_NSS_Hard; //sprzetowa obsluga linii NSS (CS)
	SPI_InitStruct.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_256; //prescaler szybkosci tansmisji  36MHz/256=140.625kHz
	SPI_InitStruct.SPI_FirstBit = SPI_FirstBit_MSB; //pierwszy bit w danych najbardziej znaczacy
	SPI_InitStruct.SPI_CRCPolynomial = 7;
	SPI_InitStruct.SPI_Mode = SPI_Mode_Master;
	SPI_Init(SPI2, &SPI_InitStruct);
	// GPIO_WriteBit(GPIOB, GPIO_Pin_12, Bit_RESET);
	while (SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_RXNE) == RESET) {

	}




	tc77_temperatura = SPI_I2S_ReceiveData(SPI2) >> 3;

	SPI_InitStruct.SPI_Mode = SPI_Mode_Slave;
	SPI_Init(SPI2, &SPI_InitStruct);
	// GPIO_WriteBit(GPIOB, GPIO_Pin_12, Bit_SET);
	temperatura_temp = tc77_temperatura *6;

	sprintf(temperatura, "Temperatura: %d,%01d*C", temperatura_temp / 100, (temperatura_temp % 100) / 10);
	printf("poieram temp %s \n", temperatura);

}
//STM_EVAL_COMInit(COM1, &USART_InitStructure);
void humidity(void) {
	static uint16_t sr[SR];
	static uint8_t idx;
	uint32_t sr1=0;
	uint8_t i;

	sr[ idx++ & (SR-1) ] = ADC_GetConversionValue(ADC1);


	for(i=0; i<SR; i++) sr1+=sr[i];
	sr1 /= SR;


	uint32_t wartoscADC1 = 0;
	uint32_t wartoscADC1V = 0;

//	while (!ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC))
//		; //odczekaj na zakonczenie konwersji
	wartoscADC1 = sr1;
	wartoscADC1V = ((wartoscADC1 * 100 ) );
	wartoscADC1V >>=12;

	sprintf((char *) wartoscADC1VTekst, "Wiglotnosc: %d%%", wartoscADC1V); //Przekszta³cenie wyniku na tekst, dzielenie calkowite wyzancza wartosc w V, dzielenie modulo - czesc po przecinku
	printf("wilgotnosc: %d \n", ADC_GetConversionValue(ADC1));

}
/******functions******/
void lcd_task(void * pvParameters) { //* pvParameters

	GLCD_Init();
	GLCD_Clear(Blue);
	GLCD_Menu();
	for (;;) {

	}

}
void sensor_task(void * pvParameters) {
//	volatile unsigned long int i;
	portTickType t;
	//static const unsigned char stopienSymbol[8] = { 0x06, 0x09, 0x09, 0x06, 0x00, 0x00, 0x00, 0 }; //symbol stopnia

	SPI_Conf();
	ADC_Conf();
	t = xTaskGetTickCount();
	while (1) {

		temp_tc77();
		vTaskDelayUntil(&t, 1000);

		humidity();
		vTaskDelay(100);

	}

}
/********MAIN********/

int main(void) {

	prvSetupHardware();

	xTaskCreate( lcd_task, ( signed char * ) "lcd", configMINIMAL_STACK_SIZE+100, NULL, 5, &xHandleLCD);
	xTaskCreate( sensor_task, ( signed char * ) "sensor", configMINIMAL_STACK_SIZE+150, NULL, 5, &xHandlesensor);

	vTaskStartScheduler();

	while (1)
		;
}

/*****END OF FILE****/
