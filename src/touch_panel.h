/*
 * touch_panel.h
 *
 *  Created on: Jul 20, 2011
 *      Author: Dariusz Bembenek
 */

#ifndef TOUCH_PANEL_H_
#define TOUCH_PANEL_H_

#define X_MIN								320
#define X_MAX								3750
#define Y_MIN								350
#define Y_MAX								3860
#define TRANSFORMED_X						240
#define TRANSFORMED_Y						320

typedef struct point_s
{
	uint16_t x;
	uint16_t y;
} point_t;

point_t point;

point_t transform(uint32_t adc_x, uint32_t adc_y);
void TouchPanel_Conf(void);

#endif /* TOUCH_PANEL_H_ */
