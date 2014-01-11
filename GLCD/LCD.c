#include <stdio.h>
#include <stdlib.h>
#include "stm32f10x.h"
#include "stm32_eval.h"
#include "TS_I2C.h"
#include "GLCD.h"
#include <FreeRTOS.h>
#include "task.h"
#include "queue.h"
#include "touch_panel.h"
#include "itoa.h"
#include "LCD.h"
#include "RTC.h"


//extern volatile unsigned char temperatura[25];
//extern volatile unsigned char wartoscADC1VTekst[25];
//
//int tch_x = 0, tch_y = 0, tch_z = 0;
//int tch_int = 0, tch_on = 0, tch_on_ = 0;
//int TC77_temperatura = 3, temperatura_temp = 4, wartoscADC1V;
////volatile unsigned char temperatura[25];
//
//void GLCD_Menu(void) {
//	printf("menu glcd \n");
//	int ad_val = 0, ad_avg = 0;
////			ad_val_ = 0xFFFF;
//	int state_flag = 0;
//	//int i = 0;
//
//	for (;;) { /* Loop forever                       */
//		/* Collect all input signals                                              */
//		/* AD converter input                                                     */
//		if (ADC1->SR & (1 << 1)) /* If conversion has finished         */
//		{
//			ad_avg += (ADC1->DR & 0x0FFF) << 8; /* Add AD value to averaging          */
//			ad_avg++;
//			ADC1->CR2 |= 1 << 22; /* Start new conversion               */
//			if ((ad_avg & 0xFF) == 0x10) {
//				ad_val = ad_avg >> 12;
//				ad_avg = 0;
//			}
//		}
//
//		/* Show all signals                                                       */
//		switch (state_flag) {
//		case 0: /* Display Background */
//			printf("case 0 \n");
//			GLCD_Clear(Tlo);
//			GLCD_SetBackColor(Tlo);
//			GLCD_SetTextColor(Olive);
//			GLCD_DisplayString(4, 2, "praca inzynierska ");
//			state_flag++;
//			break;
//
//		case 1: /* Display static text                */
//			printf("case 1 \n");
//
//			get_time();
//
//
//			/* Setup Touch-Screen controller      */
//			//TouchPanel_Conf();
//			state_flag++;
//			break;
//
//		case 2:
//			//printf("x= %d ,y= %d\n",tch_x, tch_y);
//			/* Show touch button status  */
//			//przycisk 1
//			get_time();
//			GLCD_DisplayString(1, 0, stanRTCTekst);
//			GLCD_DisplayString(3, 0, temperatura);
//			//sprintf((char *) wartoscADC1VTekst, "Wiglotnosc: %d%%", wartoscADC1V); //Przekszta³cenie wyniku na tekst, dzielenie calkowite wyzancza wartosc w V, dzielenie modulo - czesc po przecinku
//			GLCD_DisplayString(4, 0, wartoscADC1VTekst);
//			if ((tch_x) && (tch_y)) {
//
//				//1
//
//				if ((tch_x <= 3500) && (tch_y >= 380) && (tch_y <= 1300)) //!!!
//						{
//
//					if (tch_on) {
//
//						GLCD_Bmp(35, 180, 64, 30, one_on); //1
//						GLCD_Bmp(125, 180, 64, 30, two_off); //2
//						GLCD_Bmp(215, 180, 64, 30, three_off); //3
//						GLCD_Bmp(195, 120, 60, 25, off); //off
//
//						STM_EVAL_LEDOn(LED1);
//						STM_EVAL_LEDOff(LED2);
//						STM_EVAL_LEDOff(LED3);
//
//					}
//				}
//
//				if (!tch_on) {
//
//				}
//
//				tch_on_ = tch_on;
//			}
//			//2
////			(tch_on ^ tch_on_)
//			if ((tch_x) && (tch_y)) {
//
//				if ((tch_x >= 2870) && (tch_x <= 3500) && (tch_y >= 1500) && (tch_y <= 2400)) //!!!
//						{
//
//					if (tch_on) {
//
//						GLCD_Bmp(35, 180, 64, 30, one_on); //1
//						GLCD_Bmp(125, 180, 64, 30, two_on); //2
//						GLCD_Bmp(215, 180, 64, 30, three_off); //3
//						GLCD_Bmp(195, 120, 60, 25, off); //off
//
//						STM_EVAL_LEDOn(LED1);
//						STM_EVAL_LEDOn(LED2);
//						STM_EVAL_LEDOff(LED3);
//
//					}
//				}
//
//				if (!tch_on) {
//
//				}
//
//				tch_on_ = tch_on;
//			}
//
//			//3
//			if ((tch_x) && (tch_y)) {
//
//				if ((tch_x >= 3000) && (tch_x <= 3500) && (tch_y >= 2550) && (tch_y <= 3600)) //!!!
//						{
//
//					if (tch_on) {
//
//						GLCD_Bmp(35, 180, 64, 30, one_on); //1
//						GLCD_Bmp(125, 180, 64, 30, two_on); //2
//						GLCD_Bmp(215, 180, 64, 30, three_on); //3
//						GLCD_Bmp(195, 120, 60, 25, off); //off
//
//						STM_EVAL_LEDOn(LED1);
//						STM_EVAL_LEDOn(LED2);
//						STM_EVAL_LEDOn(LED3);
//
//					}
//				}
//
//				if (!tch_on) {
//
//				}
//
//				tch_on_ = tch_on;
//			}
//
//			//off
//
//			if ((tch_on ^ tch_on_) && (tch_x) && (tch_y)) {
//
//				if ((tch_x >= 250) && (tch_x <= 740) && (tch_y >= 2400) && (tch_y <= 3600)) //!!!
//						{
//
//					if (tch_on) {
//
//						GLCD_Bmp(35, 180, 64, 30, one_off); //1
//						GLCD_Bmp(125, 180, 64, 30, two_off); //2
//						GLCD_Bmp(215, 180, 64, 30, three_off); //3
//						GLCD_Bmp(195, 120, 60, 25, off); //off
//
//						STM_EVAL_LEDOff(LED1);
//						STM_EVAL_LEDOff(LED2);
//						STM_EVAL_LEDOff(LED3);
//
//					}
//				}
//
//				if (!tch_on) {
//
//				}
//
//				tch_on_ = tch_on;
//			}
//
//			break;
//
//		}
//
//	}
//}
