/* *********************************
 * *******Praca inżynierska*********
 * *********************************
 * ********Aleksandra Rys***********
 * **Elektronika i Telekomunikacja**
 * *********************************
 * *********************************
 * ********************************* */
#include <stdio.h>
#include <string.h>
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
#include "queue.h"
#include "itoa.h"
#include "touch_panel.h"
#include "FreeRTOSConfig.h"
#include "RTC.h"
#include "stm32f10x_rcc.h"
#include "stm32f10x_rtc.h"

#define SR 8
void prvSetupHardware(void);

#include "main.h"

/******functions******/
void GLCDTask(void * pvParameters) { //* pvParameters

	xQueueHandle * pGLCDQueue;
	GLCDQueueItem xMessageToSend;

	pGLCDQueue = (xQueueHandle *) pvParameters;

	int tch_x = 0, tch_y = 0, tch_on = 0;

	GLCD_Init();
	GLCD_Clear(Blue);

	GLCD_Clear(Tlo);
	GLCD_SetBackColor(Tlo);
	GLCD_SetTextColor(Olive);
	GLCD_DisplayString(4, 2, "Praca inzynierska ");
	GLCD_Clear(Tlo);
	GLCD_SetBackColor(Tlo);
	GLCD_SetTextColor(Szary);

	GLCD_DisplayString(1, 0, "Zegar : ");
	GLCD_DisplayString(2, 0, "imer: 0:00:00");

	GLCD_DisplayString(5, 0, "Owietlenie:");
	GLCD_DisplayString(6, 0, "Stan pracy:");

	//przyciski
	GLCD_Bmp(35, 180, 64, 30, one_off); //1
	GLCD_Bmp(125, 180, 64, 30, two_off); //2
	GLCD_Bmp(215, 180, 64, 30, three_off); //3
	GLCD_Bmp(205, 120, 60, 25, off); //off
	GLCD_Bmp(245, 45, 58, 22, on_timer); //on_timer

	for (;;) {

		if (xQueueReceive(*pGLCDQueue, &xMessageToSend, portMAX_DELAY)) {

			switch (xMessageToSend.Mode) {
			case Text:
				GLCD_DisplayString(xMessageToSend.CoordinatesX, xMessageToSend.CoordinatesY, xMessageToSend.Text);
				break;
			case Touch:
				printf("x= %d ,y= %d\n", tch_x, tch_y);
				tch_x = xMessageToSend.CoordinatesX;
				tch_y = xMessageToSend.CoordinatesY;
				tch_on = xMessageToSend.CoordinatesZ;
				if ((tch_x) && (tch_y)) {

					//1

					if ((tch_x <= 3500) && (tch_y >= 380) && (tch_y <= 1300)) //!!!
							{

						if (tch_on) {

							GLCD_Bmp(35, 180, 64, 30, one_on); //1
							GLCD_Bmp(125, 180, 64, 30, two_off); //2
							GLCD_Bmp(215, 180, 64, 30, three_off); //3
							GLCD_Bmp(205, 120, 60, 25, off); //off

							STM_EVAL_LEDOn(LED1);
							STM_EVAL_LEDOff(LED2);
							STM_EVAL_LEDOff(LED3);

						}
					}

					if (!tch_on) {

					}

				}
				//2

				if ((tch_x) && (tch_y)) {

					if ((tch_x >= 2870) && (tch_x <= 3500) && (tch_y >= 1500) && (tch_y <= 2400)) //!!!
							{

						if (tch_on) {

							GLCD_Bmp(35, 180, 64, 30, one_off); //1
							GLCD_Bmp(125, 180, 64, 30, two_on); //2
							GLCD_Bmp(215, 180, 64, 30, three_off); //3
							GLCD_Bmp(205, 120, 60, 25, off); //off

							STM_EVAL_LEDOff(LED1);
							STM_EVAL_LEDOn(LED2);
							STM_EVAL_LEDOff(LED3);

						}
					}

					if (!tch_on) {

					}

				}

				//3
				if ((tch_x) && (tch_y)) {

					if ((tch_x >= 3000) && (tch_x <= 3500) && (tch_y >= 2550) && (tch_y <= 3600)) //!!!
							{

						if (tch_on) {

							GLCD_Bmp(35, 180, 64, 30, one_off); //1
							GLCD_Bmp(125, 180, 64, 30, two_off); //2
							GLCD_Bmp(215, 180, 64, 30, three_on); //3
							GLCD_Bmp(205, 120, 60, 25, off); //off

							STM_EVAL_LEDOff(LED1);
							STM_EVAL_LEDOff(LED2);
							STM_EVAL_LEDOn(LED3);

						}
					}

					if (!tch_on) {

					}

				}

				//off

				if ((tch_x) && (tch_y)) {

					if ((tch_x >= 2100) && (tch_x <= 2500) && (tch_y >= 2600) && (tch_y <= 3000)) //!!!
							{

						if (tch_on) {

							GLCD_Bmp(35, 180, 64, 30, one_off); //1
							GLCD_Bmp(125, 180, 64, 30, two_off); //2
							GLCD_Bmp(215, 180, 64, 30, three_off); //3
							GLCD_Bmp(205, 120, 60, 25, off); //off

							STM_EVAL_LEDOff(LED1);
							STM_EVAL_LEDOff(LED2);
							STM_EVAL_LEDOff(LED3);

						}
					}

					if (!tch_on) {

					}

				}
				//timer

				if ((tch_x) && (tch_y)) {

					if ((tch_x >= 900) && (tch_x <= 1300) && (tch_y >= 3100) && (tch_y <= 3500)) //!!!
							{

						if (tch_on) {

							EXTI_InitTypeDef EXTI_InitStructure;
							//konfiguracja przerwań pochodzących od RTC Alarm
							EXTI_ClearITPendingBit(EXTI_Line17);
							EXTI_InitStructure.EXTI_Line = EXTI_Line17;
							EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
							EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;
							EXTI_InitStructure.EXTI_LineCmd = ENABLE;
							EXTI_Init(&EXTI_InitStructure);
							//generowanie przerwania przez RTC Alarm
							RTC_ITConfig(RTC_IT_ALR, ENABLE);
							//odczekanie na zakończenie operacji
							RTC_WaitForLastTask();

							RTC_WaitForLastTask();
							//ustawienie rejestru alarmu RTC na 12h00m00s
							RTC_SetAlarm((RTC_GetCounter() + ALARM_5MIN) % (86400 - 1));
							//odczekanie na zakończenie operacji na RTC
							RTC_WaitForLastTask();
							AlarmStatus = 1;
							StanAlarm = 0;

						}
					}

					if (!tch_on) {

					}

				}

				break;
			default:
				break;

			}

		}

	}

}
void TempHumidSensorTask(void * pvParameters) {

	portTickType t;
	xQueueHandle *pGLCDQueue;
	GLCDQueueItem xMessageToSend;
	int8_t temperatura[GLCD_TEXT_SIZE];
	int8_t wartoscADC1VTekst[GLCD_TEXT_SIZE];
	int temperatura_temp = 4;

	pGLCDQueue = (xQueueHandle *) pvParameters;

	static uint16_t sr[SR];
	static uint8_t idx;
	uint32_t sr1 = 0;
	uint8_t i;
	uint8_t start = 0;
	uint32_t tc77_temperatura;

	//konfigurowanie interfejsu SPI
	SPI_InitTypeDef SPI_InitStruct;

	SPI_InitStruct.SPI_Direction = SPI_Direction_1Line_Rx; //transmisja z wykorzystaniem jednej linii, transmisja jednokierunkowa
	SPI_InitStruct.SPI_Mode = SPI_Mode_Slave; //tryb pracy SPI
	SPI_InitStruct.SPI_DataSize = SPI_DataSize_16b; //16-bit ramka danych
	SPI_InitStruct.SPI_CPOL = SPI_CPOL_Low; //stan sygnalu taktujacego przy braku transmisji - niski
	SPI_InitStruct.SPI_CPHA = SPI_CPHA_1Edge; //aktywne zbocze sygnalu taktujacego - 1-sze zbocze
	SPI_InitStruct.SPI_NSS = SPI_NSS_Hard; //sprzetowa obsluga linii NSS (CS)
	SPI_InitStruct.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_256; //prescaler szybkosci tansmisji  36MHz/256=140.625kHz
	SPI_InitStruct.SPI_FirstBit = SPI_FirstBit_MSB; //pierwszy bit w danych najbardziej znaczacy
	SPI_InitStruct.SPI_CRCPolynomial = 7; //stopien wielomianu do obliczania sumy CRC
	SPI_Init(SPI2, &SPI_InitStruct); //inicjalizacja SPI
	SPI_SSOutputCmd(SPI2, ENABLE);

	SPI_Cmd(SPI2, ENABLE); // Wlacz SPI2

	ADC_Conf();

	t = xTaskGetTickCount();
	while (1) {

		SPI_InitStruct.SPI_Mode = SPI_Mode_Master;
		SPI_Init(SPI2, &SPI_InitStruct);

		while (SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_RXNE) == RESET) {

		}

		tc77_temperatura = SPI_I2S_ReceiveData(SPI2) >> 3;

		SPI_InitStruct.SPI_Mode = SPI_Mode_Slave;
		SPI_Init(SPI2, &SPI_InitStruct);

		temperatura_temp = tc77_temperatura * 6;

		sprintf(temperatura, "Temperatura: %d,%01d*C", temperatura_temp / 100, (temperatura_temp % 100) / 10);
//		printf("poieram temp %s \n", temperatura);

		xMessageToSend.Mode = Text;
		xMessageToSend.CoordinatesX = 3;
		xMessageToSend.CoordinatesY = 0;
		xMessageToSend.Text = &temperatura;

		if (!(xQueueSend(*pGLCDQueue,(void *) &xMessageToSend, (portTickType) 0))) {
			printf("blad kolejki\n");
		}

		if (0 == start) {
			uint16_t temp = ADC_GetConversionValue(ADC1);
			for (i = 0; i < SR; i++) {
				sr[i] = temp;
			}
			start = 1;
		} else {
			sr[idx++ & (SR - 1)] = ADC_GetConversionValue(ADC1);
		}

		for (i = 0; i < SR; i++)
			sr1 += sr[i];
		sr1 /= SR;

		uint32_t wartoscADC1 = 0;
		uint32_t wartoscADC1V = 0;

		wartoscADC1 = sr1;
		wartoscADC1V = ((wartoscADC1 * 100));
		wartoscADC1V >>= 12;

		sprintf((char *) wartoscADC1VTekst, "Wiglotnosc:  %lu%%", wartoscADC1V); //Przekszta³cenie wyniku na tekst, dzielenie calkowite wyzancza wartosc w V, dzielenie modulo - czesc po przecinku
//		printf("wilgotnosc: %u \n", ADC_GetConversionValue(ADC1));

		xMessageToSend.Mode = Text;
		xMessageToSend.CoordinatesX = 4;
		xMessageToSend.CoordinatesY = 0;
		xMessageToSend.Text = &wartoscADC1VTekst;

		if (!(xQueueSend(*pGLCDQueue,(void *) &xMessageToSend, (portTickType) 0))) {
			printf("blad kolejki\n");

		}

		vTaskDelayUntil(&t, 500);

	}

}
/********MAIN********/

#define GLCD_QUEUE_LENGTH ( 24 )

volatile xQueueHandle xGLCDQueue;

int main(void) {

	xGLCDQueue = xQueueCreate(GLCD_QUEUE_LENGTH,sizeof(GLCDQueueItem));

	prvSetupHardware();

	xTaskCreate( GLCDTask, ( signed char * ) "GLCDTask", configMINIMAL_STACK_SIZE+200, (void *) &xGLCDQueue, 5, NULL);

	xTaskCreate( TempHumidSensorTask, ( signed char * ) "TempHumidSensor", configMINIMAL_STACK_SIZE+300, (void *) &xGLCDQueue, 5, NULL);

	vTaskStartScheduler();

	while (1)
		;
}

/*****END OF FILE****/
