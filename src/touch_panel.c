/*
 * touch_panel.c
 * */

#include <stdint.h>
#include "touch_panel.h"
#include <TS_I2C.h>



void TouchPanel_Conf(void)
{
					uint32_t i;

					TS_Init  ();                    /* Initialize Touch-Screen controller */
					TS_Write (0x03, 1, 0x02);       /* Reset Touch-screen controller      */
					for (i = 0; i < 180000; i++);   /* Wait minimum of 10ms               */
					TS_Write (0x04, 1, 0x0C);       /* Enable TSC and ADC                 */
					TS_Write (0x0A, 1, 0x07);       /* Enable Touch detect, FIFO          */
					TS_Write (0x20, 1, 0x69);       /* Set sample time , 12-bit mode      */
					for (i = 0; i < 36000; i++);    /* Wait minimum of 2ms                */
					TS_Write (0x21, 1, 0x01);       /* ADC frequency 3.25 MHz    */
					TS_Write (0x41, 1, 0xF5);       /* Set TSC_CFG register      */
					TS_Write (0x4A, 1, 0x01);       /* Threshold for FIFO        */
					TS_Write (0x4B, 1, 0x01);       /* FIFO reset       */
					TS_Write (0x4B, 1, 0x00);       /* FIFO not reset   */
					TS_Write (0x56, 1, 0x07);       /* Fraction z       */
					TS_Write (0x58, 1, 0x01);       /* Drive 50 mA typical       */
					TS_Write (0x40, 1, 0x01);       /* Enable TSC       */
					TS_Write (0x0B, 1, 0xFF);       /* Clear interrupt status    */
					TS_Write (0x09, 1, 0x03);       /* Enable global interrupt   */
}


static uint32_t transform_single(uint32_t adc, uint32_t transformed_max, uint32_t min, uint32_t max)
{
	if (adc <= min)
		return 0;

	adc -= min;
	adc *= transformed_max;
	adc /= max - min;

	if  (adc > transformed_max)
		adc = transformed_max;

	return adc;
}

point_t transform(uint32_t adc_x, uint32_t adc_y)
{
	point_t point;

	//x=>y and y=>x becouse of the GLCD bitmap function (its easier)

	point.y = transform_single(adc_x, TRANSFORMED_X, X_MIN, X_MAX);
	point.x = transform_single(adc_y, TRANSFORMED_Y, Y_MIN, Y_MAX);

	return point;
}


