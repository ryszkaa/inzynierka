#ifndef _TS_I2C_H
#define _TS_I2C_H


void TS_Init (void);

 void TS_Write (unsigned char reg, unsigned char num, unsigned int val) ;

unsigned int TS_Read (unsigned char reg, unsigned char num);



#endif

